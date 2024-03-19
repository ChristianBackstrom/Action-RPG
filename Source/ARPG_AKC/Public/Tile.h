// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class ARPG_AKC_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	TArray<TSubclassOf<ATile>> AllowedNeighbors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	float BaseWeight = 1.0f;

	bool CanNeighbor(TSubclassOf<ATile> OtherTile) const { return AllowedNeighbors.Contains(OtherTile); }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GroundMesh;
	
	TArray<AActor*> ChildActors;
};
