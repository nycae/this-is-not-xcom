// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TroopManager.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API ATroopManager : public AActor
{
	GENERATED_BODY()
	
public:

	ATroopManager();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Troop Classes")
		TSubclassOf<AActor> PawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Troop Classes")
		TSubclassOf<AActor> ChampionClass;
};
