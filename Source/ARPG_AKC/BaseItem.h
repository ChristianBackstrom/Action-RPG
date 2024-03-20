// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataLibrary.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class UWidgetComponent;
class USphereComponent;
class UItemDataAsset;

UCLASS()
class ARPG_AKC_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Component")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "Item Info")
	UItemDataAsset* ItemDataAsset;
	
	UPROPERTY(VisibleAnywhere, Category = "Item Info")
	EItemType ItemType = EItemType::None; // Probably will change this later on

	UPROPERTY(VisibleAnywhere, Category = "Item Info")
	FItemGenericInfo ItemGenericInfo;
	
#ifdef UE_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
