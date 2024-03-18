#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

UENUM()
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Unique
};

UENUM()
enum class EItemType : uint8
{
	Weapon,
	Armor,
	Accessory,
	Consumable
};

USTRUCT()
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText ItemName;

	UPROPERTY(EditAnywhere)
	FText ItemDescription;

	UPROPERTY(EditAnywhere)
	FText InteractionText;
};

USTRUCT()
struct FItemStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float DamageValue;

	UPROPERTY(EditAnywhere)
	float ArmorValue;

	UPROPERTY(EditAnywhere)
	float ConsumableValue;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float Weight;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "!bIsStackable", ClampMin = 0, ClampMax = 500))
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere)
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FItemGenericInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EItemType ItemType;

	UPROPERTY(EditAnywhere)
	EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere)
	FItemTextData ItemTextData;

	UPROPERTY(EditAnywhere)
	FItemStats ItemStats;
	
	UPROPERTY(EditAnywhere)
	FItemNumericData ItemNumericData;

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;
};

UCLASS()
class UDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
