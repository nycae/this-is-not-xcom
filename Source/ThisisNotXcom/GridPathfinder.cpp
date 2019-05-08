// Fill out your copyright notice in the Description page of Project Settings.

#include "GridPathfinder.h"
#include "Engine.h"

constexpr EDirectionEnum EveryDirection[] =
	{ EDirectionEnum::DE_Backward, EDirectionEnum::DE_Forward, EDirectionEnum::DE_Left, EDirectionEnum::DE_Right };

GridPathfinder::GridPathfinder()
{
}

TArray<EDirectionEnum> GridPathfinder::GetPath
	(const FPosition& Origin, const FPosition& Destiny, uint8 MaxDepth, const AGrid* Grid)
{
	if (!Grid->isEmpty(Destiny) || MaxDepth == 0)
	{
		return TArray<EDirectionEnum>();
	}

	TArray<TArray<EDirectionEnum>> PossiblePaths;

	/* First exploration */
	for (const auto& Direction : EveryDirection)
	{
		TArray<EDirectionEnum> NewPath{ Direction };

		FPosition PathPosition(GetWhereAmI(Origin, NewPath));

		if (PathPosition == Destiny)
		{
			return NewPath;
		}

		if (Grid->isEmpty(PathPosition))
		{
			PossiblePaths.Add(NewPath);
		}
	}

	/* Aquí es donde ocurre la explosión combinatoria */
	for (uint8 CurrentDepth = 1; CurrentDepth < MaxDepth; CurrentDepth++)
	{
		uint32 StartingPathsIndex = PossiblePaths.Num(); // Needed because Possible Paths will grow

		for (uint32 PathIndex = 0; PathIndex < StartingPathsIndex; PathIndex++)
		{
			for (const auto& Direction : EveryDirection)
			{
				TArray<EDirectionEnum> NewPath(PossiblePaths[PathIndex]);
				NewPath.Add(Direction);

				FPosition PathPosition(GetWhereAmI(Origin, NewPath));

				if (PathPosition == Destiny)
				{
					return NewPath;
				}

				if (Grid->isEmpty(PathPosition))
				{
					PossiblePaths.Add(NewPath);
				}
			}
		}
	}

	return TArray<EDirectionEnum>();
	/*
	TArray<EDirectionEnum> Traceback;

	TMap<FPosition, bool> Visited;
	TQueue<FPosition> Fringe;
	Fringe.Enqueue(Origin);

	while (!Fringe.IsEmpty())
	{
		FPosition CurrentState = Fringe.Pop();
		
		if (Grid->isEmpty(CurrentState) && Visited[CurrentState] == false)
		{
			FPosition NewState(Origin);

			NewState.Row++;
			Fringe.Enqueue(NewState);

			NewState.Row -= 2;
			Fringe.Enqueue(NewState);

			NewState.Row++; NewState.Column++;
			Fringe.Enqueue(NewState);

			NewState.Column -= 2;
			Fringe.Enqueue(NewState);
		}
	}

	return Traceback;
	*/
}

FPosition GridPathfinder::GetWhereAmI
	(FPosition Origin, const TArray<EDirectionEnum>& Try)
{
	for (const auto& Direction : Try)
	{
		switch (Direction)
		{
			case EDirectionEnum::DE_Forward:
				Origin.Row++;
				break;

			case EDirectionEnum::DE_Backward:
				Origin.Row--;
				break;

			case EDirectionEnum::DE_Right:
				Origin.Column++;
				break;

			case EDirectionEnum::DE_Left:
				Origin.Column--;
				break;

			default:
				break;
		}
	}

	return Origin;

}

void GridPathfinder::PrintPath(const TArray<EDirectionEnum>& Path)
{
	for (const auto& Direction : Path)
	{
		switch (Direction)
		{
		case EDirectionEnum::DE_Forward:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Arriba");
			break;

		case EDirectionEnum::DE_Backward:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Abajo");
			break;

		case EDirectionEnum::DE_Right:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Derecha");
			break;

		case EDirectionEnum::DE_Left:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Izquierda");
			break;

		default:
			break;
		}
	}
}

TArray<FPosition> GridPathfinder::ExpandPath(const FPosition& Origin)
{
	TArray<FPosition> NewStates;

	
	return NewStates;
}