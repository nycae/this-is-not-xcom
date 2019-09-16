// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Tile.h"
#include "Unit.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "TeamLeader.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndTurn, ATeamLeader*, Team);


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

	void OnClick();

	void HideCursor();

	void DisplayCursor();

	void DisplayInterface();

	void HideInterface();

private:

	UCameraComponent* Camera;

	UUserWidget* Interface = nullptr;

	bool bIsInLookMode;

	bool bIsMenuBeeingDisplayed;

private:

	void OnEndOfAction();

	void AttendHitResult(AActor* Result);

	void AttendTile(ATile* Tile);

	void AttendUnit(AUnit* Unit);

public:

	int32 Score = 0;

	UFUNCTION(BlueprintCallable)
		void FinishTurn();

	UFUNCTION(BlueprintCallable)
		void GrantTurn();

	UFUNCTION(BlueprintCallable)
		void CancelSelection();

public:

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
		FOnEndTurn OnEndTurn;

};
