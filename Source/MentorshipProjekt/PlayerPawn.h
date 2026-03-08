// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Pawn.h"
#include "TileMapActor.h"
#include "PlayerPawn.generated.h"

UCLASS()
class MENTORSHIPPROJEKT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;

public:	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TileMap")
	FName DefaultMapID;
	
	UPROPERTY(VisibleAnywhere)
	ATileMapActor* CurrentTileMap;
	
	UPROPERTY(VisibleAnywhere)
	FTileCoord CurrentTile;
	
	UPROPERTY(VisibleAnywhere)
	FTileCoord TargetTile;
	
	float InterpolationFactor = 1.f;

	void Move(const FInputActionValue& Value);
	
	void TryStartMove(const FTileCoord& NewTile);
	void FinishMove();
	void TeleportToMap(const FTileTeleportLink& Link);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
