// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ThisisNotXcomGameMode.h"
#include "Engine/World.h"
#include "ThisisNotXcomPlayerController.h"
#include "TeamLeader.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AThisisNotXcomGameMode::AThisisNotXcomGameMode()
	: ScoreManager(NewObject<UScoreManager>())
{
	DefaultPawnClass		=	ATeamLeader::StaticClass();
	PlayerControllerClass	=	AThisisNotXcomPlayerController::StaticClass();
	PlayerOne				=	NewObject<UTeam>();
	PlayerTwo				=	NewObject<UTeam>();

}

void AThisisNotXcomGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerOne->Leader		=	GetWorld()->SpawnActor<ATeamLeader>();
	PlayerTwo->Leader		=	GetWorld()->SpawnActor<ATeamLeader>();

}