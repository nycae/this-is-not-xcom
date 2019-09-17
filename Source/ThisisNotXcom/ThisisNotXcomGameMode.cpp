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
	: ScoreManager(NewObject<UScoreManager>())
{
	PrimaryActorTick.bCanEverTick	=	false;
	DefaultPawnClass				=	NULL;
	PlayerControllerClass			=	AThisisNotXcomPlayerController::StaticClass();
	TurnCounter						=	0;
	MaxTurns						=	500;

}

void AThisisNotXcomGameMode::BeginPlay()
{
	for (TActorIterator<ATeamLeader> Itr(GetWorld()); Itr; ++Itr)
	{
		Teams.Add(*Itr);
		(*Itr)->TurnEnd.AddDynamic(this, &AThisisNotXcomGameMode::AttendNewTurn);
		
		for (AUnit* Unit : (*Itr)->Army)
		{
			Unit->OnDeath.AddDynamic(this, &AThisisNotXcomGameMode::OnUnitDeath);
		}
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);

	if (FoundActors.Num() > 0)
	{
		Cast<ATeamLeader>(FoundActors[0])->GrantTurn();
	}
	else
	{
		Teams[FMath::Rand() % Teams.Num()]->GrantTurn();
	}
}

void AThisisNotXcomGameMode::OnUnitDeath(AUnit* Unit, ATeamLeader* Team)
{
	Team->Army.Pop(Unit);
	Team->Score -= 50;

	Unit->Location->Ocupant = nullptr;
	Unit->Location->State = ETileState::TS_Empty;
	Unit->SetActorHiddenInGame(true);
	Unit->MarkPendingKill();

	FString DebugString = FString::Printf( TEXT("Player %s has no %d units"), *Team->PlayerName, Team->Army.Num());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Black, DebugString);

	if (Team->Army.Num() <= 0)
	{
		Teams.Remove(Team);

		if (Teams.Num() == 1)
		{
			FinishGame.Broadcast(Teams[0]);
		}
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
	return TurnHolder;
}

void AThisisNotXcomGameMode::AttendNewTurn(ATeamLeader* Team)
{
	TurnCounter = TurnCounter + 1;
	TurnHolder = Teams[(Teams.Find(Team) + 1) % Teams.Num()];

	TurnHolder->GrantTurn();

	if (TurnCounter >= MaxTurns)
	{
		ATeamLeader* PossibleWinner = TurnHolder;

		for (ATeamLeader* Team : Teams)
		{
			if (Team->HealthOfUnits() > PossibleWinner->HealthOfUnits())
			{
				PossibleWinner = Team;
			}
		}

		FinishGame.Broadcast(PossibleWinner);
	}
}