// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamManager.h"
#include "Engine.h"

ATeamManager::ATeamManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ATeamManager::BeginPlay()
{
	AActor::BeginPlay();

}

void ATeamManager::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

ATeamLeader* ATeamManager::CreateTeam(const FVector& Location, const FRotator& Rotation)
{

	return (ATeamLeader*) GetWorld()->SpawnActor(ATeamLeader::StaticClass(), &Location, &Rotation);
}