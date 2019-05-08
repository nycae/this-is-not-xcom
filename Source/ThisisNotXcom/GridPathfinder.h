// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Grid.h"
#include "Direction.h"

/**
 * 
 */
class THISISNOTXCOM_API GridPathfinder
{
public:
	
	GridPathfinder();
	
	/* Pseudo BFS */
	static TArray<EDirectionEnum> GetPath
		(const FPosition& Origin, const FPosition& Destiny, uint8 MaxDepth, const AGrid* Grid);

	static void PrintPath(const TArray<EDirectionEnum>& Path);

private:

	static FPosition GetWhereAmI
		(FPosition Origin, const TArray<EDirectionEnum>& Try);

	static TArray<FPosition> ExpandPath(const FPosition& Origin);

};
