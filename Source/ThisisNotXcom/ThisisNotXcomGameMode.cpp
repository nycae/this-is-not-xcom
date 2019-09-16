// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThisisNotXcomGameMode.h"
#include "Engine/World.h"
#include "Engine.h"
#include "ThisisNotXcomPlayerController.h"
#include "TeamLeader.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

AThisisNotXcomGameMode::AThisisNotXcomGameMode()
	: State(EGameState::GS_Init), ScoreManager(NewObject<UScoreManager>())
{
	PrimaryActorTick.bCanEverTick	=	false;
	DefaultPawnClass				=	NULL;//ATeamLeader::StaticClass();
	PlayerControllerClass			=	AThisisNotXcomPlayerController::StaticClass();
	TurnHolder						=	0;

}

void AThisisNotXcomGameMode::BeginPlay()
{
	for (TActorIterator<ATeamLeader> Itr(GetWorld()); Itr; ++Itr)
	{
		RemainingUnits.Add(*Itr, (*Itr)->Army.Num());

		Teams.Add(*Itr);
		(*Itr)->OnEndTurn.AddDynamic(this, &AThisisNotXcomGameMode::AttendTurn);
		
		for (AUnit* Unit : (*Itr)->Army)
		{
			Unit->OnDeath.AddDynamic(this, &AThisisNotXcomGameMode::OnUnitDeath);
		}
	}
}

void AThisisNotXcomGameMode::InitFinished()
{
	State = EGameState::GS_UnitSetup;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess( Teams[0] );

}

void AThisisNotXcomGameMode::AfterUnitDeployment()
{
	for (TActorIterator<AUnit> Itr(GetWorld()); Itr; ++Itr)
	{
		(*Itr)->OnDeath.AddDynamic(this, &AThisisNotXcomGameMode::OnUnitDeath);
	}
}

void AThisisNotXcomGameMode::OnUnitDeath(AUnit* Unit, ATeamLeader* Team)
{
	Team->Army.Remove(Unit);
	Team->Score -= 50;
	Unit->MarkPendingKill();
	
	if (Team->Army.Num() <= 0)
	{
		OnEndGame();
	}
}

void AThisisNotXcomGameMode::OnEndOfTurn()
{
	if (TurnHolder > 500)
	{
		// TODO: Force endgame
		OnEndGame();
	}

	TurnHolder++;

	//Teams[TurnHolder % Teams.Num()]->OnNewTurn();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess( Teams[TurnHolder % Teams.Num()] );

}

ATeamLeader* AThisisNotXcomGameMode::GetTeamAt(uint8 Index)
{
	if (Teams.Num() > Index)
	{
		return Teams[Index];
	}
	else
	{
		return nullptr;
	}
}

void AThisisNotXcomGameMode::OnEndGame()
{
	for (const auto& Player : Teams)
	{
		ScoreManager->UpdateScore(Player->PlayerName, Player->Score);
	}
}

ATeamLeader* AThisisNotXcomGameMode::GetTeamTurn()
{
	return Teams[TurnHolder % Teams.Num()];
}

void AThisisNotXcomGameMode::AttendTurn(ATeamLeader* Team)
{

}