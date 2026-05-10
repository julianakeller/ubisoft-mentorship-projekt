// Fill out your copyright notice in the Description page of Project Settings.
#include "TileMapActor.h"

#include "DrawDebugHelpers.h"


// Sets default values
ATileMapActor::ATileMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ATileMapActor::OnConstruction(const FTransform& Transform)
{
	Tiles.Empty();
	
	for(int32 X = 0; X < Width; X++)
	{
		for(int32 Y = 0; Y < Height; Y++)
		{
			FTileCoord Coord(X, Y);
			FTileData TileData;
			TileData.TileType = ETileType::Ground;
			TileData.bWalkable = true;

			Tiles.Add(Coord, TileData);
		}
	}
	
	DrawDebugGrid();
}

#if WITH_EDITOR
void ATileMapActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	DrawDebugGrid();
}
#endif

void ATileMapActor::DrawDebugGrid()
{
#if WITH_EDITOR
	if (!GetWorld())
		return;

	FlushPersistentDebugLines(GetWorld());

	if (!bShowDebugGrid)
		return;

	for (int32 X = 0; X < Width; X++)
	{
		for (int32 Y = 0; Y < Height; Y++)
		{
			FTileCoord Coord(X, Y);
			FVector Center = TileToWorld(Coord);

			DrawDebugBox(
				GetWorld(),
				Center,
				FVector(TileSize * 0.5f, TileSize * 0.5f, 5.f),
				FColor::Green,
				true,      // Persistent
				-1.f,      // Infinite duration
				0,
				1.f
				//GetWorld returns a pointer to current UWorld, contains all actors, the level and the virewport
				//DrawDebugBox takes box extent, which is half-size of the box dimensions
				//false -> doesn't persist over frames, redraws every OnConstruction
				//-1.f -> duration, negative means it stays indefinitely (until next OnConstruction)
				//2.f -> line thickness
			);
		}
	}
#endif
}

FVector ATileMapActor::TileToWorld(const FTileCoord& Coord) const
{
	FVector localPos((Coord.X + 0.5f) * TileSize, (Coord.Y + 0.5f) * TileSize, 0.f);
	return GetActorLocation() + localPos; //takes only position into account, not rotation or scaling of the actor
}

bool ATileMapActor::GetTeleportAtTile(const FTileCoord& Tile, FTileTeleportLink& OutLink) const
{
	for (const FTileTeleportLink& Link : TeleportLinks)
	{
		if (Link.FromTile == Tile)
		{
			OutLink = Link;
			return true;
		}
	}
	return false;
}

bool ATileMapActor::IsTileWalkable(const FTileCoord& Coord) const
{
	if (const FTileData* Tile = Tiles.Find(Coord))
	{
		return (*Tile).bWalkable;
	}
	return false;
}


