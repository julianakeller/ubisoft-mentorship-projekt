// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	Camera -> SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	(*Mesh).SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0; // Automatically possess this pawn with the first player controller
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> FoundMaps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileMapActor::StaticClass(), FoundMaps);

	for (AActor* Actor : FoundMaps)
	{
		ATileMapActor* Map = Cast<ATileMapActor>(Actor);
		if (Map && Map->MapID == DefaultMapID)
		{
			CurrentTileMap = Map;
			break;
		}
	}

	if (!CurrentTileMap)
	{
		UE_LOG(LogTemp, Error, TEXT("No TileMap found with ID %s"), *DefaultMapID.ToString());
		return;
	}
	
	InterpolationFactor = 1.f;
	CurrentTile = CurrentTileMap->StartingTile;
	TargetTile = CurrentTile;
	
	SetActorLocation(CurrentTileMap -> TileToWorld(CurrentTile));
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (Subsystem)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InterpolationFactor < 1.f)
	{
		InterpolationFactor += DeltaTime * MoveSpeed;
		InterpolationFactor = FMath::Min(InterpolationFactor, 1.f);

		FVector Start = CurrentTileMap -> TileToWorld(CurrentTile);
		FVector End = CurrentTileMap -> TileToWorld(TargetTile);

		FVector NewLocation = FMath::Lerp(Start, End, InterpolationFactor);
		SetActorLocation(NewLocation);

		if (InterpolationFactor >= 1.f)
		{
			FinishMove();
		}
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerPawn::Move
		);
	}
}

void APlayerPawn::Move(const FInputActionValue& Input)
{
	FVector2D MovementVector = Input.Get<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("X: %f Y: %f"), MovementVector.X, MovementVector.Y);
	
	int32 MovementX = 0;
	int32 MovementY = 0;
	
	if (MovementVector.X > 0.5f)
	{
		MovementX = 1;
	}
	else if (MovementVector.X < -0.5f)
	{
		MovementX = -1;
	}

	if (MovementVector.Y > 0.5f)
	{
		MovementY = 1;
	}
	else if (MovementVector.Y < -0.5f)
	{
		MovementY = -1;
	}

	if (MovementX == 0 && MovementY == 0)
	{
		return;
	}
	
	FTileCoord NextTile(CurrentTile.X + MovementX, CurrentTile.Y + MovementY);
	
	TryStartMove(NextTile);
}

void APlayerPawn::TryStartMove(const FTileCoord& NewTile)
{
	// If currently moving
	if (InterpolationFactor < 1.f)
	{
		//BufferedTile = NewTile;
		return;
	}
	
	if (!CurrentTileMap -> IsTileWalkable(NewTile))
		return;
	
	TargetTile = NewTile;
	InterpolationFactor = 0.f;
}

void APlayerPawn::FinishMove()
{
	CurrentTile = TargetTile;
	
	FTileTeleportLink TeleportLink;
	if (CurrentTileMap->GetTeleportAtTile(CurrentTile, TeleportLink))
	{
		TeleportToMap(TeleportLink);
	}
}

void APlayerPawn::TeleportToMap(const FTileTeleportLink& Link)
{
	if (!Link.TargetTileMap)
		return;
	
	//Unload old level?

	CurrentTileMap = Link.TargetTileMap;
	CurrentTile = Link.TargetTile;
	TargetTile = CurrentTile;

	InterpolationFactor = 1.f;

	SetActorLocation(CurrentTileMap->TileToWorld(CurrentTile));
}



