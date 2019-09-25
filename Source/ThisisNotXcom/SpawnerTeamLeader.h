// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TeamLeader.h"
#include "UnitManager.h"
#include "UnitType.h"
#include "SpawnerTeamLeader.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAfterUnitSpawn, ASpawnerTeamLeader*, Team, AUnit*, Unit, EUnitTypeEnum, UnitType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNoSpawnsLeftover, EUnitTypeEnum, UnitType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndOfSpawns, ASpawnerTeamLeader*, Team);


UCLASS()
class THISISNOTXCOM_API ASpawnerTeamLeader : public ATeamLeader
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
		TMap<EUnitTypeEnum, int32> RemainingSpawns;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Assets")
		AUnitManager* UnitManager;

	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FAfterUnitSpawn AfterUnitSpawn;
	
	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FOnNoSpawnsLeftover OnNoSpawnsLeftover;
	
	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FOnEndOfSpawns OnEndOfSpawns;

public:

	UFUNCTION(BlueprintCallable)
		void SpawnUnit(EUnitTypeEnum UnitType);

	UFUNCTION(BlueprintCallable)
		void StopSpawning();
};
