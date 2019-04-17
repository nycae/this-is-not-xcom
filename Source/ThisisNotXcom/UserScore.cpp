// Fill out your copyright notice in the Description page of Project Settings.

#include "UserScore.h"

FUserScore::FUserScore(const FString& Name, const int32& Num)
	: Score(Num), PlayerName(Name)
{

}

FUserScore::FUserScore(const TTuple<FString, int32>& Pair)
	: FUserScore(Pair.Key, Pair.Value)
{

}