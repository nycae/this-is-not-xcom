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
		void CarryOnBestActions(AUnit* Unit);

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

	FPosition WeakestEnemyPosition;

private:

	int32 AllyNearbyHeuristic(const FPosition& Position, TArray<FPosition>& AllyPositions);

	int32 GetNearestEnemies(AUnit* Unit, TArray<FPosition>& Output);

	int32 GetNearestAllies(AUnit* Unit, TArray<FPosition>& Output);

	FPosition FindPositionCloseToAllies(AUnit* Unit, TArray<FPosition>& AllyPositions);

	FPosition FindPositionCloseToWeakEnemy(AUnit* Unit, TArray<FPosition>& EnemyPositions);

	FPosition FindAttackPositionCloseTo(AUnit* Unit, const FPosition& Position, TArray<FPosition>& EnemyPositions);

	TArray<FPosition> FindEnemyPositions();

	TArray<FPosition> FindAllyPositions();

	TArray<FPosition> GetNearbyPositions(const FPosition& Position);

	TArray<TPair<int32, FPosition>> GetPositionOffsets(AUnit* Unit);

	TArray<TPair<int32, FPosition>> GetAllyPositionOffsets(AUnit* Unit);

	bool bCanItKillANearbyEnemy(AUnit* Unit, TArray<FPosition>& EnemyPositions);

	void AttendEnemiesOnSide(AUnit* Unit, TArray<FPosition>& EnemyPositions, TArray<FPosition>& AllyPositions);

	void AttendReachableEnemies(AUnit* Unit, TArray<FPosition>& EnemyPositions, TArray<FPosition>& AllyPositions);

	void AttendUnreachableEnemies(AUnit* Unit, TArray<FPosition>& EnemyPositions, TArray<FPosition>& AllyPositions);

	void Attack(AUnit* Unit, FPosition Position, TArray<FPosition>& EnemyPositions);

	void MoveTowardEnemies(AUnit* Unit, TArray<FPosition>& EnemyPositions);

	void MoveTowardAllies(AUnit* Unit, TArray<FPosition>& AllyPositions);

	void AttackTheWeakest(AUnit* Unit, TArray<FPosition>& EnemyPositions);

	void ExpandFringe(TArray<FPosition> Fringe);
};
