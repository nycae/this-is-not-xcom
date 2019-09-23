// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayer.h"
#include "GridPathfinder.h"


constexpr bool bAreAdjacent(const FPosition& Begin, const FPosition& End)
	{ return (Begin.Row + Begin.Column) - (End.Row + Begin.Column) == 1; }

AAIPlayer::AAIPlayer() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

void AAIPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AAIPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldIAttentATurn)
	{
		TimerForDelayedExecution(Team->Army);
		bShouldIAttentATurn = false;
	}

}

void AAIPlayer::TimerForDelayedExecution(TArray<AUnit*> Units)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, "Function Called");
	CarryOnBestActions(Units.Pop());

	if (Units.Num())
	{
		FTimerHandle UnusedHandle;
		FTimerDelegate BindedFunction;

		BindedFunction.BindUFunction(this, FName("TimerForDelayedExecution"), Units);
		GetWorldTimerManager().SetTimer(UnusedHandle, BindedFunction, 1.0f, false, 1.0f);
	}
	else
	{
		Team->FinishTurn();
	}
}

void AAIPlayer::PlayTurn()
{
	bShouldIAttentATurn = true;
}

TArray<FPosition> AAIPlayer::FindEnemyPositions()
{
	TArray<FPosition> UnitPositions = Grid->GetOccupiedPositions();

	for (const auto& Unit : Team->Army)
	{
		UnitPositions.Remove(Grid->GetPosition(Unit->Location));
	}

	return UnitPositions;
}

TArray<FPosition> AAIPlayer::FindAllyPositions()
{
	TArray<FPosition> UnitPositions = Grid->GetOccupiedPositions();

	for (const auto& Unit : Team->Army)
	{
		UnitPositions.Add(Grid->GetPosition(Unit->Location));
	}

	return UnitPositions;
}

TArray<FPosition> AAIPlayer::GetNearbyPositions(const FPosition& Position)
{
	return 
	{
		FPosition(Position.Row + 1, Position.Column),
		FPosition(Position.Row - 1, Position.Column),
		FPosition(Position.Row, Position.Column + 1),
		FPosition(Position.Row, Position.Column - 1)
	};
}

void AAIPlayer::CarryOnBestActions(AUnit* Unit)
{
	TArray<FPosition> CloserEnemies;
	TArray<FPosition> CloserAllies;

	int32 MinDistanceToEnemy = GetNearestEnemies(Unit, CloserEnemies);
	int32 MinDistanceToAlly = GetNearestAllies(Unit, CloserAllies);

	if (MinDistanceToEnemy == 1)
	{
		AttendEnemiesOnSide(Unit, CloserEnemies, CloserAllies);
	}
	else if (MinDistanceToEnemy <= ((Unit->Energy - Unit->AttackCost)/ Unit->MovementCost)) // Can reach and attack a unit
	{
		AttendReachableEnemies(Unit, CloserEnemies, CloserAllies);
	}
	else
	{
		AttendUnreachableEnemies(Unit, CloserEnemies, CloserAllies);
	}
}

void AAIPlayer::AttendEnemiesOnSide(AUnit* Unit, TArray<FPosition>& EnemyPositions, TArray<FPosition>& AllyPositions)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, "Enemies Nearby");
	int32 AdjacentPositions = 0;
	FPosition UnitPosition = Grid->GetPosition(Unit->Location);

	for (const FPosition& Position : GetNearbyPositions(UnitPosition))
	{
		if (Grid->Tiles.Contains(Position))
		{
			AdjacentPositions++;
		}
	}

	if (AdjacentPositions == EnemyPositions.Num()
		||
		EnemyPositions.Num() == 1)		
	{
		AttackTheWeakest(Unit, EnemyPositions);	// Intenta abrir hueco
		MoveTowardAllies(Unit, EnemyPositions);	// Si queda energía reagrupate
	}
	else if (AdjacentPositions - EnemyPositions.Num() < 2)
	{
		MoveTowardAllies(Unit,EnemyPositions);
	}
	else
	{
		if (bCanItKillANearbyEnemy(Unit, EnemyPositions)
			||
			EnemyPositions.Num() == 3)
		{
			AttackTheWeakest(Unit, EnemyPositions);
			MoveTowardAllies(Unit, EnemyPositions);
		}
		else
		{
			MoveTowardAllies(Unit, EnemyPositions);
		}
	}
}

void AAIPlayer::AttendReachableEnemies(AUnit* Unit, TArray<FPosition>& EnemyPositions, TArray<FPosition>& AllyPositions)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, "Enemies are reachable");

	FPosition FinalPosition = EnemyPositions[0];

	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Grid->At(EnemyPosition)->Ocupant->Health < Grid->At(FinalPosition)->Ocupant->Health)
		{
			FinalPosition = EnemyPosition;
		}
	}

	MoveTowardEnemies(Unit, EnemyPositions);

	FTimerHandle UnusedHandle;
	FTimerDelegate FunctionToExecute;

	FunctionToExecute.BindUFunction(this, FName("Attack"), Unit, FinalPosition, EnemyPositions);
	GetWorldTimerManager().SetTimer(UnusedHandle, FunctionToExecute, false, 5.0f);

	Attack(Unit, FinalPosition, EnemyPositions);
}

void AAIPlayer::AttendUnreachableEnemies(AUnit* Unit, TArray<FPosition>& EnemyPositions, TArray<FPosition>& AllyPositions)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, "Enemies are unreachable");
	MoveTowardEnemies(Unit, EnemyPositions);
}

void AAIPlayer::MoveTowardAllies(AUnit* Unit, TArray<FPosition>& AllyPositions)
{
	const FPosition TargetPosition = FindPositionCloseToAllies(Unit, AllyPositions);
	const FPosition CurrentPosition = Grid->GetPosition(Unit->Location);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald,
		FString::Printf(TEXT("Moving from [%d, %d] to [%d, %d]"), CurrentPosition.Row, CurrentPosition.Column, TargetPosition.Row, TargetPosition.Column));

	Unit->MoveTo(Grid->At(TargetPosition));
}

TArray<TPair<int32, FPosition>> AAIPlayer::GetPositionOffsets(AUnit* Unit)
{
	TArray<TPair<int32, FPosition>> Results;
	FPosition UnitPosition = Grid->GetPosition(Unit->Location);

	for (const FPosition& EnemyPosition : FindEnemyPositions())
	{
		Results.Add(TPair<int32, FPosition>((UnitPosition.Row - EnemyPosition.Row) + (UnitPosition.Column - EnemyPosition.Column), EnemyPosition));
	}

	return Results;
}

TArray<TPair<int32, FPosition>> AAIPlayer::GetAllyPositionOffsets(AUnit* Unit)
{
	TArray<TPair<int32, FPosition>> Results;
	FPosition UnitPosition = Grid->GetPosition(Unit->Location);

	for (const FPosition& AllyPosition : FindAllyPositions())
	{
		Results.Add(TPair<int32, FPosition>((UnitPosition.Row - AllyPosition.Row) + (UnitPosition.Column - AllyPosition.Column), AllyPosition));
	}

	return Results;
}

int32 AAIPlayer::GetNearestEnemies(AUnit* Unit, TArray<FPosition>& Output)
{
	TArray<TPair<int32, FPosition>> Offsets = GetPositionOffsets(Unit);
	int32 MinOffset = TNumericLimits<int32>::Max();

	for (const auto& Pair : Offsets)
	{
		if (Pair.Key < MinOffset)
		{
			MinOffset = Pair.Key;
		}
	}

	for (const auto& Pair : Offsets) 
	{
		if (Pair.Key == MinOffset)
		{
			Output.Add(Pair.Value);
		}
	}

	return MinOffset;
}

int32 AAIPlayer::GetNearestAllies(AUnit* Unit, TArray<FPosition>& Output)
{
	TArray<TPair<int32, FPosition>> Offsets = GetAllyPositionOffsets(Unit);
	int32 MinOffset = TNumericLimits<int32>::Max();

	for (const auto& Pair : Offsets)
	{
		if (Pair.Key < MinOffset)
		{
			MinOffset = Pair.Key;
		}
	}

	for (const auto& Pair : Offsets)
	{
		if (Pair.Key == MinOffset)
		{
			Output.Add(Pair.Value);
		}
	}

	return MinOffset;
}

void AAIPlayer::AttackTheWeakest(AUnit* Unit, TArray<FPosition>& EnemyPositions)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, "Attacking the weakest");
	FPosition FinalPosition = EnemyPositions[0];

	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Grid->At(EnemyPosition)->Ocupant->Health < Grid->At(FinalPosition)->Ocupant->Health)
		{
			FinalPosition = EnemyPosition;
		}
	}

	Attack(Unit, FinalPosition, EnemyPositions);

	if (bCanItKillANearbyEnemy(Unit, EnemyPositions) && Unit->Energy >= Unit->AttackCost)
	{
		AttackTheWeakest(Unit, EnemyPositions);
	}
}

void AAIPlayer::Attack(AUnit* Unit, FPosition Position, TArray<FPosition>& EnemyPositions)
{
	while (Grid->At(Position)->Ocupant != nullptr
		&&
		Unit->Energy > Unit->AttackCost)
	{
		Unit->Attack(Grid->At(Position));
	}

	if (Grid->At(Position)->Ocupant == nullptr)
	{
		EnemyPositions.Remove(Position);
	}
}

void AAIPlayer::MoveTowardEnemies(AUnit* Unit, TArray<FPosition>& EnemyPositions)
{
	const FPosition TargetPosition = FindPositionCloseToWeakEnemy(Unit, EnemyPositions);
	const FPosition CurrentPosition = Grid->GetPosition(Unit->Location);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, 
		FString::Printf(TEXT("Moving from [%d, %d] to [%d, %d]"), CurrentPosition.Row, CurrentPosition.Column, TargetPosition.Row, TargetPosition.Column ));

	Unit->MoveTo(Grid->At(TargetPosition));
}

FPosition AAIPlayer::FindPositionCloseToAllies(AUnit* Unit, TArray<FPosition>& AllyPositions)
{
	FPosition BestPosition;
	int32 CurrentHeuristic;

	int32 BestHeuristic = TNumericLimits<int32>::Max();
	TArray<FPosition> Fringe = { Grid->GetPosition(Unit->Location) };
	
	for (int32 i = 0; i < (Unit->Energy / Unit->MovementCost) ; i++)
	{
		ExpandFringe(Fringe);
	}

	for (const auto& Element : Fringe)
	{
		CurrentHeuristic = AllyNearbyHeuristic(Element, AllyPositions);

		if (CurrentHeuristic < BestHeuristic)
		{
			BestHeuristic = CurrentHeuristic;
			BestPosition = Element;
		}
	}

	return BestPosition;
}

FPosition AAIPlayer::FindPositionCloseToWeakEnemy(AUnit* Unit, TArray<FPosition>& EnemyPositions)
{
	int32 BestHealth = TNumericLimits<int32>::Max();
	FPosition BestTarget = EnemyPositions[0];

	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Grid->At(EnemyPosition)->Ocupant->Health < BestHealth)
		{
			BestHealth = Grid->At(EnemyPosition)->Ocupant->Health;
			BestTarget = EnemyPosition;
		}
	}

	return FindAttackPositionCloseTo(Unit, BestTarget, EnemyPositions);
}

FPosition AAIPlayer::FindAttackPositionCloseTo(AUnit* Unit, const FPosition& Position, TArray<FPosition>& EnemyPositions)
{
	FPosition BestPosition;
	int32 CurrentOffset;

	FPosition UnitPosition = Grid->GetPosition(Unit->Location);
	int32 BestOffset = TNumericLimits<int32>::Max();

	for (const auto& NearbyPosition : GetNearbyPositions(Position))
	{
		CurrentOffset = (UnitPosition.Row - NearbyPosition.Row) + (UnitPosition.Column - NearbyPosition.Column);
		
		if (CurrentOffset < BestOffset)
		{
			BestOffset = CurrentOffset;
			BestPosition = NearbyPosition;
		}
	}

	return BestPosition;
}

int32 AAIPlayer::AllyNearbyHeuristic(const FPosition& Position, TArray<FPosition>& AllyPositions)
{
	int32 Result = 0;

	for (const auto& AllyPosition : AllyPositions)
	{
		Result += (Position.Row - AllyPosition.Row) + (Position.Column - AllyPosition.Column);
	}

	return Result;
}

void AAIPlayer::ExpandFringe(TArray<FPosition> Fringe)
{
	TArray<FPosition> NewStates;

	for (const auto& Element : Fringe)
	{
		for (const auto& Position : GetNearbyPositions(Element))
		{
			if (Grid->isEmpty(Position))
			{
				NewStates.Add(Position);
			}
		}
	}

	for (const auto& State : NewStates)
	{
		NewStates.AddUnique(State);
	}
}

bool AAIPlayer::bCanItKillANearbyEnemy(AUnit* Unit, TArray<FPosition>& EnemyPositions)
{
	for (const auto& Position : EnemyPositions)
	{
		if (Grid->At(Position)->Ocupant->Health <= (Unit->Energy / Unit->AttackCost) * Unit->AttackDamage )
		{
			return true;
		}
	}

	return false;
};