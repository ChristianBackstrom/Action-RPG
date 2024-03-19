// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WorldGenerationGameModeBase.generated.h"

class UCell;
class ATile;
/**
 * 
 */
UCLASS()
class ARPG_AKC_API AWorldGenerationGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TSubclassOf<ATile>> AllTiles;
	
	void GenerateWorld();
	void UpdateConstraintsForNeighborCells(const FVector2D& CollapsedCellPosition);
	void RefineValidTilesForCell(UCell* CellToUpdate, const FVector2D& CollapsedNeighborPosition);
	
	UCell* GetCell(const FVector2D& GridPosition);
	void SpawnTileForCell(const UCell* Cell, FVector2D GridPosition) const;
	static FVector CalculateWorldLocation(FVector2D GridPosition);
	
	int32 GridSize = 100;

private:
	TArray<TArray<UCell*>> Grid;
	TArray<UCell*> CellsWithCollapsedNeighbors;
};
