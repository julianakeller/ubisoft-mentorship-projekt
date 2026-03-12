#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GameTimeWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), GameTimeWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
	if (LogWindowWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), LogWindowWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
	if (InventoryOverviewWidgetClass)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), InventoryOverviewWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryOverviewWidget not assigned."))
	}
}
