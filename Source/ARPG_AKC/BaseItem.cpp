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
	
}

void ABaseItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//if(bGenerate)
		//GenerateLoot();
	
}
