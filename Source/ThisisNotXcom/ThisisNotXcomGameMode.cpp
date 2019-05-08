// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThisisNotXcomGameMode.h"
#include "Engine/World.h"
#include "Engine.h"
#include "ThisisNotXcomPlayerController.h"
#include "TeamLeader.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AThisisNotXcomGameMode::AThisisNotXcomGameMode()
	: ScoreManager(NewObject<UScoreManager>()), State(EGameState::GS_Init)
{
	DefaultPawnClass		=	NULL;//ATeamLeader::StaticClass();
	PlayerControllerClass	=	AThisisNotXcomPlayerController::StaticClass();
	TurnHolder				=	0;

}

void AThisisNotXcomGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void AThisisNotXcomGameMode::InitFinished()
{
	State = EGameState::GS_PlayerTurn;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess( Teams[ /* TurnHolder % Teams.Num() */ 0] );

}

void AThisisNotXcomGameMode::OnEndOfTurn()
{
	if (TurnHolder > 500)
	{
		// TODO: Force endgame
	}

	TurnHolder++;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess( Teams[TurnHolder % Teams.Num()] );

}

void AThisisNotXcomGameMode::RegisterTeam(ATeamLeader* NewTeam)
{
	Teams.Add(NewTeam);

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