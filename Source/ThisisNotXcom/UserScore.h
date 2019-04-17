// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "UserScore.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FUserScore
{
	GENERATED_BODY()

	FUserScore(const FString& Name = "", const int32& Num = 0);

	FUserScore(const TTuple<FString, int32>& Pair);

	UPROPERTY(BlueprintReadWrite, Category = "Score")
		int32 Score;

	UPROPERTY(BlueprintReadWrite, Category = "Score")
		FString PlayerName;
};

struct ScorePriority
{
	bool operator() (const FUserScore& A, const FUserScore& B) const
	{
		return A.Score > B.Score;
	}
};