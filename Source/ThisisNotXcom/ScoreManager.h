// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserScore.h"
#include "UObject/NoExportTypes.h"
#include "ScoreManager.generated.h"


UCLASS(BlueprintType)
class THISISNOTXCOM_API UScoreManager : public UObject
{
	GENERATED_BODY()
	
public:

	UScoreManager();

	UFUNCTION(BlueprintCallable)
		FUserScore GetPlayerScore(const FString& PlayerName) const;

	UFUNCTION(BlueprintCallable)
		TArray<FUserScore> GetAllScores() const;

	UFUNCTION(BlueprintCallable)
		void UpdateScore(const FString& PlayerName, int32 Score);

private:

	TMap<FString, int32> Scores;

};

