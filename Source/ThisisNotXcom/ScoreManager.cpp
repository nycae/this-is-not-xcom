// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreManager.h"
#include "Engine.h"


UScoreManager::UScoreManager()
{
	FString ScoresFilePath = FPaths::ProjectDir() + "Scores.txt";
	FString PlayerName, PlayerScore;
	TArray<FString> FileData;

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*ScoresFilePath))
	{
		FFileHelper::LoadFileToStringArray(FileData, *ScoresFilePath);

		for (const auto& Line : FileData)
		{
			Line.Split(TEXT(" "), &PlayerName, &PlayerScore);
			Scores.Emplace(PlayerName, FCString::Atoi(*PlayerScore));
		}
	}
}

UScoreManager::~UScoreManager()
{
	FString ScoresFilePath = FPaths::ProjectDir() + "Scores.txt";
	FString FileData = "";

	for (const auto& Pair : Scores)
	{
		FileData.Append(Pair.Key + " ");
		FileData.AppendInt(Pair.Value);
		FileData.Append("\n");
	}

	FFileHelper::SaveStringToFile(FileData, *ScoresFilePath);

}

FUserScore UScoreManager::GetPlayerScore(const FString& PlayerName) const
{

	if (Scores.Contains(PlayerName))
	{
		return FUserScore(PlayerName, Scores[PlayerName]);
	}
	else
	{
		return FUserScore("Error", -1);
	}
}

TArray<FUserScore> UScoreManager::GetAllScores() const
{
	TArray<FUserScore> ScoreArray;

	for (const auto& Pair : Scores)
	{
		ScoreArray.HeapPush(FUserScore(Pair), ScorePriority());
	}

	return ScoreArray;
}

void UScoreManager::UpdateScore(const FString& PlayerName, int32 Score)
{
	if (Scores.Contains(PlayerName))
	{
		if (Scores[PlayerName] > Score)
		{
			return;
		}
	}

	Scores.Emplace(PlayerName, Score);
}