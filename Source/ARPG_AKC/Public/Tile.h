// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Tile.generated.h"


UCLASS()
class ARPG_AKC_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
	int32 BaseWeight = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
	bool CanNeighbor(TSubclassOf<ATile> OtherTile, const FVector2D& DirectionToNeighbor) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	TArray<TSubclassOf<ATile>> AllowedAllDirectionsNeighbors;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	TArray<TSubclassOf<ATile>> AllowedNorthNeighbors;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	TArray<TSubclassOf<ATile>> AllowedEastNeighbors;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	TArray<TSubclassOf<ATile>> AllowedSouthNeighbors;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	TArray<TSubclassOf<ATile>> AllowedWestNeighbors;

	static float GetPerlinNoiseHeightAtLocation(float X, float Y);
	void GenerateMesh() const;

	
};
