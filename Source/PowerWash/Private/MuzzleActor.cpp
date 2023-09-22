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
}

// Called when the game starts or when spawned
void AMuzzleActor::BeginPlay()
{
	Super::BeginPlay();
	
	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player->bHasGun) {
		waterGun = player->waterGun;
		AttachToComponent(waterGun->meshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Muzzle"));
		GetWorldTimerManager().SetTimer(MyTimerHandle, this, &AMuzzleActor::rotateEvent, 2.8f, true);//2.8�ʿ� �ѹ��� ȸ����Ű��
	}
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

void AMuzzleActor::Attached(USkeletalMeshComponent* handMesh, const ANSICHAR* pointName )//���� ���̸�Ż�޽�, ���� �����̸�
{
	meshComp->SetSimulatePhysics(false);
	AttachToComponent(handMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(pointName));

}


