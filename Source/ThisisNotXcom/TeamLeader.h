// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "Trooper.h"
#include "Champion.h"
#include "PawnTrooper.h"
#include "Engine/Player.h"
#include "TeamLeader.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API UTeamLeader : public UPlayer
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		TSubclassOf<AActor> PawnUnitClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Units")
		TSubclassOf<AActor> ChampionUnitClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
		AGrid* CombatGrid;

	void SpawnPawnAt(const FPosition& Position);

	void SpawnChampionAt(const FPosition& Position);

private:

	TArray<TWeakObjectPtr<AChampion>, TFixedAllocator<3>> Champions;

	TArray<TWeakObjectPtr<APawnTrooper>, TFixedAllocator<5>> Pawns;

};
