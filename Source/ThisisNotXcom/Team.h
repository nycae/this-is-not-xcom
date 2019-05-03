// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamLeader.h"
#include "Champion.h"
#include "PawnTrooper.h"
#include "GameFramework/Actor.h"
#include "Team.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API UTeam : public UObject
{
	GENERATED_BODY()
	
public:

	UTeam();

	void AddChampion(AChampion* Champion);

	void AddPawn(APawnTrooper* Pawn);

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
		ATeamLeader*			Leader;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Army")
		TArray<ATrooper*>		Champions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Army")
		TArray<ATrooper*>	Pawns;

};
