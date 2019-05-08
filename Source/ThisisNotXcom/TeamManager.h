// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamLeader.h"
#include "GameFramework/Actor.h"
#include "TeamManager.generated.h"

constexpr uint8 DefaultTeamNum = 2;

UCLASS()
class THISISNOTXCOM_API ATeamManager : public AActor
{
	GENERATED_BODY()
	
public:

	ATeamManager();

	virtual void Tick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		ATeamLeader* CreateTeam(const FVector& Location, const FRotator& Rotation);

protected:

	virtual void BeginPlay() override;

public:

	uint8 NumberOfTeams = DefaultTeamNum;

};
