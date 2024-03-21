// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataLibrary.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class ARPG_AKC_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

	UPROPERTY(EditAnywhere, Category = "Component")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	FItemGenericInfo GenerationInfo;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Component")
	USphereComponent* SphereComponent;

#ifdef UE_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
