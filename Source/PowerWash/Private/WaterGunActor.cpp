// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGunActor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "DrawDebugHelpers.h"
#include "DecalCompoenent.h"
#include "VRCharacter.h"
#include <Components/ArrowComponent.h>
#include <Kismet/GameplayStatics.h>
#include "RenderTargetProcess.h"

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

	muzzleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Muzzle Mesh"));
	muzzleMesh->SetupAttachment(meshComp);
	muzzleMesh->SetSimulatePhysics(false);
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Comp"));
	arrowComp->SetupAttachment(muzzleMesh);
	arrowComp->SetSimulatePhysics(false);
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
			player->bHasGun = true;
			player->waterGun = this;
			//if(player->waterGun != nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,*player->waterGun->GetName());

		}
	}
	
	currRot = muzzleMesh->GetRelativeRotation();

	StartRotation = muzzleMesh->GetRelativeRotation();
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


	if (muzzleMesh)
	{
			StartRotation = muzzleMesh->GetRelativeRotation();
		if (StartRotation.Roll <= TargetRotation.Roll)
		{
			auto changeRot = FRotator(StartRotation.Pitch , StartRotation.Yaw, StartRotation.Roll + 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("StartRotation : %f,%f,%f & TargetRotation : %f,%f,%f &changeRot : %f,%f,%f"), StartRotation.Roll, StartRotation.Pitch, StartRotation.Yaw, TargetRotation.Roll, TargetRotation.Pitch, TargetRotation.Yaw, changeRot.Roll, changeRot.Pitch, changeRot.Yaw);
			muzzleMesh->SetRelativeRotation(changeRot);
		}

		muzzleRotation = muzzleMesh->GetComponentRotation();//meshComp->GetSocketRotation(TEXT("Muzzle"));

	}

}

void AWaterGunActor::Shoot()
{
	FHitResult hitInfo;
	if (player->bHasGun && meshComp->DoesSocketExist(TEXT("Muzzle")))
	{
		muzzleLocation = meshComp->GetSocketLocation(TEXT("Muzzle"));
		muzzleRotation = /*muzzleMesh->GetComponentRotation();*/meshComp->GetSocketRotation(TEXT("Muzzle"));
		USkeletalMeshSocket const* mySocket = nullptr;
		mySocket= meshComp->GetSocketByName(TEXT("Muzzle"));
		
		WideShot(shotAngle);
	}
}

void AWaterGunActor::WideShot(float degree)
{
	FVector muzzleFwdVec;
	if (!horShot) muzzleRotation.Pitch -= degree / 2;
	else muzzleRotation.Yaw -= degree / 2;

	for (int i = 0; i < shotTime; ++i)
	{
		muzzleFwdVec = muzzleRotation.Vector();
		ShootWater(muzzleFwdVec);
		if (!horShot) muzzleRotation.Pitch += degree / shotTime;
		else muzzleRotation.Yaw += degree / shotTime;
	}
}

void AWaterGunActor::ShootWater(FVector muzzleFwdVec)
{
	FHitResult hitInfo;
	DrawDebugLine(GetWorld(), muzzleLocation, muzzleLocation + muzzleFwdVec * shootPower, FColor::White, false, 0.2f, 0, 1.0f);
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, muzzleLocation, muzzleLocation + muzzleFwdVec * shootPower, ECC_Visibility))
	{
		DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 5, 8, FColor::White, false, 0.2f, 0, 0.3f);
		//player->DrawComp->DrawCar(hitInfo);
	}	
}

void AWaterGunActor::ChangeAngle()
{
	if (shotAngle < 40) shotAngle += 10;
	else shotAngle = 0;
	//FMath::Lerp;

}

void AWaterGunActor::shotRot()
{
	if (horShot)horShot = false;
	else horShot = true;
	/*currRot = muzzleMesh->GetComponentRotation();*/
	if (currRot.Pitch > 88) currRot.Pitch+= 90;
	if(oneTime)
	{

		TargetRotation = FRotator(StartRotation.Pitch , StartRotation.Yaw, StartRotation.Roll + 90.f);
		UE_LOG(LogTemp, Warning, TEXT("oneTIme"));
		oneTime = false;
	}

}

