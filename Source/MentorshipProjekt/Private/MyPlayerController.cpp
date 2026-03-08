#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GameTimeWidgetClass)
	{
		UUserWidget* Widget = CreateWidget(this, GameTimeWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}
