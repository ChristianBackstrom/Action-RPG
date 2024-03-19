// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

#include "Tile.h"

void UCell::Initialize(const FVector2D InGridPosition, const TArray<TSubclassOf<ATile>>& AllTiles)
{
	GridPosition = InGridPosition;
	SetPossibleTiles(AllTiles);
}

void UCell::Collapse()
{
	if (bIsCollapsed) return;

	float TotalWeight = 0.0f;
	for (float Weight : TileWeights)
	{
		TotalWeight += Weight;
	}

	float RandomWeight = FMath::FRandRange(0.0f, TotalWeight);
	float CurrentWeight = 0.0f;

	int32 SelectedIndex = 0;
	for (; SelectedIndex < TileWeights.Num(); ++SelectedIndex)
	{
		CurrentWeight += TileWeights[SelectedIndex];
		if (CurrentWeight >= RandomWeight) break;
	}

	SetCollapsedTile(PossibleTiles[SelectedIndex]);
}

void UCell::SetCollapsedTile(TSubclassOf<ATile> Tile)
{
	CollapsedTile = Tile;
	PossibleTiles.Empty();
	TileWeights.Empty();
	bIsCollapsed = true;
}

void UCell::SetPossibleTiles(const TArray<TSubclassOf<ATile>>& InPossibleTiles)
{
	PossibleTiles = InPossibleTiles;
	TileWeights.Empty();
	for (const TSubclassOf<ATile>& TileClass : PossibleTiles)
	{
		float Weight = TileClass.GetDefaultObject()->BaseWeight;
		TileWeights.Add(Weight);
	}
	Entropy = PossibleTiles.Num();
}

