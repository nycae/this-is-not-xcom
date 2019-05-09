// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "TroopManager.h"
#include "Grid.h"
#include "TeamManager.h"
#include "ThisisNotXComGameMode.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class THISISNOTXCOM_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	void FindGrid();

	void FindTroopManager();

	void FindGameMode();

	void SpawnTeamLeaders();

	void PreGenerateGround();

	void SetPlayerTroops();

	void FixGround();

	void GenerateGround();

	void SpawnPlayerTroops();

	TWeakObjectPtr<AThisisNotXcomGameMode> GameMode;

	TWeakObjectPtr<AGrid> CombatGrid;

	TWeakObjectPtr<ATroopManager> TroopManager;

	ATile* SpawnFloor(const FVector& Position);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		int32 MapMaxX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		int32 MapMaxY;
		*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		int32 SideSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		int32 TileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Size")
		TArray<FPosition> AditionalTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map Characteristics")
		float BlockChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structures")
		TSubclassOf<AActor> FloorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Structures")
		UInstancedStaticMeshComponent* ObstructingStructure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team Management")
		ATeamManager* TeamManager;

};
