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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int32 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		uint8 AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		uint8 MaxMoveDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		uint8 MaxAttackDepth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		FVector GetVectorFromDirection(EDirectionEnum Direction, int32 TileSize) const;

	UFUNCTION(BlueprintCallable)
		FRotator GetRotationByDirection(EDirectionEnum Direction) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATeamLeader* Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TArray<EDirectionEnum> TurnMovements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		FVector Origin;

	UPROPERTY(BlueprintReadWrite)
		EDirectionEnum Facing = EDirectionEnum::DE_Forward;

	UFUNCTION(BlueprintCallable)
		float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable)
		int OnAttack(ATrooper* Attacker);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
		void PlayMovementAnimation(const TArray<EDirectionEnum>& MovementList);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
		void PlayAttackAnimation(EDirectionEnum Direction);

};
