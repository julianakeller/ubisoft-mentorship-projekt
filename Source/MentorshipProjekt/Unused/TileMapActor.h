// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMapActor.generated.h"

USTRUCT(BlueprintType)
struct FTileCoord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 X = 0;

	UPROPERTY(EditAnywhere)
	int32 Y = 0;

	FTileCoord() {}

	FTileCoord(int32 InX, int32 InY)
	{
		X = InX;
		Y = InY;
	}

	bool operator==(const FTileCoord& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Ground
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETileType TileType = ETileType::Ground;

	UPROPERTY(EditAnywhere)
	bool bWalkable = true;
};

USTRUCT(BlueprintType)
struct FTileTeleportLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FTileCoord FromTile;

	UPROPERTY(EditAnywhere)
	ATileMapActor* TargetTileMap = nullptr;

	UPROPERTY(EditAnywhere)
	FTileCoord TargetTile;
};

FORCEINLINE uint32 GetTypeHash(const FTileCoord& Coord) {
	return HashCombine(::GetTypeHash(Coord.X), ::GetTypeHash(Coord.Y));
}

UCLASS()
class MENTORSHIPPROJEKT_API ATileMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileMapActor();

protected:
	// Called when the actor is moved into a level or moved in the level or when a UPROPERTY values is changed or the level is loaded. In game, called when the actor is spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TileMap")
	TSoftObjectPtr<UWorld> AssociatedLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TileMap")
	FName MapID;
	
	UPROPERTY(EditAnywhere, Category = "Tile Map")
	int32 Width = 10;

	UPROPERTY(EditAnywhere, Category = "Tile Map")
	int32 Height = 10;

	UPROPERTY(EditAnywhere, Category = "Tile Map")
	float TileSize = 100.f;

	UPROPERTY(EditAnywhere, Category = "Tile Map")
	bool bShowDebugGrid = false;

	UPROPERTY(VisibleAnywhere, Category = "Tile Map")
	TMap<FTileCoord, FTileData> Tiles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="TileMap")
	FTileCoord StartingTile = FTileCoord(0,0);

	UPROPERTY(EditAnywhere, Category="Teleport")
	TArray<FTileTeleportLink> TeleportLinks;
	
	FVector TileToWorld(const FTileCoord& Coord) const;
	bool IsTileWalkable(const FTileCoord& Coord) const;

	bool GetTeleportAtTile(const FTileCoord& Tile, FTileTeleportLink& OutLink) const;
	
private:
	void DrawDebugGrid();
};
