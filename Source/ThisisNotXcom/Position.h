#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Position.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FPosition
{
	GENERATED_USTRUCT_BODY()

	FPosition(int32 Row = -1, int32 Column = -1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Row;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Column;

	bool operator== (const FPosition& Other) const;

	bool operator!= (const FPosition& Other) const;

	bool operator< (const FPosition& Other) const;

	friend FORCEINLINE int32 GetTypeHash(const FPosition& Position)
	{
		return (Position.Row >= Position.Column) ? Position.Row * Position.Row + Position.Row + Position.Column : Position.Row + Position.Column * Position.Column;
	};
};