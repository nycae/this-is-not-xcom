// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Unit.h"
#include "ThisisNotXcomGameMode.h"
#include "GameFramework/Actor.h"
#include "AIPlayer.generated.h"


UCLASS(BlueprintType)
class THISISNOTXCOM_API AAIPlayer : public AActor
{
	GENERATED_BODY()

public:

	AAIPlayer();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void GoForTheWeakest(AUnit* ArgUnit);

	UFUNCTION(BlueprintCallable)
		void PlayTurn();

	UFUNCTION(BlueprintCallable)
		void TimerForDelayedExecution(TArray<AUnit*> Units);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map assets")
		AGrid* Grid = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map assets")
		ATeamLeader* Team = nullptr;

protected:

	virtual void BeginPlay() override;

private:

	bool bShouldIAttentATurn = false;

	AUnit* Unit;

	FPosition UnitPosition;

	TArray<FPosition> EnemyPositions;

	TArray<FPosition> WeakestEnemies;

	TArray<FPosition> NearbyEnemies;

private:

	constexpr int32 Offset(const FPosition& Begin, const FPosition& Target);

	int32 FindCloserEnemies();

	FPosition GetPositionFromPath(const TArray<EDirectionEnum>& Path);

	TArray<FPosition> GetEnemyPositions();

	TArray<FPosition> GetAllyPositions();

	TArray<FPosition> GetNearbyPositions(const FPosition& Position);

	TArray<FPosition> FilterPositionsByEnemyHealth();

	bool bIsUnitSurrounded();

	bool bCanItKillANearbyEnemy();

	void MoveTowardsWeakerEnemy();

	void AttackSomethingIfYouCan();

	void AttackWeakestEnemyNearby();

	void Attack(const FPosition& Position);

	void MoveTowards(const FPosition& EnemyPosition);

};
