// Fill out your copyright notice in the Description page of Project Settings.

#include "AStar.h"
#include "Engine.h"


State::State(const FPosition& PositionArg)
{
	Position = PositionArg;
}

int State::Cost() const
{
	return Position.Row + Position.Column;
}

int State::Heuristic() const
{
	return (Position.Row + Position.Column) * (Position.Row + Position.Column);
}

bool State::isFinalState(const uint32& MaxX, const uint32& MaxY) const
{
	return (Position.Row + Position.Column) == FMath::Min(MaxX, MaxY);
}

const FPosition& State::GetPosition() const
{
	return Position;
}

uint32 State::GetIndex(const uint32& MaxColumn) const
{
	return (Position.Row * MaxColumn) + Position.Column;
}

FString State::ToString() const
{
	FString Str = "[";
	Str.AppendInt(Position.Row);
	Str += ", ";
	Str.AppendInt(Position.Column);
	Str += "]";

	return Str;
}

AStar::AStar(AGrid* Grid, const int32& MapMaxX, const int32& MapMaxY)
	: Visited(false, (MapMaxX * MapMaxY) )
{
	MaxY = MapMaxY;
	MaxX = MapMaxX;

	BattleGrid = Grid;
}

bool AStar::isPossiblePathExisting(const FPosition& InitialState)
{
	State StateBuffer;

	StateCollection.HeapPush(State(InitialState), StatePredicate());

	while ( StateCollection.Num() > 0 ) {

		StateCollection.HeapPop(StateBuffer, StatePredicate());

		Visited[StateBuffer.GetIndex(MaxX)] = true;

		if (StateBuffer.isFinalState(MaxX, MaxY))
			return true;
		else
			ExploreNewLevel(StateBuffer);
	}

	return false;

}

void AStar::ExploreNewLevel(const State& StateArg)
{
	FPosition RightNeighbour(StateArg.GetPosition().Row + 1, StateArg.GetPosition().Column);
	FPosition LeftNeighbour (StateArg.GetPosition().Row - 1, StateArg.GetPosition().Column);
	FPosition UpperNeighbour(StateArg.GetPosition().Row, StateArg.GetPosition().Column + 1);
	FPosition LowerNeighbour(StateArg.GetPosition().Row, StateArg.GetPosition().Column - 1);

	State RightNeighbourState(RightNeighbour);
	State LeftNeighbourState(LeftNeighbour);
	State UpperNeighbourState(UpperNeighbour);
	State LowerNeighbourState(LowerNeighbour);

	if ((int64)RightNeighbourState.GetIndex(MaxX) < Visited.Num())
	{	
		if (!Visited[RightNeighbourState.GetIndex(MaxX)]
			&&
			!BattleGrid->isObstructed(RightNeighbour))
		{
			StateCollection.HeapPush(RightNeighbourState, StatePredicate());
		}

	}
	
	if (LeftNeighbour.Row > 0)
	{
		if (!Visited[LeftNeighbourState.GetIndex(MaxX)]
			&&
			!BattleGrid->isObstructed(LeftNeighbour))
		{
			StateCollection.HeapPush(LeftNeighbourState, StatePredicate());
		}
	}

	if ((int64)UpperNeighbourState.GetIndex(MaxX) < Visited.Num()) 
	{
		if (!Visited[UpperNeighbourState.GetIndex(MaxX)]
			&&
			!BattleGrid->isObstructed(UpperNeighbour)) 
		{
			StateCollection.HeapPush(UpperNeighbourState, StatePredicate());
		}

	}
	
	if ((int64)LowerNeighbour.Column > 0 )
	{
		if (!Visited[LowerNeighbourState.GetIndex(MaxX)]
			&&
			!BattleGrid->isObstructed(LowerNeighbour))
		{
			StateCollection.HeapPush(LowerNeighbour, StatePredicate());
		}

	}
}