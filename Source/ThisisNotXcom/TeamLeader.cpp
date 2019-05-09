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
	bIsMenuBeeingDisplayed = false;

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
	/*
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/Blueprints/UserInterface/BP_InGameMenu"));

	if (WidgetClass.Class)
	{
		Menu = CreateWidget<UUserWidget>(UGameplayStatics::GetGameInstance(GetWorld()), WidgetClass.Class);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Yellow, "No se ha encontrado la clase");
	}
	*/
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
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &ATeamLeader::EndTurn);
	InputComponent->BindAction("ResetTurn", IE_Pressed, this, &ATeamLeader::OnNewTurn);
	InputComponent->BindAction("ForceEndgame", IE_Pressed, GameMode, &AThisisNotXcomGameMode::OnEndGame);

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
							if (!bIsTurnAttackDone)
							{
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "Esa unidad no es mia");
								if ( (Tile->Ocupant->OnAttack(SelectedTile->Ocupant)) < 0 )
								{
									CombatGrid->FreeCoordinate(CombatGrid->GetPosition(Tile));
									Score += 50;
								}
								OnEndAttack();
							} // TODO: Attack
						} // Si la unidad es enemiga atácala
					}
					else
					{
						if (!bIsTurnMoveDone)
						{
							FPosition SelectedUnitPosition = CombatGrid->GetPosition(SelectedTile.Get());
							FPosition ObjectivePosition = CombatGrid->GetPosition(Tile);

							TArray<EDirectionEnum> Path = CombatGrid->GetPath
							(SelectedUnitPosition, ObjectivePosition, SelectedTile->Ocupant->MaxMoveDepth);

							if (Path.Num() > 0)
							{
								SelectedTile->Ocupant->PlayMovementAnimation(Path);
								CombatGrid->SwapPositions(SelectedUnitPosition, ObjectivePosition);
								OnEndMove();
							}
							else
							{
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, "No se puede mover");
							}
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

void ATeamLeader::OnEndMove()
{
	SelectedTile->ToggleSelected();
	SelectedTile = nullptr;

	bIsTurnMoveDone = true;

	if (bIsTurnMoveDone && bIsTurnAttackDone)
	{
		Cast<AThisisNotXcomGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OnEndOfTurn();
	}
}

void ATeamLeader::OnEndAttack()
{
	SelectedTile->ToggleSelected();
	SelectedTile = nullptr;

	bIsTurnAttackDone = true;

	Score += 10;

	if (bIsTurnMoveDone && bIsTurnAttackDone)
	{
		Cast<AThisisNotXcomGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OnEndOfTurn();
	}
}

void ATeamLeader::EndTurn()
{
	Cast<AThisisNotXcomGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->OnEndOfTurn();
}

void ATeamLeader::KillMe(ATrooper* Trooper)
{
	Trooper->SetActorHiddenInGame(true);
}

void ATeamLeader::OnUnitKill()
{
	NumberOfUnits--;

	if (NumberOfUnits == 0)
	{
		GameMode->OnEndGame();
	}
}
