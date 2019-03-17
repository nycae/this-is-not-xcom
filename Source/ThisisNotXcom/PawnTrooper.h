// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trooper.h"
#include "PawnTrooper.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API APawnTrooper : public ATrooper
{
	GENERATED_BODY()
	
public:

	APawnTrooper();

	UFUNCTION(BlueprintCallable)
		virtual void Move(const FPosition& Position) const override;
	
	UFUNCTION(BlueprintCallable)
		virtual void Attack(const FPosition& Position) const override;

};
