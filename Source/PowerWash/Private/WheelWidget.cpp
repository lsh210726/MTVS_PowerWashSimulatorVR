// Fill out your copyright notice in the Description page of Project Settings.


#include "WheelWidget.h"
#include "Components/Button.h"
#include "VRCharacter.h"
#include "WaterGunActor.h"
#include "MuzzleActor.h"



void UWheelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_LT->OnClicked.AddDynamic(this, &UWheelWidget::LT);
	btn_RT->OnClicked.AddDynamic(this, &UWheelWidget::RT);
	btn_LB->OnClicked.AddDynamic(this, &UWheelWidget::LB);
	btn_RB->OnClicked.AddDynamic(this, &UWheelWidget::RB);

	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	waterGun = player->waterGun;
	muzzle = waterGun->MuzzleActor;
}

void UWheelWidget::LT()
{
	UE_LOG(LogTemp, Warning, TEXT("LT"));
	waterGun->shotAngle = 0;
	muzzle->changeMaterial(1);
}

void UWheelWidget::RT()
{
	UE_LOG(LogTemp, Warning, TEXT("RT"));
	waterGun->shotAngle = 10;
	muzzle->changeMaterial(2);

}

void UWheelWidget::LB()
{
	UE_LOG(LogTemp, Warning, TEXT("LB"));
	waterGun->shotAngle = 30;
	muzzle->changeMaterial(3);

}

void UWheelWidget::RB()
{
	UE_LOG(LogTemp, Warning, TEXT("RB"));
	waterGun->shotAngle = 20;
	muzzle->changeMaterial(4);

}

