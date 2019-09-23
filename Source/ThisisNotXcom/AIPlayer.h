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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map assets")
		int32 MapMaxX = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map assets")
		int32 MapMaxY = 8;

protected:

	virtual void BeginPlay() override;

private:

	bool bShouldIAttentATurn = false;

	AUnit* Unit;

	UPROPERTY(VisibleAnywhere, Category = "Debug assets")
		FPosition TargetPosition;

	FPosition UnitPosition;

	TArray<FPosition> EnemyPositions;

private:

	constexpr int32 Offset(const FPosition& Begin, const FPosition& Target);

	FPosition GetRegroupPosition();

	FPosition GetPositionCloseToObjective();

	FPosition GetPositionFromPath(const TArray<EDirectionEnum>& Path);

	TArray<FPosition> GetEnemyPositions();

	TArray<FPosition> GetNearbyPositions(const FPosition& Position);

	bool bCanReachObjective();

	bool bHasEnemiesNearby();

	void FindNewObjective();

	void MoveTowardsObjective();

	void AttackWeakestEnemyNearby();

	void Attack(const FPosition& Position);

	void MoveTowards(const FPosition& EnemyPosition);

};
