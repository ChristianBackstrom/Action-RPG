#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

UENUM()
enum class EItemRarity : uint8
{
	Common UMETA(DisplayName = "Common") ,
	Uncommon UMETA(DisplayName = "Uncommon") ,
	Rare UMETA(DisplayName = "Rare") ,
	Unique UMETA(DisplayName = "Unique") 
};

UENUM()
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"), 
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Accessory UMETA(DisplayName = "Accessory"),
	Consumable UMETA(DisplayName = "Consumable")
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

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType == EItemType::Weapon"))
	float DamageValue;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType == EItemType::Armor"))
	float ArmorValue;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType == EItemType::Consumable"))
	float ConsumableValue;

	EItemType ItemType;
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsStackable", ClampMin = 0, ClampMax = 500))
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
