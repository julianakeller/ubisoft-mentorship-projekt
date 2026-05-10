// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMenuWidget.h"

void UBaseMenuWidget::Open()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	
	//Trigger BP logic, e.g. animations, sound
	OnMenuOpened();
}

void UBaseMenuWidget::Close()
{
	RemoveFromParent();
	OnMenuClosed();
}

void UBaseMenuWidget::InitializeMenu(AInteractableBase* Source)
{
	SourceInteractable = Source;
}

void UBaseMenuWidget::OnMenuOpened()
{
}

void UBaseMenuWidget::OnMenuClosed()
{
}
