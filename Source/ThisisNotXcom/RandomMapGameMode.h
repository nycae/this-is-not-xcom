// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grid.h"
#include "GameFramework/GameModeBase.h"
#include "RandomMapGameMode.generated.h"


UCLASS()
class THISISNOTXCOM_API ARandomMapGameMode : public AThisisNotXcomGameMode
{
	GENERATED_BODY()
	
public:

	ARandomMapGameMode();

protected:

	virtual void BeginPlay() override;

private:

	AGrid* Grid;

};
