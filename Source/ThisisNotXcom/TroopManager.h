// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TroopManager.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API UTroopManager : public UObject
{
	GENERATED_BODY()
	
public:

	UTroopManager();

	UFUNCTION(BlueprintCallable)
		void SpawnPawnAt(const FVector& Position);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Troop Classes")
		TSubclassOf<AActor> PawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Troop Classes")
		TSubclassOf<AActor> ChampionClass;
};
