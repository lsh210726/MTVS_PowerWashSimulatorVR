// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include "VRCharacter.h"


// Sets default values for this component's properties
UEffectComponent::UEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player=GetOwner<AVRCharacter>();	
	gun=player->waterGun;


}


// Called every frame
void UEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UEffectComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UEffectComponent::StartEffect);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UEffectComponent::EndEffect);

}

void UEffectComponent::SpawnShootEffect()
{
	//SpawnActor
	/*AYourBlueprintClass* SpawnedActor = GetWorld()->SpawnActor<AYourBlueprintClass>(BlueprintClass, SpawnLocation, SpawnRotation);*/
}

void UEffectComponent::StartEffect()
{

}

void UEffectComponent::EndEffect()
{

}

