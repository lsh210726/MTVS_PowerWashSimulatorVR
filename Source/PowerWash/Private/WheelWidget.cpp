// Fill out your copyright notice in the Description page of Project Settings.


#include "WheelWidget.h"
#include "Components/Button.h"
#include "VRCharacter.h"
#include "WaterGunActor.h"
#include "MuzzleActor.h"
#include "Components/WidgetComponent.h"




void UWheelWidget::changeHidden(bool status)
{
	if (isShow) isShow = false;
	else isShow = true;
}

void UWheelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_LT->OnClicked.AddDynamic(this, &UWheelWidget::LT);
	btn_RT->OnClicked.AddDynamic(this, &UWheelWidget::RT);
	btn_LB->OnClicked.AddDynamic(this, &UWheelWidget::LB);
	btn_RB->OnClicked.AddDynamic(this, &UWheelWidget::RB);

	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(player->bHasGun) waterGun = player->waterGun;
	muzzle = waterGun->MuzzleActor;
}

void UWheelWidget::LT()
{
	UE_LOG(LogTemp, Warning, TEXT("LT"));
	waterGun->shotAngle = 0;
	muzzle->changeMaterial(1);
	//player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
	//player->MenuUI->SetHiddenInGame(true);
	player->isWheelMenuHidden = true;

	FTimerHandle GravityTimerHandle;
	float GravityTime = 0.5f;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			player->MenuUI->SetHiddenInGame(true);
			player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경
	
}

void UWheelWidget::RT()
{
	UE_LOG(LogTemp, Warning, TEXT("RT"));
	waterGun->shotAngle = 10;
	muzzle->changeMaterial(2);
	//player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
	//player->MenuUI->SetHiddenInGame(true);
	player->isWheelMenuHidden = true;

	FTimerHandle GravityTimerHandle;
	float GravityTime = 0.5f;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			player->MenuUI->SetHiddenInGame(true);
			player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경
}

void UWheelWidget::LB()
{
	UE_LOG(LogTemp, Warning, TEXT("LB"));
	waterGun->shotAngle = 30;
	muzzle->changeMaterial(3);
	//player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
	//player->MenuUI->SetHiddenInGame(true);
	player->isWheelMenuHidden = true;

	FTimerHandle GravityTimerHandle;
	float GravityTime = 0.5f;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			player->MenuUI->SetHiddenInGame(true);
			player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경
}

void UWheelWidget::RB()
{
	UE_LOG(LogTemp, Warning, TEXT("RB"));
	waterGun->shotAngle = 20;
	muzzle->changeMaterial(4);
	//player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
	//player->MenuUI->SetHiddenInGame(true);
	player->isWheelMenuHidden = true;

	FTimerHandle GravityTimerHandle;
	float GravityTime = 0.5f;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			player->MenuUI->SetHiddenInGame(true);
			player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경

}

