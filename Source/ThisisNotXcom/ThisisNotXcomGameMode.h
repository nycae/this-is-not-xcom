// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "TeamLeader.h"
#include "UnitManager.h"
#include "ScoreManager.h"
#include "GameFramework/GameModeBase.h"
#include "ThisisNotXcomGameMode.generated.h"

constexpr char* DefaultPlayerOneName = "PlayerOne";
constexpr char* DefaultPlayerTwoName = "PlayerTwo";

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GS_Init			UMETA(DisplayName="Initializing"),
	GS_UnitSetup	UMETA(DisplayName="Unit setup"),
	GS_PlayerTurn	UMETA(DisplayName="Player Turn"),
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

	UFUNCTION(BlueprintCallable)
		virtual void InitFinished();

	UFUNCTION(BlueprintCallable)
		virtual void AfterUnitDeployment();

	UFUNCTION(BlueprintCallable)
		virtual void OnUnitDeath(AUnit* Unit, ATeamLeader* Team);

	UFUNCTION(BlueprintCallable)
		virtual void OnEndOfTurn();

	UFUNCTION(BlueprintCallable)
		virtual void OnEndGame();

	UFUNCTION(BlueprintCallable)
		ATeamLeader* GetTeamAt(uint8 Index);

	UFUNCTION(BlueprintCallable)
		ATeamLeader* GetTeamTurn();

private:

	EGameState State;

	TArray<ATeamLeader*> Teams;

	TMap<ATeamLeader*, int32> RemainingUnits;

	uint16 TurnHolder;

	UScoreManager* ScoreManager;


public:

	void AttendTurn(ATeamLeader* Team);

};



