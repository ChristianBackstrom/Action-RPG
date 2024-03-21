// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	StaticMesh->SetMobility(EComponentMobility::Movable);
	StaticMesh->SetupAttachment(RootComponent);
}

bool ATile::CanNeighbor(TSubclassOf<ATile> OtherTile, const FVector2D& DirectionToNeighbor) const
{
	if (DirectionToNeighbor.Equals(FVector2D(1, 0))) // North
	{
		return AllowedNorthNeighbors.Contains(OtherTile);
	}
	if (DirectionToNeighbor.Equals(FVector2D(0, 1))) // East
	{
		return AllowedEastNeighbors.Contains(OtherTile);
	}
	if (DirectionToNeighbor.Equals(FVector2D(-1, 0))) // South
	{
		return AllowedSouthNeighbors.Contains(OtherTile);
	}
	if (DirectionToNeighbor.Equals(FVector2D(0, -1))) // West
	{
		return AllowedWestNeighbors.Contains(OtherTile);
	}

	return false;
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	//GenerateMesh();
}

float ATile::GetPerlinNoiseHeightAtLocation(float X, float Y)
{
	// Scale factors adjust the frequency of noise. Adjust these based on your needs.
	float ScaleX = 0.0001263435f;
	float ScaleY = 0.0001263435f;

	// Getting noise value for given coordinates. This is a placeholder for your Perlin noise function.
	float NoiseValue = FMath::PerlinNoise2D(FVector2D(X * ScaleX, Y * ScaleY));

	// Map the noise value to a desired height range.
	float MinHeight = -100.0f; // Minimum height of your terrain
	float MaxHeight = 500.0f;  // Maximum height of your terrain


	return FMath::Lerp(MinHeight, MaxHeight, NoiseValue);
}

void ATile::GenerateMesh() const
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    // Grid size and tile size
    const int32 GridSize = 10; // Number of vertices on one side of the grid
    const float TileSize = 100.0f; // Size of each tile

    // Generate grid vertices with heights based on Perlin noise
    for(int32 Y = 0; Y < GridSize; ++Y)
    {
        for(int32 X = 0; X < GridSize; ++X)
        {
        	const float WorldX = GetActorLocation().X + X * TileSize;
        	const float WorldY = GetActorLocation().Y + Y * TileSize;
        	const float Z = GetPerlinNoiseHeightAtLocation(WorldX, WorldY);
        	Vertices.Add(FVector(X * TileSize, Y * TileSize, Z));

            // Generate UVs based on X and Y to map textures properly
            UVs.Add(FVector2D(X / static_cast<float>(GridSize - 1), Y / static_cast<float>(GridSize - 1)));
        }
    }

    // Generate triangles for the grid
    for(int32 Y = 0; Y < GridSize - 1; ++Y)
    {
        for(int32 X = 0; X < GridSize - 1; ++X)
        {
            int32 BottomLeft = Y * GridSize + X;
            int32 BottomRight = BottomLeft + 1;
            int32 TopLeft = BottomLeft + GridSize;
            int32 TopRight = TopLeft + 1;

            Triangles.Add(BottomLeft);
            Triangles.Add(TopLeft);
            Triangles.Add(TopRight);

            Triangles.Add(BottomLeft);
            Triangles.Add(TopRight);
            Triangles.Add(BottomRight);
        }
    }

    // Auto-generate normals, tangents, and vertex colors or define your own.
    Normals.Init(FVector(0, 0, 1), Vertices.Num()); // Simplified normal calculation
    Tangents.Init(FProcMeshTangent(1, 0, 0), Vertices.Num()); // Simplified tangent calculation
    Colors.Init(FLinearColor::White, Vertices.Num()); // Vertex colors (optional).

    // Create the mesh.
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);

    // Enable collision if needed.
    ProceduralMesh->ContainsPhysicsTriMeshData(true);
}

