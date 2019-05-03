// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "TeamLeader.generated.h"

class AThisisNotXcomGameMode;

constexpr char* DefaultPlayerOneName = "PlayerOne";

constexpr char* DefaultPlayerTwoName = "PlayerTwo";

UCLASS()
class THISISNOTXCOM_API ATeamLeader : public APawn
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	ATeamLeader();

	ATeamLeader(const FString& InitName);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveRight(float Value);

	void MoveUp(float Value);

	void MoveForward(float Value);

	void RotateUp(float Value);

	void RotateRight(float Value);

private:

	UCameraComponent* Camera;

	AThisisNotXcomGameMode* GameMode;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Name")
		FString PlayerName;

};
