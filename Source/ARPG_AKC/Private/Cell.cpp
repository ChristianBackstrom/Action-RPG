// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "Tile.h"

void UCell::Initialize(const FVector2D InGridPosition, const TArray<TSubclassOf<ATile>>& AllTiles)
{
	GridPosition = InGridPosition;
	SetPossibleTiles(AllTiles);
}

bool UCell::Collapse()
{
	if (bIsCollapsed) return false;
	if (PossibleTiles.Num() == 0) return false;

	for (TSubclassOf<ATile> Tile : PossibleTiles)
	{
		if (!Tile)
			UE_LOG(LogTemp, Warning, TEXT("No tile to spawn for cell at"));
	}

	int32 TotalWeight = 0.0f;
	for (int32 Weight : TileWeights)
	{
		TotalWeight += Weight;
	}

	const int32 RandomWeight = FMath::RandRange(0, TotalWeight);
	int32 CurrentWeight = 0;

	int32 SelectedIndex = 0;
	for (; SelectedIndex < TileWeights.Num(); ++SelectedIndex)
	{
		CurrentWeight += TileWeights[SelectedIndex];
		if (CurrentWeight >= RandomWeight) break;
	}
	
	if (SelectedIndex >= 0 && SelectedIndex < PossibleTiles.Num())
	{
		SetCollapsedTile(PossibleTiles[SelectedIndex]);
		return true;
	}
	return false;
}

void UCell::SetCollapsedTile(TSubclassOf<ATile> Tile)
{
	if (!Tile)
		UE_LOG(LogTemp, Warning, TEXT("Failed to select a valid tile for collapsing."));
	CollapsedTile = Tile;
	PossibleTiles.Empty();
	TileWeights.Empty();
	bIsCollapsed = true;
}

void UCell::SetPossibleTiles(const TArray<TSubclassOf<ATile>>& InPossibleTiles)
{
	for (TSubclassOf<ATile> Tile : InPossibleTiles)
	{
		if (!Tile)
			UE_LOG(LogTemp, Warning, TEXT("No tile to spawn for cell at"));
	}
	PossibleTiles = InPossibleTiles;
	TileWeights.Empty();
	for (const TSubclassOf<ATile>& TileClass : PossibleTiles)
	{
		float Weight = TileClass.GetDefaultObject()->BaseWeight;
		TileWeights.Add(Weight);
	}
	Entropy = PossibleTiles.Num();
}

