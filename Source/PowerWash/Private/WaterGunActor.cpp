// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGunActor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "DrawDebugHelpers.h"
#include "DecalCompoenent.h"
#include "VRCharacter.h"
#include <Components/ArrowComponent.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include "RenderTargetProcess.h"
#include "Kismet/GameplayStatics.h"
#include "MuzzleActor.h"
#include "ShootComponent.h"
#include <Particles/ParticleSystemComponent.h>




// Sets default values
AWaterGunActor::AWaterGunActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(10,35,18));
	boxComp->SetSimulatePhysics(false);

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, -24, -5));
	meshComp->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AWaterGunActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{

		});

	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//이민하 추가
	shootComp = player->GetComponentByClass<UShootComponent>();


	player->waterGun = this;
	grabReleseGun();

	TargetRotation = StartRotation;

	// 회전 시간 설정
	RotationTime = 2.0f; // 예: 2초 동안 회전

	// 시작 시간 저장
	StartTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("StartRotation : %f,%f,%f"), StartRotation.Roll, StartRotation.Pitch, StartRotation.Yaw);


}

// Called every frame
void AWaterGunActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaterGunActor::Shoot()
{
	FHitResult hitInfo;
	if (player->bHasGun && MuzzleActor!=nullptr)
	{
		muzzleLocation = meshComp->GetSocketLocation(TEXT("Muzzle"));
		muzzleRotation = meshComp->GetSocketRotation(TEXT("Muzzle"));
		USkeletalMeshSocket const* mySocket = nullptr;
		mySocket = meshComp->GetSocketByName(TEXT("Muzzle"));
		WideShot(shotAngle);
		shotSoundPlay();
	}
}

void AWaterGunActor::WideShot(float degree)//물줄기 넓게 쏘는 각도 변경
{
	FVector muzzleFwdVec;
	if (!horShot) muzzleRotation.Pitch -= degree / 2;
	else muzzleRotation.Yaw -= degree / 2;

	for (int i = 0; i < shotTime; ++i)
	{
		muzzleFwdVec = muzzleRotation.Vector();
		//muzzleFwdVec = FVector(muzzleRotation.Roll, muzzleRotation.Pitch, muzzleRotation.Yaw);
		UE_LOG(LogTemp, Warning, TEXT(" % f, % f, % f"), muzzleRotation.Roll, muzzleRotation.Pitch, muzzleRotation.Yaw);
		UE_LOG(LogTemp, Warning, TEXT(" % f, % f, % f"), muzzleFwdVec.X, muzzleFwdVec.Y, muzzleFwdVec.Z);
		ShootWater(muzzleFwdVec);
		if (!horShot) muzzleRotation.Pitch += degree / shotTime;
		else muzzleRotation.Yaw += degree / shotTime;
	}
}

void AWaterGunActor::ShootWater(FVector muzzleFwdVec)//물 쏘기
{
	FHitResult hitInfo;


	if (GetWorld()->LineTraceSingleByChannel(hitInfo, muzzleLocation, muzzleLocation + muzzleFwdVec * shootPower, ECC_Visibility))
	{
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.bTraceComplex = true;
		params.bReturnPhysicalMaterial = true;
		FHitResult result2;

		if (hitInfo.GetComponent()->LineTraceComponent(result2, muzzleLocation, muzzleLocation + muzzleFwdVec * shootPower, params))
		{
			auto HitActor = Cast<AActor>(result2.GetActor());
			 // (result2.ImpactPoint);
			if(shootComp) shootComp->SteamComp->SetWorldLocation(result2.ImpactPoint);
			

			URenderTargetProcess* renderComp = HitActor->GetComponentByClass<URenderTargetProcess>();
			if (renderComp) renderComp->DrawCar(result2,muzzleLocation);

		}
	}
}

void AWaterGunActor::ChangeAngle()//물줄기를 세로로 쏠지 가로로 쏠지
{
	if (shotAngle < 40) shotAngle += 10;
	else shotAngle = 0;
	//FMath::Lerp;

}

void AWaterGunActor::shotRot()
{
	if (horShot)horShot = false;
	else horShot = true;
}

void AWaterGunActor::grabReleseGun()//플레이어 손 소켓에 총 붙이기
{
	if (player)
	{
		if (!player->bHasGun)
		{
			if (player->rightHand)
			{
				AttachToComponent(player->rightHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
				player->bHasGun = true;

				UE_LOG(LogTemp, Warning, TEXT("grabGun"));
	
			}
		}
		else
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			SetActorLocation(FVector(0));
			UE_LOG(LogTemp, Warning, TEXT("NoGrabGun"));
			player->bHasGun = false;
		}
		UE_LOG(LogTemp, Warning, TEXT("grabReleseGunEventOn"));

	}
}

void AWaterGunActor::shotSoundPlay_Implementation()
{

}

