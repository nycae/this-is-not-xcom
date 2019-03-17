// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

// Sets default values
ATile::ATile()
	: Ocupant(nullptr), State(ETileState::TS_Empty), bIsSelected(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	AActor::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

}

