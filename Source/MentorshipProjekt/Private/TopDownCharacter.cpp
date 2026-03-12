// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "MentorshipProjekt/InteractionSystem/InteractablesInterface.h"
#include "MentorshipProjekt/Menu/MenuManager.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* OLD
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	//GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	*/
	
	//Create Spring Arm -> keeps camera at a fixed distance
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	SpringArm->TargetArmLength = 800.0f;
	//SpringArm->SetRelativeRotation(FRotator(-60.f, 0.0f, 0.0f));
	
	// If enabled, prevents clipping
	SpringArm->bDoCollisionTest = true;
	
	//Create Camera:
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	//Camera->bUsePawnControlRotation = true;
	
	//Attach to  Spring Arm Socket
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TargetArmLength = SpringArm->TargetArmLength;
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(IMC_Player, 0);
			}
		}
	}
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Interpolate zooming:
	if (!FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetArmLength, 1.f))
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, ZoomInterpolationSpeed);
	}
}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Move);
		EIC->BindAction(IA_LookAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Look);
		EIC->BindAction(IA_ZoomAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Zoom);
		EIC->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &ATopDownCharacter::Interact);
		EIC->BindAction(IA_InteractSecondary, ETriggerEvent::Triggered, this, &ATopDownCharacter::InteractSecondary);
		//EIC->BindAction(IA_CloseMenu, ETriggerEvent::Triggered, this, &ATopDownCharacter::CloseMenu);
	}
}

void ATopDownCharacter::Move(const FInputActionValue& Input)
{
	FVector2D MovementVector = Input.Get<FVector2D>();

	if (MovementVector.IsNearlyZero())
	{
		return;
	}
	
	if (!IsValid(Controller) || Controller == nullptr)
	{
		return;
	}
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	FVector DesiredDirection = ForwardDirection * MovementVector.Y + RightDirection   * MovementVector.X;

	DesiredDirection.Normalize();

	AddMovementInput(DesiredDirection);

	// Rotate character toward movement direction
	if (!DesiredDirection.IsNearlyZero())
	{
		FRotator TargetRotation = DesiredDirection.Rotation();
		SetActorRotation(FRotator(0.f, TargetRotation.Yaw, 0.f));
	}
	
	//Add Movement based on Input:
	//AddMovementInput(ForwardDirection, MovementVector.Y);
	//AddMovementInput(RightDirection, MovementVector.X)
}

void ATopDownCharacter::Look(const FInputActionValue& Input)
{
	FVector2D MovementVector = Input.Get<FVector2D>();
	
	if (MovementVector.IsNearlyZero())
	{
		return;
	}
	
	if (!IsValid(Controller) || Controller == nullptr)
	{
		return;
	}
	
	AddControllerYawInput(MovementVector.X);
	AddControllerPitchInput(MovementVector.Y);
}

void ATopDownCharacter::Zoom(const FInputActionValue& Input)
{
	float Value = Input.Get<float>();
	
	UE_LOG(LogTemp, Warning, TEXT("Zoom: %f"), Value);
	
	if (FMath::IsNearlyZero(Value)){
		return;
	}
	
	TargetArmLength -= Value * ZoomSpeed * GetWorld()->GetDeltaSeconds();
    
	TargetArmLength = FMath::Clamp(TargetArmLength, MinZoom, MaxZoom);
}

void ATopDownCharacter::Interact()
{
	TArray<AActor*> OverlappingActors;
	
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->GetClass()->ImplementsInterface(UInteractablesInterface::StaticClass()))
		{
			if (IInteractablesInterface* InteractablesInterface = Cast<IInteractablesInterface>(OverlappingActor))
			{
				InteractablesInterface->OnInteracted(this);
				break;
			}
		}
	}
}

void ATopDownCharacter::InteractSecondary()
{
	TArray<AActor*> OverlappingActors;
	
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->GetClass()->ImplementsInterface(UInteractablesInterface::StaticClass()))
		{
			if (IInteractablesInterface* InteractablesInterface = Cast<IInteractablesInterface>(OverlappingActor))
			{
				InteractablesInterface->OnInteractedSecondary(this);
				break;
			}
		}
	}
}

void ATopDownCharacter::CloseMenu()
{
	if (UMenuManager* MenuManager = GetWorld()->GetSubsystem<UMenuManager>())
	{
		// Pass nullptr to close any open menu
		MenuManager->ToggleMenu(nullptr, nullptr);
	}
}
