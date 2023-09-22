// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPointerComponent.h"
#include "EnhancedInputComponent.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "VRCharacter.h"


// Sets default values for this component's properties
UWidgetPointerComponent::UWidgetPointerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWidgetPointerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();
}	


// Called every frame
void UWidgetPointerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWidgetPointerComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
 	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UWidgetPointerComponent::PressButton); 
 	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UWidgetPointerComponent::ReleaseButton); 
}

void UWidgetPointerComponent::PressButton()
{
	player->rightWidgetPointer->PressPointerKey(EKeys::LeftMouseButton);
}

void UWidgetPointerComponent::ReleaseButton()
{
	player->rightWidgetPointer->ReleasePointerKey(EKeys::LeftMouseButton);
}

