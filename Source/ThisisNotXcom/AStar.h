// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Grid.h"
#include "State.h"
#include "Position.h"

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

	bool isPossiblePathExisting(const FPosition& InitialState, const FPosition& FinalState);

private:

	TArray<State> StateCollection;

	TBitArray<FDefaultBitArrayAllocator> Visited;

	AGrid* BattleGrid;

	int32 MaxX;

	int32 MaxY;

	void ExploreNewLevel(const State& StateArg);

};
