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

	UCell* FirstCell = GetCell(FVector2D(GridSize / 2, GridSize / 2));
	FirstCell->SetCollapsedTile(AllTiles[0]);
	Propagate(FirstCell);
	SpawnTileForCell(FirstCell);
	
	int IterationsLeft = GridSize * GridSize - 1;
	while(IterationsLeft > 0)
	{
		UCell* LowestEntropyCell = nullptr;
		for (UCell* Cell : LowerEntropyCells)
		{
			if (!LowestEntropyCell)
			{
				LowestEntropyCell = Cell;
				continue;
			}
			if (LowestEntropyCell->GetEntropy() < Cell->GetEntropy())
			{
				LowestEntropyCell = Cell;
			}
		}
		LowestEntropyCell->Collapse();
		SpawnTileForCell(LowestEntropyCell);
		Propagate(LowestEntropyCell);
		LowerEntropyCells.Remove(LowestEntropyCell);
		IterationsLeft--;
		UE_LOG(LogTemp, Warning, TEXT("%d Tiles remaining"), IterationsLeft);
		UE_LOG(LogTemp, Warning, TEXT("%d LowerEntrpyCells"), LowerEntropyCells.Num());
	}
}

void AWorldGenerationGameModeBase::Propagate(UCell* CollapsedCell)
{
	TQueue<UCell*> CellsToUpdate;
	TSet<UCell*> EnqueuedCells;

	CellsToUpdate.Enqueue(CollapsedCell);
	EnqueuedCells.Add(CollapsedCell);

	while (!CellsToUpdate.IsEmpty())
	{
		UCell* CurrentCell;
		CellsToUpdate.Dequeue(CurrentCell);

		TArray<UCell*> Neighbors = GetNeighbors(CurrentCell->GridPosition);
		for (UCell* Neighbor : Neighbors)
		{
			if (Neighbor->bIsCollapsed || EnqueuedCells.Contains(Neighbor)) continue;

			if (ApplyDirectionalConstraints(Neighbor, CurrentCell))
			{
				LowerEntropyCells.Add(Neighbor);
			}
			{
				CellsToUpdate.Enqueue(Neighbor);
				EnqueuedCells.Add(Neighbor);
			}
		}
	}
}

bool AWorldGenerationGameModeBase::ApplyDirectionalConstraints(UCell* CellToUpdate, const UCell* CollapsedNeighborCell)
{
	if (!CellToUpdate || CellToUpdate->bIsCollapsed) return false;
	if (!CollapsedNeighborCell || !CollapsedNeighborCell->bIsCollapsed) return false;
	
	TArray<TSubclassOf<ATile>> NewPossibleTiles;
	const FVector2D Direction = GetDirection(CellToUpdate->GridPosition, CollapsedNeighborCell->GridPosition);
	
	if (Direction == FVector2D(0, 0)) return false;
	
	for (TSubclassOf<ATile> PossibleTile : CellToUpdate->GetPossibleTiles())
	{
		if (PossibleTile.GetDefaultObject()->CanNeighbor(CollapsedNeighborCell->GetCollapsedTile(), Direction))
		{
			NewPossibleTiles.Add(PossibleTile);
		}
	}
	
	const bool bChanged = NewPossibleTiles.Num() != CellToUpdate->GetEntropy();
	if (bChanged)
	{
		CellToUpdate->SetPossibleTiles(NewPossibleTiles);
		LowerEntropyCells.Add(CellToUpdate);
	}
	
	return bChanged;
}

TArray<UCell*> AWorldGenerationGameModeBase::GetNeighbors(FVector2D GridPosition) const
{
	TArray<UCell*> Neighbors;
	
	TArray Directions =
	{
		FVector2D(0, -1), // North
		FVector2D(1, 0),  // East
		FVector2D(0, 1),  // South
		FVector2D(-1, 0), // West
	};

	for (const FVector2D& Direction : Directions)
	{
		FVector2D NeighborPosition = GridPosition + Direction;
		UCell* NeighborCell = GetCell(NeighborPosition);
        
		if (!NeighborCell) continue;

		Neighbors.Add(NeighborCell);
	}
	return Neighbors;
}

UCell* AWorldGenerationGameModeBase::GetCell(const FVector2D& GridPosition) const
{
	if (GridPosition.X < 0 || GridPosition.X >= GridSize || GridPosition.Y < 0 || GridPosition.Y >= GridSize) return nullptr;

	return Grid[GridPosition.X][GridPosition.Y];
}

FVector2D AWorldGenerationGameModeBase::GetDirection(const FVector2D From, const FVector2D To)
{
	const FVector2D Direction = To - From;
	
	if (FMath::Abs(Direction.X) + FMath::Abs(Direction.Y) != 1) return FVector2D(0, 0);

	return Direction;
}

void AWorldGenerationGameModeBase::SpawnTileForCell(const UCell* Cell) const
{
	if (!Cell || !Cell->bIsCollapsed) return;

	const FVector Location = CalculateWorldLocation(Cell->GridPosition);
	const FRotator Rotation = FRotator(0);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ATile>(Cell->GetCollapsedTile(), Location, Rotation, SpawnParameters);
}

FVector AWorldGenerationGameModeBase::CalculateWorldLocation(FVector2D GridPosition)
{
	return FVector(GridPosition.X * 100.0f, GridPosition.Y * 100.0f, 0.0f);
}


