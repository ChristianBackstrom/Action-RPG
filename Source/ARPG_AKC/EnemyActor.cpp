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

void AEnemyActor::GenerateLoot() const
{
	static const FString ContextString(TEXT("Loot Table Context"));
	
	if (LootTable)
	{
		TArray<FName> RowNames = LootTable->GetRowNames();
	 	
		if (RowNames.Num() > 0)
		{
			float TotalWeight = 0;
			for (const FName RowName : RowNames)
			{
				if (const FLootTable* LootRow = LootTable->FindRow<FLootTable>(RowName, ContextString))
				{
					TotalWeight += LootRow->Weight;
				}
			}

			for (int i = 0; i < NumItemsToSpawn; ++i)
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
							UE_LOG(LogTemp, Warning, TEXT("Random weight: %f, Current weight: %f"), RandomWeight, CurrentWeight);
						
							if (const UItemDataAsset* SelectedItem = LootRow->ItemDataAsset)
							{
								SpawnLootAroundEnemy(SelectedItem, 100);
							}
							break;
						}
					}
				}
			}
		}
	}
	MeshComponent->SetVisibility(false);
}

void AEnemyActor::SpawnLootAroundEnemy(const UItemDataAsset* ItemData, float Radius) const
{
	FVector2D EnemyLocation2D = FVector2D(GetActorLocation().X, GetActorLocation().Y);
	FRotator EnemyRotation = GetActorRotation();
	
	FVector2D RandomPoint = FMath::RandPointInCircle(Radius);
	FVector SpawnLocation = FVector(RandomPoint.X + EnemyLocation2D.X, RandomPoint.Y + EnemyLocation2D.Y, GetActorLocation().Z);
        
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	ABaseItem* Item = GetWorld()->SpawnActor<ABaseItem>(BaseItemActor, SpawnLocation, EnemyRotation, SpawnParameters);
	Item->MeshComponent->SetStaticMesh(ItemData->ItemGenericInfo.Mesh);
	
}

