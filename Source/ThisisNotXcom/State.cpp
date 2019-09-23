
#include "State.h"

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