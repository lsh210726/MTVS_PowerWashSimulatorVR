// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGunActor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "VRCharacter.h"

// Sets default values
AWaterGunActor::AWaterGunActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetSimulatePhysics(false);

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AWaterGunActor::BeginPlay()
{
	Super::BeginPlay();
	//owner를 설정해줘야 한다
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{

	});

	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (player)
	{
		if (player->rightHand)
		{
			AttachToComponent(player->rightHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
		}
	}


}

// Called every frame
void AWaterGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

