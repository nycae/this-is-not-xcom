
// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreManager.h"
#include "Engine.h"


UScoreManager::UScoreManager()
{
	FString File;

	if (FPaths::FileExists(*(FPaths::ProjectDir() + "/Scores.txt")))
	{
		if (FFileHelper::LoadFileToString(File, *(FPaths::ProjectDir() + "/Scores.txt")))
		{
			TArray<FString> Lines;
			File.ParseIntoArray(Lines, *FString("\n"), true);

			for (const auto& Line : Lines) 
			{

				TArray<FString> Pair;
				Line.ParseIntoArray(Pair, TEXT(" "), true);

				if (Pair.Num() >= 2)
				{
					Scores.Add(Pair[0], FCString::Atoi(*Pair[1]));
				}
			}
		}
	}
}

UScoreManager::~UScoreManager()
{
	FString ScoresFilePath = FPaths::ProjectDir() + "Scores.txt";
	FString FileData = "";

	for (const auto& Pair : Scores)
	{
		FileData.Append(Pair.Key.Replace(*FString(" "), *FString("-")) + " ");
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