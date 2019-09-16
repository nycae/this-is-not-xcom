// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EDirectionEnum : uint8
{
	DE_Forward		UMETA(DisplayName = "Forward"),
	DE_Backward		UMETA(DisplayName = "Down"),
	DE_Left			UMETA(DisplayName = "Left"),
	DE_Right		UMETA(DisplayName = "Right"),
	DE_FwdLeft		UMETA(DisplayName = "Forward Left"),
	DE_FwdRight		UMETA(DisplayName = "Forward Right"),
	DE_BwdLeft		UMETA(DisplayName = "Backward Left"),
	DE_BwdRight		UMETA(DisplayName = "Backward Right"),
	DE_MAX			UMETA(DisplayName = "MAX")
};

