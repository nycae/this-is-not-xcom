// Fill out your copyright notice in the Description page of Project Settings.

#include "Trooper.h"
#include "ConstructorHelpers.h"

// Sets default values
ATrooper::ATrooper()
	: Super(), bIsDead(false)
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

FVector ATrooper::GetVectorFromDirection(EDirectionEnum Direction, int32 TileSize) const
{
	FVector FinalPosition = GetActorLocation();

	switch (Direction)
	{
		case EDirectionEnum::DE_Forward:
			FinalPosition.X += TileSize;
			break;

		case EDirectionEnum::DE_Backward:
			FinalPosition.X -= TileSize;
			break;

		case EDirectionEnum::DE_Right:
			FinalPosition.Y += TileSize;
			break;

		case EDirectionEnum::DE_Left:
			FinalPosition.Y -= TileSize;
			break;
	}

	return FinalPosition;
}

FRotator ATrooper::GetRotationByDirection(EDirectionEnum Direction) const
{
	switch (Direction)
	{
	case EDirectionEnum::DE_Forward:
		return FRotator(0.0f, 0.0f, 0.0f);
		break;

	case EDirectionEnum::DE_Backward:
		return FRotator(0.0f, 180.0f, 0.0f);
		break;

	case EDirectionEnum::DE_Right:
		return FRotator(0.0f, 90.0f, 0.0f);
		break;

	case EDirectionEnum::DE_Left:
		return FRotator(0.0f, -90.0f, 0.0f);
		break;
	}

	return FRotator();
}

float ATrooper::GetHealthPercentage() const
{
	return ((float)Health) / MaxHealth;
}

int ATrooper::OnAttack(ATrooper* Attacker)
{
	Health -= Attacker->AttackDamage;

	if (Health <= 0)
	{
		bIsDead = true;
		SetActorHiddenInGame(true);
	}

	return Health;
}