// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorIndicatorActor.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

// Sets default values
AFloorIndicatorActor::AFloorIndicatorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	floorIndicator = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Floor Effect"));
	floorIndicator->SetupAttachment(rootComp);
}

// Called when the game starts or when spawned
void AFloorIndicatorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloorIndicatorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

