// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayer.h"
#include "AStar.h"
#include "GridPathfinder.h"


constexpr bool bAreAdjacent(const FPosition& Begin, const FPosition& End)
	{ return (Begin.Row + Begin.Column) - (End.Row + Begin.Column) == 1; }

constexpr int32 AAIPlayer::Offset(const FPosition& Begin, const FPosition& Target)
	{ return FMath::Abs(Begin.Row - Target.Row) + FMath::Abs(Begin.Column - Target.Column); }

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

void AAIPlayer::PlayTurn()
{
	bShouldIAttentATurn = true;
}

void AAIPlayer::TimerForDelayedExecution(TArray<AUnit*> Units)
{
	FTimerHandle UnusedHandle;
	FTimerDelegate BindedFunction;

	GoForTheWeakest(Units.Pop());

	if (Units.Num())
	{
		BindedFunction.BindUFunction(this, FName("TimerForDelayedExecution"), Units);
		GetWorldTimerManager().SetTimer(UnusedHandle, BindedFunction, 1.0f, false, 1.0f);
	}
	else
	{
		BindedFunction.BindUFunction(Team, FName("FinishTurn"), Units);
		GetWorldTimerManager().SetTimer(UnusedHandle, BindedFunction, 1.0f, false, 1.0f);
	}
	
}

void AAIPlayer::GoForTheWeakest(AUnit* ArgUnit)
{
	FTimerHandle UnusedHandle;

	Unit			=	ArgUnit;
	UnitPosition	=	Grid->GetPosition(Unit->Location);

	FindNewObjective();
	

	if (bHasEnemiesNearby())
	{
		AttackWeakestEnemyNearby();
	}
	else
	{
		MoveTowardsObjective();
	}

}

TArray<FPosition> AAIPlayer::GetEnemyPositions()
{
	TArray<FPosition> UnitPositions = Grid->GetOccupiedPositions();

	for (const auto& Unit : Team->Army)
	{
		UnitPositions.Remove(Grid->GetPosition(Unit->Location));
	}

	return UnitPositions;
}

TArray<FPosition> AAIPlayer::GetNearbyPositions(const FPosition& Position)
{
	const TArray<FPosition> Candidates =
	{
		FPosition(Position.Row + 1, Position.Column),
		FPosition(Position.Row - 1, Position.Column),
		FPosition(Position.Row, Position.Column + 1),
		FPosition(Position.Row, Position.Column - 1)
	};

	TArray<FPosition> Finales;

	for (const auto& Candidate : Candidates)
	{
		if (Grid->Tiles.Contains(Candidate))
		{
			Finales.Add(Candidate);
		}
	}

	return Finales;
}

FPosition AAIPlayer::GetSideObjective()
{
	int32 MinOffset = TNumericLimits<int32>::Max();

	int32 CurrentOffset;
	TArray<FPosition> FinalPositions = {UnitPosition};

	for (const auto& EnemyPosition : EnemyPositions)
	{
		for (const auto& Position : GetNearbyPositions(EnemyPosition))
		{
			CurrentOffset = Offset(UnitPosition, Position);

			if (CurrentOffset < MinOffset)
			{
				CurrentOffset = MinOffset;
				FinalPositions.Empty();
				FinalPositions.Add(Position);
			}
			else if (CurrentOffset == MinOffset)
			{
				FinalPositions.Add(Position);
			}
		}
	}
	
	return FinalPositions[FMath::Rand() % FinalPositions.Num()];
}

FPosition AAIPlayer::GetPositionCloseToObjective()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "Unit is moving to objective");

	int32 IndexMax;
	int32 CurrentOffset;

	int32 MinOffset = TNumericLimits<int32>::Max();
	TArray<FPosition> PossiblePositions = {UnitPosition};
	TArray<FPosition> BestPositions;

	for (int32 Depth = 0; Depth < Unit->Energy / Unit->MovementCost; Depth++)
	{
		IndexMax = PossiblePositions.Num();
		
		for (int32 Index = 0; Index < IndexMax; Index++)
		{
			for (const auto& Position : GetNearbyPositions(PossiblePositions[Index]))
			{
				if (Grid->At(Position)->State == ETileState::TS_Empty)
				{
					PossiblePositions.AddUnique(Position);
					CurrentOffset = Offset(Position, TargetPosition);
					if (CurrentOffset < MinOffset)
					{
						MinOffset = CurrentOffset;
						BestPositions.Empty();
						BestPositions.AddUnique(Position);
					}
					else if (CurrentOffset == MinOffset)
					{
						BestPositions.AddUnique(Position);
					}
				}
			}
		}
	}

	return BestPositions[FMath::Rand() % BestPositions.Num()];
}

FPosition AAIPlayer::GetPositionFromPath(const TArray<EDirectionEnum>& Path)
{
	FPosition FinalPosition = Grid->GetPosition(Unit->Location);

	for (const auto& Direction : Path)
	{
		switch (Direction)
		{
			case EDirectionEnum::DE_Forward:
				FinalPosition.Row++;
				break;

			case EDirectionEnum::DE_Backward:
				FinalPosition.Row--;
				break;
			
			case EDirectionEnum::DE_Right:
				FinalPosition.Column++;
				break;

			case EDirectionEnum::DE_Left:
				FinalPosition.Column--;
				break;
		}
	}

	return FinalPosition;
}

void AAIPlayer::AttackWeakestEnemyNearby()
{
	FPosition FinalPosition;
	int32 MinHealth = TNumericLimits<int32>::Max();

	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Offset(UnitPosition, EnemyPosition) <= Unit->MaxAttackDepth)
		{
			const AUnit* Enemy = Grid->At(EnemyPosition)->Ocupant;

			if (Enemy->Health < MinHealth)
			{
				FinalPosition = EnemyPosition;
			}
		}
	}

	Attack(FinalPosition);
}

void AAIPlayer::Attack(const FPosition& Position)
{

	while (	Grid->At(Position)->Ocupant != nullptr
			&&
			Unit->Energy >= Unit->AttackCost)
	{
		Unit->Attack(Grid->At(Position));
	}

	if (Grid->At(Position)->Ocupant == nullptr)
	{
		FindNewObjective();
	}
}

void AAIPlayer::MoveTowards(const FPosition& Position)
{
	FPosition FinalPosition;

	const int32 MaxDepth		=	(Unit->Energy / Unit->MovementCost);
	TArray<EDirectionEnum> Path =	GridPathfinder::GetPath(Unit->Location, Grid->At(Position), MaxDepth, Grid);

	FinalPosition = GetPositionFromPath(Path);
	UnitPosition = FinalPosition;

	if (Grid->Tiles.Contains(FinalPosition) && Path.Num() > 0)
	{
		Unit->MoveTo(Grid->At(FinalPosition));
	}

	UnitPosition = Grid->GetPosition(Unit->Location);
}

void AAIPlayer::MoveTowardsObjective()
{
	const FPosition FinalPosition = bCanReachObjective()
		? GetPositionCloseToObjective()
		: GetSideObjective();

	MoveTowards(FinalPosition);
}

void AAIPlayer::FindNewObjective()
{
	int32 CurrentOffset;

	TArray<FPosition> PossiblePositions;
	TArray<FPosition> FinalPositions;

	int32 MinOffset = TNumericLimits<int32>::Max();
	int32 MinHealth = TNumericLimits<int32>::Max();

	EnemyPositions = GetEnemyPositions();

	for (const auto& Position : EnemyPositions)
	{
		if (Grid->At(Position)->Ocupant->Health < MinHealth)
		{
			MinHealth = Grid->At(Position)->Ocupant->Health;
			PossiblePositions.Empty();
			PossiblePositions.Add(Position);
		}
		else if (Grid->At(Position)->Ocupant->Health == MinHealth)
		{
			PossiblePositions.Add(Position);
		}
	}

	for (const auto& Position : PossiblePositions)
	{
		CurrentOffset = Offset(Position, UnitPosition);
		if (CurrentOffset < MinOffset)
		{
			CurrentOffset = MinOffset;
			FinalPositions.Empty();
			FinalPositions.Add(Position);
		}
		else if (CurrentOffset == MinOffset)
		{
			FinalPositions.Add(Position);
		}
	}

	TargetPosition = FinalPositions[FMath::Rand() % FinalPositions.Num()];
}

bool AAIPlayer::bCanReachObjective()
{
	for (const auto& NearbyPosition : GetNearbyPositions(TargetPosition))
	{
		if (AStar(Grid, MapMaxX, MapMaxY).isPossiblePathExisting(UnitPosition, NearbyPosition))
		{
			return true;
		}
	}

	return false;
}

bool AAIPlayer::bHasEnemiesNearby()
{
	for (const auto& EnemyPosition : EnemyPositions)
	{
		if (Offset(UnitPosition, EnemyPosition) <= Unit->MaxAttackDepth)
		{
			return true;
		}
	}

	return false;
}