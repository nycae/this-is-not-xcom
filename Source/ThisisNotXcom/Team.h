// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamLeader.h"
#include "UObject/NoExportTypes.h"
#include "Team.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API UTeam : public UObject
{
	GENERATED_BODY()
	
public:

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
		TWeakObjectPtr<UTeamLeader>									Leader;

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Army")
		TArray<TWeakObjectPtr<AChampion>, TFixedAllocator<3>>		Champions;

//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Army")
		TArray<TWeakObjectPtr<APawnTrooper>, TFixedAllocator<5>>	Pawns;

};
