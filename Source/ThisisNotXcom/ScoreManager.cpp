// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreManager.h"

UScoreManager::UScoreManager()
{

}

FUserScore UScoreManager::GetPlayerScore(const FString& PlayerName) const
{

	if (Scores.Contains(PlayerName))
		return FUserScore(PlayerName, Scores[PlayerName]);
	else
		return FUserScore("Error", -1);
}

TArray<FUserScore> UScoreManager::GetAllScores() const
{
	TArray<FUserScore> ScoreArray;

	for (const auto& Pair : Scores)
		ScoreArray.HeapPush(FUserScore(Pair), ScorePriority());

	return ScoreArray;
}

void UScoreManager::UpdateScore(const FString& PlayerName, int32 Score)
{
	if (Scores.Contains(PlayerName))
		if (Scores[PlayerName] > Score)
			return;

	Scores.Emplace(PlayerName, Score);
}