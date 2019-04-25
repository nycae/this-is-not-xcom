// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team.h"
#include "GameFramework/GameState.h"
#include "ThisisNotXcomGameState.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API AThisisNotXcomGameState : public AGameState
{
	GENERATED_BODY()

public:

	AThisisNotXcomGameState();

	UTeam* FirstTeam;

	UTeam* SecondTeam;
	
};
