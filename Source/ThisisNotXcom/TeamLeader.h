// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Tile.h"
#include "Unit.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "TeamLeader.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnEnd, ATeamLeader*, Team);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnBegin, ATeamLeader*, Team);

UCLASS()
class THISISNOTXCOM_API ATeamLeader : public APawn
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	ATeamLeader();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveRight(float Value);

	void MoveUp(float Value);

	void MoveForward(float Value);

	void RotateUp(float Value);

	void RotateRight(float Value);

	void AttendTap(ETouchIndex::Type FingerIndex, FVector Location);

	void OnClick();

	void HideCursor();

	void DisplayCursor();

private:

	UCameraComponent* Camera;

	bool bIsInLookMode;

	bool bIsMenuBeeingDisplayed;

private:

	void OnEndOfAction();

	void AttendHitResult(AActor* Result);

	void AttendTile(ATile* Tile);

	void AttendUnit(AUnit* Unit);

public:

	UFUNCTION(BlueprintCallable)
		void FinishTurn();

	UFUNCTION(BlueprintCallable)
		void GrantTurn();

	UFUNCTION(BlueprintCallable)
		void CancelSelection();

	UFUNCTION(BlueprintCallable)
		void SwitchCamera(ACameraActor* NewCamera);

	UFUNCTION(BlueprintCallable)
		int32 HealthOfUnits() const;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scores")
		int32 Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map assets")
		AGrid* CombatGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		TArray<AUnit*> Army;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Atributes")
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TWeakObjectPtr<ATile> SelectedTile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
		TWeakObjectPtr<ATile> ObjectiveTile = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FOnTurnEnd TurnEnd;

	UPROPERTY(BlueprintAssignable, Category = "Event dispacher")
		FOnTurnBegin BeginTurn;

};
