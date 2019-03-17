//

#include "Position.h"


FPosition::FPosition(int32 Row, int32 Column) 
	: Row(Row), Column(Column) 
{
}

bool FPosition::operator==(const FPosition& Other) const
{
	return (Row == Other.Row && Column == Other.Column);
}

bool FPosition::operator<(const FPosition& Other) const
{
	return (Column > Other.Column) ? false : (Column < Other.Column) ? true : (Row < Other.Row);
}

/*
FORCEINLINE int32 GetTypeHash(const FPosition& Position)
{
	// Szudzik's function
	return (Position.Row >= Position.Column) ? Position.Row * Position.Row + Position.Row + Position.Column : Position.Row + Position.Column * Position.Column;
}
*/