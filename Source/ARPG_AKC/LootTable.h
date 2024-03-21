#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "LootTable.generated.h"

USTRUCT(BlueprintType)
struct FLootTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UItemDataAsset* ItemDataAsset;

	UPROPERTY(EditAnywhere)
	float Weight;
};
