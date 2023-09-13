// Fill out your copyright notice in the Description page of Project Settings.


#include "HandAnimComponent.h"
#include "VRCharacter.h"
#include "HandAnimInstance.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>


// Sets default values for this component's properties
UHandAnimComponent::UHandAnimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHandAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();
}


// Called every frame
void UHandAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHandAnimComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[5], ETriggerEvent::Triggered, this, &UHandAnimComponent::OnPressTrigger_Right);
	enhancedInputComponent->BindAction(inputActions[5], ETriggerEvent::Completed, this, &UHandAnimComponent::OnPressTrigger_Right);
	enhancedInputComponent->BindAction(inputActions[6], ETriggerEvent::Triggered, this, &UHandAnimComponent::OnPressTrigger_Left);
	enhancedInputComponent->BindAction(inputActions[6], ETriggerEvent::Completed, this, &UHandAnimComponent::OnPressTrigger_Left);
}

void UHandAnimComponent::OnPressTrigger_Right(const  FInputActionValue& value)
{
	float inputValue = value.Get<float>();
	if(player->rightHandAnim !=nullptr) player->rightHandAnim->PoseAlphaGrasp = inputValue;
}

void UHandAnimComponent::OnPressTrigger_Left(const  FInputActionValue& value)
{
	float inputValue = value.Get<float>();
	if (player->leftHandAnim != nullptr) 
	{
		player->leftHandAnim->PoseAlphaIndexCurl = inputValue;
		player->leftHandAnim->PoseAlphaGrasp = inputValue;
	}
}
