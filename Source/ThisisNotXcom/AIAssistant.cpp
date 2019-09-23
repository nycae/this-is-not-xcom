// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAssistant.h"

// Sets default values
AAIAssistant::AAIAssistant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIAssistant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIAssistant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Orange, "I have ticked");

}

