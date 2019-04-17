// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Trooper.h"
#include "Champion.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API AChampion : public ATrooper
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		virtual void Move(const FPosition& Position) const override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack(const FPosition& Position) const override;
};
