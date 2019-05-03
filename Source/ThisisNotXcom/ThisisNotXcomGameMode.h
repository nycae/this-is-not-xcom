// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Team.h"
#include "ScoreManager.h"
#include "GameFramework/GameModeBase.h"
#include "ThisisNotXcomGameMode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GS_Init				UMETA(DisplayName="Initializing"),
	GS_PlayerOneTurn	UMETA(DisplayName="Player One Turn"),
	GS_PlayerTwoTurn	UMETA(DisplayName="Player Two Turn"),
	GS_Ending			UMETA(DisplayName="Ending"),
	GS_MAX				UMETA(DisplayName="MAX")
};

UCLASS()
class AThisisNotXcomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AThisisNotXcomGameMode();

	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite)
		UScoreManager* ScoreManager;

	UPROPERTY(BlueprintReadWrite)
		UTeam* PlayerOne;

	UPROPERTY(BlueprintReadWrite)
		UTeam* PlayerTwo;
};



