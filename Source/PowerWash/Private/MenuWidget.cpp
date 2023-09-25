// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Components/Button.h"
#include "VRCharacter.h"
#include "WaterGunActor.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_BuildUIOnOff->OnClicked.AddDynamic(this, &UMenuWidget::BuildUIOnOff);
	btn_GameStart->OnClicked.AddDynamic(this, &UMenuWidget::GameStart);

	player = Cast<AVRCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

}

void UMenuWidget::BuildUIOnOff_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("build ui button clicked."));
	player->waterGun->grabReleseGun();
}

void UMenuWidget::GameStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("game start ui button clicked."));
}
