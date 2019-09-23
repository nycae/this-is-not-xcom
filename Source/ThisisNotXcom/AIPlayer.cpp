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
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "Atending unit");
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
		Team->FinishTurn();
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
	
	if (Unit->Energy >= Unit->MovementCost)
	{
		MoveTowardsObjective();
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "Unit Cant do more");
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

	return Candidates;
}

FPosition AAIPlayer::GetRegroupPosition()
{
	int32 CurrentOffset = 0;

	for (const auto& AllyUnit : Team->Army)
	{
		if (AllyUnit != Unit)
		{
			for (const auto& NearbyPosition : GetNearbyPositions(UnitPosition))
			{
				if (AStar(Grid, MapMaxX, MapMaxY).isPossiblePathExisting(UnitPosition, NearbyPosition))
				{
					return NearbyPosition;
				}
			}
		}
	}

	return UnitPosition;
}

FPosition AAIPlayer::GetPositionCloseToObjective()
{
	int32 CurrentOffset;
	FPosition FinalPosition;

	int32 MinOffset = TNumericLimits<int32>::Max();

	for (const auto& Position : GetNearbyPositions(TargetPosition))
	{
		CurrentOffset = Offset(UnitPosition, Position);

		if (CurrentOffset < MinOffset)
		{
			MinOffset = CurrentOffset;
			FinalPosition = Position;
		}
	}
	
	return FinalPosition;
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
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, "AttackingNewObjective");
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
	TArray<EDirectionEnum> Path =	GridPathfinder::GetPath(Unit->Location, Grid->At(Position), TNumericLimits<int32>::Max(), Grid);

	while (Path.Num() > MaxDepth)
	{
		Path.RemoveAt(Path.Num() - 1);
	}

	FinalPosition = GetPositionFromPath(Path);
	UnitPosition = FinalPosition;
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString::Printf(TEXT("Unit should move to [%d, %d]"), FinalPosition.Row, FinalPosition.Column));
	Unit->MoveTo(Grid->At(FinalPosition));
}

void AAIPlayer::MoveTowardsObjective()
{
	if (bCanReachObjective())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString::Printf(TEXT("Unit is heading to [%d, %d]") ,TargetPosition.Row, TargetPosition.Column));
		MoveTowards(GetPositionCloseToObjective());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, "Unit is retreating");
		MoveTowards(GetRegroupPosition());
	}
}

void AAIPlayer::FindNewObjective()
{
	EnemyPositions = GetEnemyPositions();

	int32 MinHealth = TNumericLimits<int32>::Max();

	for (const auto& Position : EnemyPositions)
	{
		if (Grid->At(Position)->Ocupant->Health < MinHealth)
		{
			TargetPosition = Position;
		}
	}
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