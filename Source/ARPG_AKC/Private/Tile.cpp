// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;
	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	RootComponent = GroundMesh;
}

bool ATile::CanNeighbor(TSubclassOf<ATile> OtherTile, const FVector2D& DirectionToNeighbor) const
{
	if (DirectionToNeighbor.Equals(FVector2D(0, -1))) // North
	{
		return AllowedNorthNeighbors.Contains(OtherTile);
	}
	if (DirectionToNeighbor.Equals(FVector2D(1, 0))) // East
	{
		return AllowedEastNeighbors.Contains(OtherTile);
	}
	if (DirectionToNeighbor.Equals(FVector2D(0, 1))) // South
	{
		return AllowedSouthNeighbors.Contains(OtherTile);
	}
	if (DirectionToNeighbor.Equals(FVector2D(-1, 0))) // West
	{
		return AllowedWestNeighbors.Contains(OtherTile);
	}

	return false;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<ATile> Tile : AllowedAllDirectionsNeighbors)
	{
		AllowedNorthNeighbors.Add(Tile);
		AllowedEastNeighbors.Add(Tile);
		AllowedSouthNeighbors.Add(Tile);
		AllowedWestNeighbors.Add(Tile);
	}
}

