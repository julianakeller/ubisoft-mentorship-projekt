// Fill out your copyright notice in the Description page of Project Settings.


#include "FamilyMemberNameLabelWidget.h"

#include "Components/TextBlock.h"

void UFamilyMemberNameLabelWidget::SetNameText(const FText& NewText)
{
	if (FamilyMemberName)
	{
		FamilyMemberName->SetText(NewText);
	}
}
