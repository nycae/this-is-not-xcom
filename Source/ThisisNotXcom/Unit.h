// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Direction.h"
#include "GameFramework/Character.h"
#include "Position.h"
#include "Unit.generated.h"


class ATeamLeader;
class ATile;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitDie, AUnit*, Unit, ATeamLeader*, Team);


UCLASS()
class THISISNOTXCOM_API AUnit : public ACharacter
{
	GENERATED_BODY()

public:

	AUnit();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int64 MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int64 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int64 MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int64 Energy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int64 AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int32 MaxAttackDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int32 MovementCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		int32 AttackCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
		ATile* Location = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
		ATeamLeader* Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TArray<EDirectionEnum> TurnMovements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		FVector Origin;

	UPROPERTY(BlueprintReadWrite)
		EDirectionEnum Facing = EDirectionEnum::DE_Forward;

	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FOnUnitDie OnDeath;

protected:

	UFUNCTION(BlueprintCallable)
		FVector GetVectorFromDirection(EDirectionEnum Direction, int32 TileSize) const;

	UFUNCTION(BlueprintCallable)
		FRotator GetRotationByDirection(EDirectionEnum Direction) const;

public:

	UFUNCTION(BlueprintCallable)
		float GetHealthPercentage() const;

	UFUNCTION(BlueprintCallable)
		float GetEnergyPercentage() const;

	UFUNCTION(BlueprintCallable)
		int GetMaxMoveDepth() const;

	UFUNCTION(BlueprintCallable)
		int OnAttack(AUnit* Attacker);

	UFUNCTION(BlueprintCallable)
		void MoveTo(ATile* Tile);

	UFUNCTION(BlueprintCallable)
		void Attack(ATile* Tile);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void PlayMovementAnimation(const TArray<EDirectionEnum>& MovementList);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void PlayAttackAnimation(EDirectionEnum Direction);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void TurnTo(EDirectionEnum Direction);

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void PlayRunAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void PlayIdleAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void PlayDamagedAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations", BlueprintCallable)
		void PlayAttackMontage();

};
