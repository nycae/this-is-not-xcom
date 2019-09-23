// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamLeader.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GridPathfinder.h"


ATeamLeader::ATeamLeader()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsInLookMode = false;
	bIsMenuBeeingDisplayed = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Capera Component"));
	Camera->SetupAttachment(RootComponent);

}

void ATeamLeader::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 15.0f;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -15.0f;

}

void ATeamLeader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		if (!PlayerController->bShowMouseCursor)
		{
			FVector2D Size;
			GEngine->GameViewport->GetViewportSize(Size);
			PlayerController->SetMouseLocation(Size.X / 2, Size.Y / 2);
		}
	}

}

void ATeamLeader::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
	
	/* Delegados para los controles */
	// Ejes (Joysticks)
	
	InputComponent->BindAxis("Turn", this, &ATeamLeader::RotateRight);
	InputComponent->BindAxis("LookUp", this, &ATeamLeader::RotateUp);
	InputComponent->BindAxis("MoveForward", this, &ATeamLeader::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATeamLeader::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ATeamLeader::MoveUp);

	// Botones
	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ATeamLeader::OnClick);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ATeamLeader::HideCursor);
	InputComponent->BindAction("RightMouse", IE_Released, this, &ATeamLeader::DisplayCursor);
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &ATeamLeader::FinishTurn);
	//InputComponent->BindAction("ResetTurn", IE_Pressed, this, &ATeamLeader::FinishTurn);
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
	if (!bIsInLookMode)
	{
		return;
	}

	FRotator NewRotation = AActor::GetActorRotation();
	NewRotation.Yaw += Value;
	AActor::SetActorRotation(NewRotation);

}

void ATeamLeader::RotateUp(float Value)
{
	if (!bIsInLookMode)
	{
		return;
	}

	FRotator NewRotation = AActor::GetActorRotation();
	NewRotation.Pitch += Value;
	AActor::SetActorRotation(NewRotation);

}

void ATeamLeader::HideCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
	}

	bIsInLookMode = true;

}

void ATeamLeader::DisplayCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
	}

	bIsInLookMode = false;

}

void ATeamLeader::CancelSelection()
{
	if (ObjectiveTile.Get() != nullptr)
	{
		ObjectiveTile->ToggleObjective();
		ObjectiveTile = nullptr;
	}

	if (SelectedTile.Get() != nullptr)
	{
		SelectedTile->ToggleSelected();
		SelectedTile = nullptr;
	}
}

void ATeamLeader::AttendTile(ATile* Tile)
{
	if (!Tile->Ocupant || Tile->Ocupant->Team != this)
	{
		if (ObjectiveTile.IsValid())
		{
			ObjectiveTile->ToggleObjective();
		}

		ObjectiveTile = Tile;
		ObjectiveTile->ToggleObjective();
	}
	else
	{
		if (SelectedTile.IsValid())
		{
			SelectedTile->ToggleSelected();
		}

		SelectedTile = Tile;
		SelectedTile->ToggleSelected();
	}
}

void ATeamLeader::AttendUnit(AUnit* Unit)
{
	return AttendTile(Unit->Location);
}

void ATeamLeader::AttendHitResult(AActor* Result)
{
	if (Cast<ATile>(Result))
	{
		return AttendTile(Cast<ATile>(Result));
	}

	if (Cast<AUnit>(Result))
	{
		return AttendUnit(Cast<AUnit>(Result));
	}
}

void ATeamLeader::OnClick()
{
	if (bIsInLookMode)
	{
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		if (HitResult.GetActor() != nullptr)
		{
			AttendHitResult(HitResult.GetActor());
		}
		else
		{
			SelectedTile = nullptr;
			ObjectiveTile = nullptr;
		}
	}
}

void ATeamLeader::OnEndOfAction()
{
	SelectedTile->ToggleSelected();
	SelectedTile = nullptr;

	ObjectiveTile->ToggleObjective();
	SelectedTile = nullptr;
}

/*
AUnit* ATeamLeader::SpawnUnit(EUnitTypeEnum UnitClass)
{
	if (!SelectedTile.IsValid())
	{
		return nullptr;
	}

	if (!UnitSpawnCount.Contains(UnitClass))
	{
		UnitSpawnCount.Add(UnitClass, 0);
	}

	if (UnitSpawnCount[UnitClass] >= UnitManager->UnitCount[UnitClass])
	{
		return nullptr;
	}
	
	FVector Location = SelectedTile->GetActorLocation();
	AUnit* Unit = (AUnit*) GetWorld()->SpawnActor(UnitManager->UnitClasses[UnitClass], &Location);

	Unit->Team = this;
	Unit->Location = SelectedTile.Get();

	UnitSpawnCount[UnitClass]++;

	return Unit;
}
*/

void ATeamLeader::SwitchCamera(ACameraActor* NewCamera)
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(NewCamera, 1.0f);

}

void ATeamLeader::FinishTurn()
{
	TurnEnd.Broadcast(this);
}

void ATeamLeader::GrantTurn()
{
	for (auto& Unit : Army)
	{
		Unit->ReplenishEnergy();
	}

	BeginTurn.Broadcast(this);
}

int32 ATeamLeader::HealthOfUnits() const
{
	int32 Health = 0;

	for (const AUnit* Unit : Army)
	{
		Health += Unit->Health;
	}

	return Health;
}