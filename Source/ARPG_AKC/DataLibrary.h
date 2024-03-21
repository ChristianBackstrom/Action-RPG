#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataLibrary.generated.h"

UENUM()
enum class EItemRarity : uint8
{
	Common = 100 UMETA(DisplayName = "Common") ,
	Uncommon = 75 UMETA(DisplayName = "Uncommon") ,
	Rare = 20 UMETA(DisplayName = "Rare") ,
	Unique = 2 UMETA(DisplayName = "Unique") 
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

	EItemRarity GenerateRandomRarity() const
	{
		const float TotalWeight = static_cast<int8>(EItemRarity::Unique) + 
							  static_cast<int8>(EItemRarity::Rare) + 
							  static_cast<int8>(EItemRarity::Uncommon) + 
							  static_cast<int8>(EItemRarity::Common);
		
		const float RandomWeight = FMath::FRandRange(1, TotalWeight);

		UE_LOG(LogTemp, Warning, TEXT("RandomWeight: %f"), RandomWeight);
		
		if (RandomWeight <= static_cast<int8>(EItemRarity::Unique))
		{
			return EItemRarity::Unique;
		}
		if (RandomWeight <= static_cast<int8>(EItemRarity::Unique) + static_cast<int8>(EItemRarity::Rare))
		{
			return EItemRarity::Rare;
		}
		if (RandomWeight <= static_cast<int8>(EItemRarity::Unique) +
							static_cast<int8>(EItemRarity::Rare) +
							static_cast<int8>(EItemRarity::Uncommon))
		{
			return EItemRarity::Uncommon;
		}
		
		return EItemRarity::Common;
	}
};

UCLASS()
class UDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
