// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "TeamLeader.h"
#include "ScoreManager.h"
#include "GameFramework/GameModeBase.h"
#include "ThisisNotXcomGameMode.generated.h"

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
		UTeamLeader* PlayerOne;

	UPROPERTY(BlueprintReadWrite)
		UTeamLeader* PlayerTwo;
};



