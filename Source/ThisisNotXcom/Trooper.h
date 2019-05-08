// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Direction.h"
#include "GameFramework/Character.h"
#include "Position.h"
#include "Trooper.generated.h"

class ATeamLeader;

UCLASS()
class THISISNOTXCOM_API ATrooper : public ACharacter
{
	GENERATED_BODY()

public:

	ATrooper();

protected:

	uint8 MaxHealth;

	uint8 Health;

	uint8 AttackDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		FVector GetVectorFromDirection(EDirectionEnum Direction) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	uint8 MaxMoveDepth;

	uint8 MaxAttackDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATeamLeader* Team;

	UPROPERTY(BlueprintReadWrite)
		EDirectionEnum Facing = EDirectionEnum::DE_Forward;

	UFUNCTION(BlueprintCallable)
		float GetHealthPercentage() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
		void PlayMovementAnimation(const TArray<EDirectionEnum>& MovementList);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
		void PlayAttackAnimation(EDirectionEnum Direction);

};
