// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Grid.h"
#include "Position.h"


//typedef TPair<FPosition, AGrid*> UState;

class State
{
public:

	State(const FPosition& PositionArg = FPosition());

	int Cost() const;

	int Heuristic() const;

	bool isFinalState(const uint32& MaxX, const uint32& MaxY) const;

	const FPosition& GetPosition() const;

	uint32 GetIndex(const uint32& MaxColumn) const;

	FString ToString() const;

private:

	FPosition Position;

};

struct StatePredicate
{
	bool operator() (const State& A, const State& B) const
	{
		return (A.Cost() + A.Heuristic()) > (B.Cost() + B.Cost());
	}
};

class THISISNOTXCOM_API AStar 
{
public:

	AStar(AGrid* Grid = nullptr, const int32& MapMaxX = 0, const int32& MapMaxY = 0);

	bool isPossiblePathExisting(const FPosition& InitialState);

private:

	TArray<State> StateCollection;

	TBitArray<FDefaultBitArrayAllocator> Visited;

	AGrid* BattleGrid;

	int32 MaxX;

	int32 MaxY;

	void ExploreNewLevel(const State& StateArg);

};
