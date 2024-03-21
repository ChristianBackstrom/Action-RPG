// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell.generated.h"

class ATile;
/**
 * 
 */
UCLASS()
class ARPG_AKC_API UCell : public UObject
{
	GENERATED_BODY()

public:
	bool bIsCollapsed = false;
	FVector2D GridPosition;

	void Initialize(FVector2D InGridPosition, const TArray<TSubclassOf<ATile>>& AllTiles);
	bool Collapse();
	
	TArray<int32> GetTileWeights() const { return TileWeights; }
	TArray<TSubclassOf<ATile>> GetPossibleTiles() const { return PossibleTiles; }
	int32 GetEntropy() const { return Entropy; }
	TSubclassOf<ATile> GetCollapsedTile() const { return CollapsedTile; }

	void SetPossibleTiles(const TArray<TSubclassOf<ATile>>& InPossibleTiles);
	void SetCollapsedTile(TSubclassOf<ATile> Tile);
	
private:
	TSubclassOf<ATile> CollapsedTile;
	TArray<int32> TileWeights;
	TArray<TSubclassOf<ATile>> PossibleTiles;
	int32 Entropy;
};
