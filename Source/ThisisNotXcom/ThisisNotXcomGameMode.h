// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "TeamLeader.h"
#include "UnitManager.h"
#include "ScoreManager.h"
#include "GameFramework/GameModeBase.h"
#include "ThisisNotXcomGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEnd, ATeamLeader*, Winner);


UCLASS()
class AThisisNotXcomGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AThisisNotXcomGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float Delta) override;

	UFUNCTION(BlueprintCallable)
		virtual void OnUnitDeath(AUnit* Unit, ATeamLeader* Team);

	UFUNCTION(BlueprintCallable)
		virtual void OnEndGame();

	UFUNCTION(BlueprintCallable)
		ATeamLeader* GetTeamTurn();

private:

	float LastTurnTimestamp;

	int32 TurnCounter;

	ATeamLeader* TurnHolder;

	TArray<ATeamLeader*> Teams;

	UScoreManager* ScoreManager;

private:

	void PickStarter();

public:

	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FOnGameEnd FinishGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		int32 MaxTurns;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		float MaxTurnTime;

public:

	void AttendNewTurn(ATeamLeader* Team);

};



