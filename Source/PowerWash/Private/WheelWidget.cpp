// Fill out your copyright notice in the Description page of Project Settings.


#include "WheelWidget.h"
#include "Components/Button.h"

void UWheelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_LT->OnClicked.AddDynamic(this, &UWheelWidget::LT);
	btn_RT->OnClicked.AddDynamic(this, &UWheelWidget::RT);
	btn_LB->OnClicked.AddDynamic(this, &UWheelWidget::LB);
	btn_RB->OnClicked.AddDynamic(this, &UWheelWidget::RB);
}

void UWheelWidget::LT()
{
	UE_LOG(LogTemp, Warning, TEXT("LT"));
}

void UWheelWidget::RT()
{
	UE_LOG(LogTemp, Warning, TEXT("RT"));

}

void UWheelWidget::LB()
{
	UE_LOG(LogTemp, Warning, TEXT("LB"));

}

void UWheelWidget::RB()
{
	UE_LOG(LogTemp, Warning, TEXT("RB"));

}

