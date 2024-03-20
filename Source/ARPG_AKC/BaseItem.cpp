// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

#include "ItemDataAsset.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Collider");
	SphereComponent->SetupAttachment(MeshComponent);
	
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(ItemType == EItemType::None) return;

	if(ItemDataAsset)
	{
		ItemGenericInfo = ItemDataAsset->ItemGenericInfo;
		
		MeshComponent->SetStaticMesh(ItemGenericInfo.Mesh);
	}
}