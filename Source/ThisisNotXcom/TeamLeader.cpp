// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamLeader.h"
#include "Camera/CameraComponent.h"
#include "ThisisNotXcomGameMode.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"


ATeamLeader::ATeamLeader()
	: ATeamLeader("Default")
{

}

ATeamLeader::ATeamLeader(const FString& InitName)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Capera Component"));

	Camera->SetupAttachment(RootComponent);

}

void ATeamLeader::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AThisisNotXcomGameMode>(GetWorld()->GetAuthGameMode());

	verify(GameMode != nullptr);

}

void ATeamLeader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATeamLeader::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);

	//Delegados para los controles
	InputComponent->BindAxis("Turn", this, &ATeamLeader::RotateRight);
	InputComponent->BindAxis("LookUp", this, &ATeamLeader::RotateUp);
	InputComponent->BindAxis("MoveForward", this, &ATeamLeader::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATeamLeader::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ATeamLeader::MoveUp);

}

void ATeamLeader::MoveForward(float Value)
{
	AActor::SetActorLocation(AActor::GetActorLocation() + (AActor::GetActorForwardVector() * Value * 10.0f));

}

void ATeamLeader::MoveRight(float Value)
{
	AActor::SetActorLocation(AActor::GetActorLocation() + (AActor::GetActorRightVector() * Value * 10.0f));

}

void ATeamLeader::MoveUp(float Value)
{
	FVector NewLocation = AActor::GetActorLocation();
	NewLocation.Z += Value * 10.0f;
	AActor::SetActorLocation(NewLocation);

}

void ATeamLeader::RotateRight(float Value)
{
	FRotator NewRotation = AActor::GetActorRotation();
	NewRotation.Yaw += Value;
	AActor::SetActorRotation(NewRotation);

}

void ATeamLeader::RotateUp(float Value)
{
	FRotator NewRotation = AActor::GetActorRotation();
	NewRotation.Pitch += Value;
	AActor::SetActorRotation(NewRotation);

}