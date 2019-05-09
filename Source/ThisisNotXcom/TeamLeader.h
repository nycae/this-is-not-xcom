// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tile.h"
#include "Camera/CameraComponent.h"
#include "TeamLeader.generated.h"

class AThisisNotXcomGameMode;

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

	void OnClick();

	void HideCursor();

	void DisplayCursor();

private:

	UCameraComponent* Camera;

	AThisisNotXcomGameMode* GameMode;

	TWeakObjectPtr<ATile> SelectedTile = nullptr;

	TWeakObjectPtr<AGrid> CombatGrid;

	bool bIsInLookMode;

	bool bIsTurnMoveDone;

	bool bIsTurnAttackDone;

	bool bIsMenuBeeingDisplayed;

	void OnEndMove();

	void OnEndAttack();

	void OnUnitKill();

public:

	void KillMe(ATrooper* Trooper);

	UFUNCTION(BlueprintCallable)
		void OnNewTurn();

	UFUNCTION(BlueprintCallable)
		void EndTurn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		int32 Score = 0;

	UPROPERTY(BlueprintReadWrite)
		uint8 NumberOfUnits = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
		FString PlayerName;

};
