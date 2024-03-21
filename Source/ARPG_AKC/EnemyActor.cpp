// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "BaseItem.h"
#include "LootTable.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bGenerate) return;
	GenerateLoot();
	bGenerate = false;
	
}

void AEnemyActor::GenerateLoot()
{
	if (!LootTable) return;
	const FString ContextString(TEXT("Loot Table Context"));
	const TArray<FName> RowNames = LootTable->GetRowNames();
	if (RowNames.Num() == 0) return;

	float TotalWeight = 0;
	for (const FName RowName : RowNames)
	{
		if (const FLootTable* LootRow = LootTable->FindRow<FLootTable>(RowName, ContextString))
		{
			TotalWeight += LootRow->Weight;
		}
	}

	int TotalRarityWeight = NumItemsToSpawn; // Initialize with base number of items to spawn
    for (int i = 0; i < NumItemsToSpawn; i++)
    {
        const float RandomWeight = FMath::FRandRange(0, TotalWeight);
        float CurrentWeight = 0;
        for (const FName RowName : RowNames)
        {
            if (const FLootTable* LootRow = LootTable->FindRow<FLootTable>(RowName, ContextString))
            {
                CurrentWeight += LootRow->Weight;
                if (CurrentWeight > RandomWeight)
                {
                    if (const UItemDataAsset* SelectedItem = LootRow->ItemDataAsset)
                    {
                        SpawnLootAroundEnemy(SelectedItem, 300, TotalRarityWeight);
                        TotalRarityWeight--; // Decrease rarity weight after each spawn
                    }
                    break;
                }
            }
        }
    }    
	
	//Just to remove the enemy from the screen but not destroy (for debugging purposes)
	MeshComponent->SetVisibility(false);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEnemyActor::SpawnLootAroundEnemy(const UItemDataAsset* ItemData, float Radius, int RarityWeight) const
{
	const FVector2D EnemyLocation2D(GetActorLocation().X, GetActorLocation().Y);
	const FRotator EnemyRotation = GetActorRotation();
	const FVector2D RandomPoint = FMath::RandPointInCircle(Radius);
	const FVector SpawnLocation(RandomPoint.X + EnemyLocation2D.X, RandomPoint.Y + EnemyLocation2D.Y, GetActorLocation().Z);
	
	FItemGenericInfo NewItemInfo = ItemData->ItemGenericInfo;
	
	NewItemInfo.ItemRarity = NewItemInfo.GenerateRandomRarity();
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ABaseItem* Item = GetWorld()->SpawnActor<ABaseItem>(BaseItemActor, SpawnLocation, EnemyRotation, SpawnParameters);
	Item->MeshComponent->SetStaticMesh(ItemData->ItemGenericInfo.Mesh);
	
	Item->GenerationInfo = NewItemInfo;

	// Make rarity have an effect with num of items spawned WIP
	if(Item->GenerationInfo.ItemRarity == EItemRarity::Unique)
	{
		RarityWeight += 5;
	}
	else if(Item->GenerationInfo.ItemRarity == EItemRarity::Rare)
	{
		RarityWeight += 3;
	}
	else if(Item->GenerationInfo.ItemRarity == EItemRarity::Uncommon)
	{
		RarityWeight += 2;
	}
	else
	{
		RarityWeight++;
	}
}

