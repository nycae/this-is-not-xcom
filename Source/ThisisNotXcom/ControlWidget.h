// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamLeader.h"
#include "Blueprint/UserWidget.h"
#include "ControlWidget.generated.h"

/**
 * 
 */
UCLASS()
class THISISNOTXCOM_API UControlWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	ATeamLeader* Team;

};
