// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamLeader.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "ThisisNotXcomGameMode.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GridPathfinder.h"
#include "Grid.h"


ATeamLeader::ATeamLeader()
	: ATeamLeader("Default")
{
	bIsTurnMoveDone = false;
	bIsTurnAttackDone = false;
	bIsInLookMode = false;

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

	TArray<AActor*> Grids;

	GameMode = Cast<AThisisNotXcomGameMode>(GetWorld()->GetAuthGameMode());
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Grids);

	verify(GameMode != nullptr);
	verify(Grids.Num() > 0);

	CombatGrid = Cast<AGrid>(Grids[0]);

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

/* Hellish contextual button */
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
			ATile* Tile = Cast<ATile>(HitResult.GetActor());

			if (Tile)
			{
				if (SelectedTile == nullptr)
				{
					if (Tile->Ocupant)
					{
						if (this == Tile->Ocupant->Team)
						{
							SelectedTile = Tile;
							Tile->ToggleSelected();
						}
						else
						{
							GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "Esa unidad no es mia");
						}
					}
					else
					{
						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "No detecto ocupante");
					}
				} /* Si no hay unidad seleccionada selecciónala */
				else // Se ha seleccionado unidad
				{
					if (Tile->Ocupant)
					{
						if (Tile->Ocupant->Team == this)
						{
							SelectedTile->ToggleSelected();
							Tile->ToggleSelected();
							SelectedTile = Tile;
						} // Si la nueva unidad es aliada cambia
						else
						{
							GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "Esa unidad no es mia");
							// TODO: Attack
						} // Si la unidad es enemiga atácala
					}
					else
					{
						FPosition SelectedUnitPosition = CombatGrid->GetPosition(SelectedTile.Get());
						FPosition ObjectivePosition = CombatGrid->GetPosition(Tile);

						TArray<EDirectionEnum> Path = CombatGrid->GetPath
							(SelectedUnitPosition, ObjectivePosition, SelectedTile->Ocupant->MaxMoveDepth);

						if (Path.Num() > 0)
						{
							// TODO: Move
							SelectedTile->Ocupant->PlayMovementAnimation(Path);
							CombatGrid->SwapPositions(SelectedUnitPosition, ObjectivePosition);

							SelectedTile->ToggleSelected();
							SelectedTile = nullptr;
						}
						else
						{
							GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "No se puede mover");
						}
					} // Si no hay unidad mueve la seleccionada
				}
			}
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("You hit: %s"), *HitResult.Actor->GetName()));
		}
	}
}

void ATeamLeader::OnNewTurn()
{
	bIsTurnMoveDone = false;
	bIsTurnAttackDone = false;

}