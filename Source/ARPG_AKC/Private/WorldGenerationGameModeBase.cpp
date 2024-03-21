// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Public\WorldGenerationGameModeBase.h"

#include "Cell.h"
#include "Tile.h"

AWorldGenerationGameModeBase::AWorldGenerationGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldGenerationGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	CreateGrid();
	UCell* FirstCell = GetCell(FVector2D(GridSize / 2, GridSize / 2));
	FirstCell->SetCollapsedTile(AllTiles[1]);
	Propagate(FirstCell);
	SpawnTileForCell(FirstCell);
	IterationsLeft = GridSize * GridSize - 1;
	bIsGenerating = true;
}

void AWorldGenerationGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Timer++;
	if (Timer != 20) return;
	Timer = 0;
	//while (bIsGenerating)
	//{
	//	if (!ProcessGenerationStep())
	//	{
	//		bIsGenerating = false;
	//	}
	//}
	
	for (int32 i = 0; i < 1; i++)
	{
		if (bIsGenerating)
		{
			if (!ProcessGenerationStep())
			{
				bIsGenerating = false;
			}
		}
	}
}

void AWorldGenerationGameModeBase::CreateGrid()
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
}

bool AWorldGenerationGameModeBase::ProcessGenerationStep()
{
	TArray<UCell*> MinimumEntropyCells = GetMinimumEntropyCells();
	int32 RandNum = FMath::RandRange(0, MinimumEntropyCells.Num() - 1);
	UCell* LowestEntropyCell = MinimumEntropyCells[RandNum];
	LowestEntropyCell->Collapse();
	SpawnTileForCell(LowestEntropyCell);
	Propagate(LowestEntropyCell);
	CellsAdjacentToCollapsedCells.Remove(LowestEntropyCell);
	IterationsLeft--;
	UE_LOG(LogTemp, Warning, TEXT("%d Tiles remaining"), IterationsLeft);

	return IterationsLeft > 0;
}

TArray<UCell*> AWorldGenerationGameModeBase::GetMinimumEntropyCells() const
{
	TArray<UCell*> MinimumEntropyCells;
	int32 LowestEntropyValue = AllTiles.Num();
	
	for (const UCell* Cell : CellsAdjacentToCollapsedCells)
	{
		if (LowestEntropyValue > Cell->GetEntropy())
		{
			LowestEntropyValue = Cell->GetEntropy();
		}
	}

	for (UCell* Cell : CellsAdjacentToCollapsedCells)
	{
		if (LowestEntropyValue == Cell->GetEntropy())
		{
			MinimumEntropyCells.Add(Cell);
		}
	}

	return MinimumEntropyCells;
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

			CellsAdjacentToCollapsedCells.Add(Neighbor);
			if (ApplyDirectionalConstraints(Neighbor, CurrentCell))
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
		if (!PossibleTile) continue;
		if (PossibleTile.GetDefaultObject()->CanNeighbor(CollapsedNeighborCell->GetCollapsedTile(), Direction))
		{
			NewPossibleTiles.Add(PossibleTile);
		}
	}
	
	const bool bChanged = NewPossibleTiles.Num() != CellToUpdate->GetEntropy();
	if (bChanged)
	{
		CellToUpdate->SetPossibleTiles(NewPossibleTiles);
	}
	
	return bChanged;
}

TArray<UCell*> AWorldGenerationGameModeBase::GetNeighbors(FVector2D GridPosition) const
{
	TArray<UCell*> Neighbors;
	
	TArray Directions =
	{
		FVector2D(1, 0), // North
		FVector2D(0, 1),  // East
		FVector2D(-1, 0),  // South
		FVector2D(0, -1), // West
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
	if (Cell->GetCollapsedTile() == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("No tile to spawn for cell at %s"), *Cell->GridPosition.ToString());


	const FVector Location = CalculateWorldLocation(Cell->GridPosition);
	const FRotator Rotation = FRotator(0);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const ATile* Tile = GetWorld()->SpawnActor<ATile>(Cell->GetCollapsedTile(), FVector(0, 0, 0), Rotation, SpawnParameters);

	Tile->StaticMesh->SetWorldLocation(Location + Tile->StaticMesh->GetRelativeLocation());
}

FVector AWorldGenerationGameModeBase::CalculateWorldLocation(FVector2D GridPosition)
{
	return FVector(GridPosition.X * 1200.0f, GridPosition.Y * 1200.0f, 0.0f);
}
