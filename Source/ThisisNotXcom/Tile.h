// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trooper.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class AGrid;

UENUM()
enum class ETileState : uint8
{
	TS_Empty		= 0 UMETA(DisplayName = "Empty"),
	TS_Occupied		= 1 UMETA(DisplayName = "Occupied"),
	TS_Obstructed	= 2 UMETA(DisplayName = "Obstructed"),
	TS_MAX			= 3 UMETA(DisplayName = "MAX")
};

UCLASS()
class THISISNOTXCOM_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable)
		virtual void OnClick(); 

	UFUNCTION(BlueprintCallable)
		void EndTurn();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Tile")
		void ToggleSelected();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ocupant")
		ATrooper* Ocupant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		ETileState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Is Selected")
		bool bIsSelected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent")
		AGrid* Grid;

};
