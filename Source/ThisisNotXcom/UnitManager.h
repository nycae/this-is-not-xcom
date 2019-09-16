// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "UnitType.h"
#include "GameFramework/Actor.h"
#include "UnitManager.generated.h"


class ATeamLeader;


UENUM(BlueprintType)
enum class EUnitTypeEnum : uint8
{
	TCE_Pawn	UMETA(DisplayName = "Pawn"),
	TCE_Cham	UMETA(DisplayName = "Champion"),
	TCE_Ranger	UMETA(DisplayName = "Ranger"),
	TCE_MAX		UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ETeamEnum : uint8
{
	TE_White	UMETA(DisplayName = "White"),
	TE_Blue 	UMETA(DisplayName = "Blue"),
	TE_Black	UMETA(DisplayName = "Black"),
	TE_Grass	UMETA(DisplayName = "Grass"),
	TE_MAX		UMETA(DisplayName = "Max")
};

USTRUCT(BlueprintType)
struct FMeshKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ETeamEnum Team;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EUnitTypeEnum Type;

	friend FORCEINLINE int32 GetTypeHash(FMeshKey Key)
	{
		return ((uint8)Key.Team >= (uint8)Key.Type)
			? (uint8)Key.Team * (uint8)Key.Team + (uint8)Key.Team + (uint8)Key.Type
			: (uint8)Key.Team + (uint8)Key.Type * (uint8)Key.Type;
	};

	bool operator< (FMeshKey Other) const
	{
		return (Team < Other.Team) ? true : (Team > Other.Team) ? false : (Type > Other.Type) ;
	};
};


UCLASS()
class THISISNOTXCOM_API AUnitManager : public AActor
{
	GENERATED_BODY()

public:

	AUnitManager();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit Config")
		TMap<EUnitTypeEnum, TSubclassOf<AActor>> UnitClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit Config")
		TMap<EUnitTypeEnum, int32> UnitCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Teams")
		TMap<FMeshKey, USkeletalMesh*> TeamMeshes; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teams")
		TMap<ETeamEnum, ATeamLeader*> Teams;

};
