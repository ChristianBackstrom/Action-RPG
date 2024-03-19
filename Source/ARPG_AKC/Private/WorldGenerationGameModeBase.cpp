// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\WorldGenerationGameModeBase.h"

#include "Cell.h"
#include "Tile.h"

void AWorldGenerationGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateWorld();
}

void AWorldGenerationGameModeBase::GenerateWorld()
{
	Grid.SetNum(GridSize);
	for (int32 i = 0; i < GridSize; i++)
	{
		Grid[i].SetNum(GridSize);
		for (int32 j = 0; j < GridSize; j++)
		{
			UCell* NewCell = NewObject<UCell>();
			NewCell->Initialize(FVector2D(i, j), AllTiles);
			Grid[i][j] = NewCell;
		}
	}

	Grid[GridSize / 2][GridSize / 2]->Collapse();
	UpdateConstraintsForNeighborCells(FVector2D(GridSize/2, GridSize/2));
	SpawnTileForCell(Grid[GridSize / 2][GridSize / 2], Grid[GridSize / 2][GridSize / 2]->GridPosition);
	
	
	int IterationsLeft = GridSize * GridSize - 1;
	while(IterationsLeft > 0)
	{
		UCell* LowestEntropyCell = CellsWithCollapsedNeighbors[0];
		for (UCell* Cell : CellsWithCollapsedNeighbors)
		{
			if (LowestEntropyCell->GetEntropy() < Cell->GetEntropy())
			{
				LowestEntropyCell = Cell;
			}
		}
		LowestEntropyCell->Collapse();
		SpawnTileForCell(LowestEntropyCell, LowestEntropyCell->GridPosition);
		UpdateConstraintsForNeighborCells(LowestEntropyCell->GridPosition);
		CellsWithCollapsedNeighbors.Remove(LowestEntropyCell);
		IterationsLeft--;
	}
}

void AWorldGenerationGameModeBase::UpdateConstraintsForNeighborCells(const FVector2D& CollapsedCellPosition)
{
	TArray Offsets =
	{
		FVector2D(0, -1), // North
		FVector2D(1, -1), // North East
		FVector2D(1, 0),  // East
		FVector2D(1, 1),  // South East
		FVector2D(0, 1),  // South
		FVector2D(-1, 1), // South West
		FVector2D(-1, 0), // West
		FVector2D(-1, -1) // North West
	};

	for (const FVector2D& Offset : Offsets)
	{
		FVector2D NeighborPosition = CollapsedCellPosition + Offset;
		UCell* NeighborCell = GetCell(NeighborPosition);
        
		if (NeighborCell && !NeighborCell->bIsCollapsed)
		{
			RefineValidTilesForCell(NeighborCell, CollapsedCellPosition);
			if (!CellsWithCollapsedNeighbors.Contains(NeighborCell))
			{
				CellsWithCollapsedNeighbors.Add(NeighborCell);
			}
		}
	}
}


void AWorldGenerationGameModeBase::RefineValidTilesForCell(UCell* CellToUpdate, const FVector2D& CollapsedNeighborPosition)
{
	const UCell* CollapsedNeighborCell = GetCell(CollapsedNeighborPosition);
	if (!CollapsedNeighborCell || !CollapsedNeighborCell->bIsCollapsed) return;

	const TSubclassOf<ATile> CollapsedTile = CollapsedNeighborCell->GetCollapsedTile();

	TArray<TSubclassOf<ATile>> NewPossibleTiles;
	for (TSubclassOf<ATile> PossibleTile : CellToUpdate->GetPossibleTiles())
	{
		if (CollapsedTile.GetDefaultObject()->CanNeighbor(PossibleTile))
		{
			NewPossibleTiles.Add(PossibleTile);
		}
	}

	CellToUpdate->SetPossibleTiles(NewPossibleTiles);
}

UCell* AWorldGenerationGameModeBase::GetCell(const FVector2D& GridPosition)
{
	if (GridPosition.X < 0 || GridPosition.X >= GridSize || GridPosition.Y < 0 || GridPosition.Y >= GridSize) return nullptr;

	return Grid[GridPosition.X][GridPosition.Y];
}

void AWorldGenerationGameModeBase::SpawnTileForCell(const UCell* Cell, FVector2D GridPosition) const
{
	if (!Cell || !Cell->bIsCollapsed) return;

	const FVector Location = CalculateWorldLocation(GridPosition); // Implement this based on your grid-to-world mapping
	const FRotator Rotation = FRotator(0); // Adjust as needed

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<ATile>(Cell->GetCollapsedTile(), Location, Rotation, SpawnParameters);
}

FVector AWorldGenerationGameModeBase::CalculateWorldLocation(FVector2D GridPosition)
{
	return FVector(GridPosition.X * 100.0f, GridPosition.Y * 100.0f, 0.0f);
}


