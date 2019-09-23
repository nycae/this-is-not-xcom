// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPlayer.h"
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
	}
	else
	{
		BindedFunction.BindUFunction(Team, FName("FinishTurn"));
	}

	GetWorldTimerManager().SetTimer(UnusedHandle, BindedFunction, 1.0f, false, 1.0f);
}

void AAIPlayer::GoForTheWeakest(AUnit* ArgUnit)
{
	FTimerHandle	UnusedHandle;

	Unit			=	ArgUnit;
	UnitPosition	=	Grid->GetPosition(Unit->Location);
	EnemyPositions	=	GetEnemyPositions();
	WeakestEnemies	=	FilterPositionsByEnemyHealth();

	int32 CloserEnemyDistance = FindCloserEnemies();

	if (bIsUnitSurrounded())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "Unit is surrounded");
		AttackWeakestEnemyNearby();
	} 
	
	if (bCanItKillANearbyEnemy())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "Unit can attack");
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AAIPlayer::AttackWeakestEnemyNearby, 0.0, false, 1.0f);
	}

	if (Unit->Energy >= Unit->MovementCost)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "Unit is moving towards the target");
		MoveTowardsWeakerEnemy();
	}

	if (Unit->Energy >= Unit->AttackCost)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, "Unit is doing a desperate attack");
		AttackSomethingIfYouCan();
	}
}

int32 AAIPlayer::FindCloserEnemies()
{
	int32 CurrentOffset;

	int32 MinOffset = TNumericLimits<int32>::Max();
	FPosition UnitPosition = Grid->GetPosition(Unit->Location);

	NearbyEnemies = EnemyPositions;

	for (const auto& EnemyPosition : EnemyPositions)
	{
		CurrentOffset = Offset(UnitPosition, EnemyPosition);

		if (CurrentOffset < MinOffset)
		{
			NearbyEnemies.Empty();
			NearbyEnemies.Add(EnemyPosition);
			MinOffset = CurrentOffset;
		}
		else if (CurrentOffset == MinOffset)
		{
			NearbyEnemies.Add(EnemyPosition);
		}
	}

	return MinOffset;
}

TArray<FPosition> AAIPlayer::FilterPositionsByEnemyHealth()
{
	TArray<FPosition> FinalPositions;
	int32 MinHealth = TNumericLimits<int32>::Max();

	for (const auto& Position : EnemyPositions)
	{
		if (Grid->At(Position)->Ocupant->Health < MinHealth)
		{
			FinalPositions.Empty();
			FinalPositions.Add(Position);
			MinHealth = Grid->At(Position)->Ocupant->Health;
		}
		else if (Grid->At(Position)->Ocupant->Health == MinHealth)
		{
			FinalPositions.Add(Position);
		}
	}

	return FinalPositions;
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

TArray<FPosition> AAIPlayer::GetAllyPositions()
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
	FPosition FinalPosition = NearbyEnemies[0];

	for (const auto& NearbyEnemy : NearbyEnemies)
	{
		if (Grid->At(NearbyEnemy)->Ocupant->Health < Grid->At(FinalPosition)->Ocupant->Health)
		{
			FinalPosition = NearbyEnemy;
		}
	}

	Attack(FinalPosition);

}

void AAIPlayer::AttackSomethingIfYouCan()
{
	int32 MinHealth			=	TNumericLimits<int32>::Max();
	FPosition FinalTarget	=	FPosition();

	for (const auto& Position : NearbyEnemies)
	{
		if (Offset(UnitPosition, Position) <= Unit->MaxAttackDepth
			&&
			Grid->At(Position)->Ocupant->Health < MinHealth)
		{
			FinalTarget = Position;
			MinHealth = Grid->At(Position)->Ocupant->Health;
		}
	}

	if (FinalTarget != FPosition())
	{
		Attack(FinalTarget);
	}
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
		EnemyPositions.Remove(Position);
		WeakestEnemies.Remove(Position);
		NearbyEnemies.Remove(Position);
	}
}

void AAIPlayer::MoveTowards(const FPosition& Position)
{
	const int32 MaxDepth		=	(Unit->Energy / Unit->MovementCost);
	TArray<EDirectionEnum> Path =	GridPathfinder::GetPath(Unit->Location, Grid->At(Position), MaxDepth + 4, Grid);

	if (Path.Num() == 0)
	{
		return;
	}

	while (Path.Num() > MaxDepth)
	{
		Path.RemoveAt(Path.Num() - 1);
	}

	Unit->MoveTo(Grid->At(GetPositionFromPath(Path)));
}

void AAIPlayer::MoveTowardsWeakerEnemy()
{
	FPosition CloserPosition;
	int32 CurrentOffset, WeakestEnemiesCount;
	int32 CloserOffset = TNumericLimits<int32>::Max();

	for (int32 X = 0; X < Unit->MaxAttackDepth; X++)
	{
		WeakestEnemiesCount = WeakestEnemies.Num();

		for (int32 Index = 0; Index < WeakestEnemiesCount; Index++)
		{
			for (const auto& Candidate : GetNearbyPositions(WeakestEnemies[Index]))
			{
				if (Grid->isEmpty(Candidate))
				{
					CurrentOffset = Offset(UnitPosition, Candidate);

					if (CurrentOffset < CloserOffset)
					{
						CloserOffset = CurrentOffset;
						CloserPosition = Candidate;
					}
				}
			}
		}
	}

	MoveTowards(CloserPosition);
}

bool AAIPlayer::bCanItKillANearbyEnemy()
{
	int32 EnergyCost;
	TArray<EDirectionEnum> Path;

	for (const auto& Position : EnemyPositions)
	{
		Path = GridPathfinder::GetPath(UnitPosition, Position, Unit->Energy/Unit->MovementCost, Grid);

		if (Path.Num() > 0)
		{
			EnergyCost = ((Path.Num() - 1) * Unit->MovementCost)
				+ ((Grid->At(Position)->Ocupant->Health / Unit->AttackDamage) * Unit->AttackCost);

			if ( EnergyCost <= Unit->Energy )
			{
				return true;
			}
		}
	}

	return false;
};

bool AAIPlayer::bIsUnitSurrounded()
{
	for (const auto& AdjacentPosition : GetNearbyPositions(Grid->GetPosition(Unit->Location)))
	{
		if (Grid->isEmpty(AdjacentPosition))
		{
			return false;
		}
	}
	return true;
}