// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Position.h"
#include "Trooper.generated.h"

UCLASS()
class THISISNOTXCOM_API ATrooper : public ACharacter
{
	GENERATED_BODY()

public:

	ATrooper();

private:

	uint8 MaxHealth;

	uint8 Health;

	uint8 AttackDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Move(const FPosition& Position) const;

	virtual void Attack(const FPosition& Position) const;

	UPROPERTY(BlueprintReadWrite)
		float Speed = 0.0f;
};
