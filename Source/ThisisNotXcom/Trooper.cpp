// Fill out your copyright notice in the Description page of Project Settings.

#include "Trooper.h"
#include "ConstructorHelpers.h"

// Sets default values
ATrooper::ATrooper() 
	: Super(), MaxHealth(50), Health(50), AttackDamage(15)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ATrooper::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATrooper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATrooper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATrooper::Move(const FPosition& Position) const
{
	return;
}

void ATrooper::Attack(const FPosition& Position) const
{
	return;
}


FVector ATrooper::GetVectorFromDirection(EDirectionEnum Direction) const
{
	FVector FinalPosition = GetActorLocation();

	switch (Direction)
	{
		case EDirectionEnum::DE_Forward:
			FinalPosition.X += 100.0f;
			break;

		case EDirectionEnum::DE_Backward:
			FinalPosition.X -= 100.0f;
			break;

		case EDirectionEnum::DE_Right:
			FinalPosition.Y += 100.0f;
			break;

		case EDirectionEnum::DE_Left:
			FinalPosition.Y -= 100.0f;
			break;
	}

	return FinalPosition;
}
