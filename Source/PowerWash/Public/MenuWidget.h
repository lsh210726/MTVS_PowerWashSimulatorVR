// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class POWERWASH_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category=UI,meta=(BindWidget))
	class UButton* btn_BuildUIOnOff;

	UPROPERTY(EditDefaultsOnly, Category=UI,meta=(BindWidget))
	class UButton* btn_GameStart;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void BuildUIOnOff();

	UFUNCTION(BlueprintNativeEvent)
	void GameStart();
	
};
