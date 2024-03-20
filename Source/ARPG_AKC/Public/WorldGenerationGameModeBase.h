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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 GridSize = 100;
	
	void GenerateWorld();
	
	void Propagate(UCell* CollapsedCell);
	bool ApplyDirectionalConstraints(UCell* CellToUpdate, const UCell* CollapsedNeighborCell);

	TArray<UCell*> GetNeighbors(FVector2D GridPosition) const;
	UCell* GetCell(const FVector2D& GridPosition) const;
	static FVector2D GetDirection(const FVector2D From, const FVector2D To);
	void SpawnTileForCell(const UCell* Cell) const;
	static FVector CalculateWorldLocation(FVector2D GridPosition);

private:
	TArray<TArray<UCell*>> Grid;
	TSet<UCell*> LowerEntropyCells;
};
