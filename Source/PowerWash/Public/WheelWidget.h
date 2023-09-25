// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WheelWidget.generated.h"

/**
 * 
 */
UCLASS()
class POWERWASH_API UWheelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category=UI,meta=(BindWidget))
	class UButton* btn_LT;

	UPROPERTY(EditDefaultsOnly, Category=UI,meta=(BindWidget))
	class UButton* btn_RT;

	UPROPERTY(EditDefaultsOnly, Category=UI,meta=(BindWidget))
	class UButton* btn_LB;

	UPROPERTY(EditDefaultsOnly, Category=UI,meta=(BindWidget))
	class UButton* btn_RB;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void LT();
	UFUNCTION()
	void RT();
	UFUNCTION()
	void LB();
	UFUNCTION()
	void RB();

private:
	class AVRCharacter* player;
	class AWaterGunActor* waterGun;
	class AMuzzleActor* muzzle;
};
