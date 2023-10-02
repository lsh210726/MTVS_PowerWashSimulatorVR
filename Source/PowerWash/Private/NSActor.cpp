// Fill out your copyright notice in the Description page of Project Settings.


#include "NSActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"

// Sets default values
ANSActor::ANSActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//나이아가라
	/*NSSpriteSource = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/LMH/protomap/NS_SpriteEffect.NS_SpriteEffect"));*/
	NSSpritecomp=CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect Comp"));
	NSSpritecomp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> temp_effect(TEXT("/Game/LMH/protomap/NS_SpriteEffect.NS_SpriteEffect"));
	if (temp_effect.Succeeded()) NSSpritecomp->SetAsset(temp_effect.Object);
	
}

// Called when the game starts or when spawned
void ANSActor::BeginPlay()
{
	Super::BeginPlay();
	NSSpritecomp->SetVisibility(false);
}

// Called every frame
void ANSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANSActor::SetVisibleOnOff(bool isOn)
{
	if (isOn){
		
		NSSpritecomp->SetVisibility(isOn);
		StartTimer();
	}
}

// Function to start the timer
void ANSActor::StartTimer()
{
	FTimerHandle TimerHandle;
	// Get a reference to the world's timer manager
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANSActor::TimerCallback, 1.0f, false, 0.f);
	}
}

// Function to be called by the timer
void ANSActor::TimerCallback()
{
	//UE_LOG(LogTemp, Warning, TEXT("Timer Callback Called"));
	NSSpritecomp->SetVisibility(false);
}