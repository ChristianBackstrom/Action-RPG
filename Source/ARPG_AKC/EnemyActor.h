// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

class ABaseItem;

UCLASS()
class ARPG_AKC_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AEnemyActor();
	
	virtual void Tick(float DeltaTime) override;
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UDataTable* LootTable;

	UPROPERTY(EditAnywhere)
	int NumItemsToSpawn = 10;
	
	UPROPERTY(EditAnywhere)
	bool bGenerate = false;
	
	void GenerateLoot() const;
	
	void SpawnLootAroundEnemy(const UItemDataAsset* ItemData, float Radius) const;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseItem> BaseItemActor;
	

};
