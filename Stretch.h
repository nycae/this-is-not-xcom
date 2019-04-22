// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stretch.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStretch
{
	GENERATED_BODY()

	//FStretch(const FVector& VarOrigin = FVector(0.0f, 0.0f, 0.0f), const FVector& VarDestiny = FVector(0.0f, 0.0f, 0.0f));

	UPROPERTY(BlueprintReadWrite, Category = "Origin")
		FVector Origin;

	UPROPERTY(BlueprintReadWrite, Category = "Destiny")
		FVector Destiny;

};