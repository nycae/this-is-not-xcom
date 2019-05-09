// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "TeamLeader.h"
#include "ScoreManager.h"
#include "GameFramework/GameModeBase.h"
#include "ThisisNotXcomGameMode.generated.h"

constexpr char* DefaultPlayerOneName = "PlayerOne";
constexpr char* DefaultPlayerTwoName = "PlayerTwo";

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GS_Init			UMETA(DisplayName="Initializing"),
	GS_PlayerTurn	UMETA(DisplayName="Player One Turn"),
	GS_TurnSwap		UMETA(DisplayName="Turn Swap"),
	GS_Ending		UMETA(DisplayName="Ending"),
	GS_MAX			UMETA(DisplayName="MAX")
};

UCLASS()
class AThisisNotXcomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AThisisNotXcomGameMode();

	virtual void BeginPlay() override;

	void RegisterTeam(ATeamLeader* NewTeam);

	UFUNCTION(BlueprintCallable)
		void InitFinished();

	UFUNCTION(BlueprintCallable)
		void OnEndOfTurn();

	UFUNCTION(BlueprintCallable)
		void OnEndGame();

private:

	EGameState State;

	//TCircularQueue<ATeamLeader*> Teams;	/* No appropriate default constructor available */
	TArray<ATeamLeader*> Teams;

	int16 TurnHolder;

public:

	UPROPERTY(BlueprintReadWrite)
		UScoreManager* ScoreManager;

	UFUNCTION(BlueprintCallable)
		ATeamLeader* GetTeamAt(uint8 Index);

	UFUNCTION(BlueprintCallable)
		ATeamLeader* GetTeamTurn();

};



