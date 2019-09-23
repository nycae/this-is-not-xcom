// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Grid.h"
#include "State.h"
#include "Position.h"
#include "TeamLeader.h"

/*
class State
{
public:

	State(const FPosition& PositionArg, ATeamLeader* Team);

	int Cost() const;

	int Heuristic() const;

	const FPosition& GetPosition() const;

	uint32 GetIndex(const uint32& MaxColumn) const;

	FString ToString() const;

private:

	FPosition Position;

	ATeamLeader* Team;

};
*/

struct StatePredicate
{
	bool operator() (ATeamLeader* Team, const State& A, const State& B) const
	{
		return (A.Cost() + A.Heuristic()) > (B.Cost() + B.Cost());
	}
};

class PositionComparator
{
public:

	static FPosition GetBetterPosition(const FPosition& Origin, int32 MaxDepth, AGrid* Grid, ATeamLeader* Team);

private:

	static void ExpandPath(TArray<State> Fringe);

};