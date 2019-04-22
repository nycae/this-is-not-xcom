// Fill out your copyright notice in the Description page of Project Settings.

#include "TroopManager.h"

ATroopManager::ATroopManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATroopManager::BeginPlay()
{

}

void ATroopManager::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
}