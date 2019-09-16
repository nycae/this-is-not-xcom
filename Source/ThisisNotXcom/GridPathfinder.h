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
	
	/* Pseudo BFS */
	static TArray<EDirectionEnum> GetPath
		(const FPosition& Origin, const FPosition& Destiny, int32 MaxDepth, const AGrid* Grid);

	static TArray<EDirectionEnum> GetPath
		(const ATile* Origin, const ATile* Destiny, int32 MaxDepth, const AGrid* Grid);

	static void PrintPath(const TArray<EDirectionEnum>& Path);

private:

	static FPosition GetWhereAmI
		(FPosition Origin, const TArray<EDirectionEnum>& Try);

};
