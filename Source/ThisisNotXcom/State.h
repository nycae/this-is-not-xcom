
#include "Position.h"

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