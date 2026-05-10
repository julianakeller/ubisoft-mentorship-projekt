// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NPCCharacter.h"
#include "../AI/BrainComponents/CustomerBrainComponent.h"
#include "GameFramework/Character.h"
#include "CustomerCharacter.generated.h"

class UCustomerInformationDisplayWidget;
struct FCustomerInstanceData;

UCLASS()
class MENTORSHIPPROJEKT_API ACustomerCharacter : public ANPCCharacter
{
	GENERATED_BODY()

public:
	ACustomerCharacter();

	void Initialize(FGuid InCustomerInstanceId);
	
	virtual void BeginPlay() override;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	//UCustomerBrainComponent* CustomerBrainComponent; //Visit behavior

	virtual void Tick(float DeltaTime) override;

	FCustomerInstanceData GetInstanceData() const;
	FCustomerInstanceData* GetInstanceDataRef();

	UPROPERTY()
	FGuid CustomerId;
	
	UPROPERTY(VisibleAnywhere)
	int32 SuccessfulPurchases = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* CustomerInformationDisplayWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCustomerInformationDisplayWidget* CustomerInformationDisplay;
};
