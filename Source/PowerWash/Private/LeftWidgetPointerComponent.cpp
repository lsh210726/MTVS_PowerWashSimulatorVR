// Fill out your copyright notice in the Description page of Project Settings.


#include "LeftWidgetPointerComponent.h"
#include "EnhancedInputComponent.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "VRCharacter.h"

// Sets default values for this component's properties
ULeftWidgetPointerComponent::ULeftWidgetPointerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULeftWidgetPointerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();
}


// Called every frame
void ULeftWidgetPointerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULeftWidgetPointerComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Started, this, &ULeftWidgetPointerComponent::PressButton);//왼쪽 그립 잡으면
	enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Completed, this, &ULeftWidgetPointerComponent::ReleaseButton);

}

void ULeftWidgetPointerComponent::PressButton()
{
	player->LeftWidgetPointer->PressPointerKey(EKeys::LeftMouseButton);
}

void ULeftWidgetPointerComponent::ReleaseButton()
{
	player->LeftWidgetPointer->ReleasePointerKey(EKeys::LeftMouseButton);
	UE_LOG(LogTemp, Warning, TEXT("left btn relese"));
}