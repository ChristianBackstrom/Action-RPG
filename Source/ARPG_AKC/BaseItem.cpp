// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "LootTable.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Collider");
	SphereComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

	GenerateLoot();
}

void ABaseItem::GenerateLoot() const
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
							UE_LOG(LogTemp, Warning, TEXT("Selected Item: %s"), *SelectedItem->GetName());
							ABaseItem* Item = GetWorld()->SpawnActor<ABaseItem>(BaseItemActor, GetActorLocation(), GetActorRotation());
							Item->MeshComponent->SetStaticMesh(SelectedItem->ItemGenericInfo.Mesh);
						}
						break;
					}
				}
			}
		}
	}
}

void ABaseItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//if(bGenerate)
		//GenerateLoot();
	
}
