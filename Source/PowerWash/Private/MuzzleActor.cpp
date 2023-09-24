// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzleActor.h"
#include "VRCharacter.h"
#include "WaterGunActor.h"
#include <Components/SkeletalMeshComponent.h>


// Sets default values
AMuzzleActor::AMuzzleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);

	material1 = CreateDefaultSubobject<UMaterialInterface>("Material1");
	material2 = CreateDefaultSubobject<UMaterialInterface>("Material2");
	material3 = CreateDefaultSubobject<UMaterialInterface>("Material3");
	material4 = CreateDefaultSubobject<UMaterialInterface>("Material4");
}

// Called when the game starts or when spawned
void AMuzzleActor::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player->bHasGun) {
		waterGun = player->waterGun;
		AttachToComponent(waterGun->meshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Muzzle"));
		waterGun->MuzzleActor = this;
		GetWorldTimerManager().SetTimer(MyTimerHandle, this, &AMuzzleActor::rotateEvent, 2.8f, true);//2.8초에 한번씩 회전시키기
	}

	meshComp->SetMaterial(0, material1);
}

// Called every frame
void AMuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (letsRotate) rotateMuzzle();
	if (90 <= rotCounter) {
		letsRotate = false;
		rotCounter = 0;
	}
}

void AMuzzleActor::rotateEvent()
{
	letsRotate = true;
}

void AMuzzleActor::rotateMuzzle()
{
	//UE_LOG(LogTemp, Warning, TEXT("rotate"));
	meshComp->AddRelativeRotation(FRotator(0, 0, 1));
	++rotCounter;
}

void AMuzzleActor::Attached(USkeletalMeshComponent* handMesh, const ANSICHAR* pointName )//붙일 스켈리탈메시, 붙일 소켓이름
{
	meshComp->SetSimulatePhysics(false);
	AttachToComponent(handMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(pointName));

}

void AMuzzleActor::changeMaterial(int i)//머터리얼 변경 함수
{
	switch (i)
	{
	case 1:
		meshComp->SetMaterial(0, material1);
		break;
	case 2:
		meshComp->SetMaterial(0, material2);
		break;
	case 3:
		meshComp->SetMaterial(0, material3);
		break;
	case 4:
		meshComp->SetMaterial(0, material4);
		break;
	default:
		break;
	}
	
}

