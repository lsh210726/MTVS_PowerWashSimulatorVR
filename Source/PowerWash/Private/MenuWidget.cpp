// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_BuildUIOnOff->OnClicked.AddDynamic(this, &UMenuWidget::BuildUIOnOff);
	btn_GameStart->OnClicked.AddDynamic(this, &UMenuWidget::GameStart);
}

void UMenuWidget::BuildUIOnOff_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("build ui button clicked."));
}

void UMenuWidget::GameStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("game start ui button clicked."));
}
