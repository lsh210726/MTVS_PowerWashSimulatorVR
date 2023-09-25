// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "..\Source\PowerWash\Public\VRCharacter.h"
#include "ShootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POWERWASH_API UShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShootComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);
	void RighttTriggerDown();
	void OnNiagaraEffect();    //이민하 추가
	void OffNiagaraEffect();    //이민하 추가
	void RightHandMove(const struct FInputActionValue& value);
	void ChangeAngle();
	void ShowUI();

	UFUNCTION(BlueprintCallable)
	void HorShot();
	UFUNCTION(BlueprintCallable)
	void UIClick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Components")
	bool bShowUI = false;

	class AVRCharacter* player;

//이민하 코드추가
public:
	UPROPERTY(EditAnywhere,Category="MySettings|Effects")
    class UNiagaraSystem* NGShootMuzzle;
	
	UPROPERTY(VisibleAnywhere,Category="MySettings|Effects")
	class UNiagaraComponent* NiagaraComp;
};
