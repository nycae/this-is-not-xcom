// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIPlayer.h"
#include "MapGenerator.h"
#include "SpawnerTeamLeader.h"
#include "AISpawnerPlayer.generated.h"


UCLASS()
class THISISNOTXCOM_API AAISpawnerPlayer : public AAIPlayer
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Assets")
		ASpawnerTeamLeader* TeamLeader;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Map Assets")
		AMapGenerator* MapGenerator;

public:

	UFUNCTION(BlueprintCallable)
		void SpawnUnit(const ATile* Tile, EUnitTypeEnum UnitType);

};
