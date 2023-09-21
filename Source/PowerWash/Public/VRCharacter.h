// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HandAnimInstance.h"
#include "..\Source\PowerWash\Public\ShootComponent.h"
#include "VRCharacter.generated.h"

UCLASS()
class POWERWASH_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UCameraComponent* hmdCam;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UStaticMeshComponent* hmdMesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="MySettings|Components")
	class UMotionControllerComponent* leftMotionController;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UTextRenderComponent* leftLog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MySettings|Components")
	class UMotionControllerComponent* rightMotionController;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UTextRenderComponent* rightLog;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UMoveComponent* moveComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UNiagaraComponent* lineFx;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UGrabComponent* grabComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UHandAnimComponent* handAnimComp;

	class UShootComponent* shootComp;//물총쏘기

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class AWaterGunActor* waterGun;

	UPROPERTY(EditAnywhere, Category="MySettings|Inputs")
	class UInputMappingContext* imc_VRmap;

	UPROPERTY(EditAnywhere, Category="MySettings|Inputs")
	TArray<class UInputAction*> inputActions;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	TSubclassOf<class ABallActor> myBall_bp;
	
	UPROPERTY(EditDefaultsOnly, Category = "MySettings|MoveMent")
	bool bIsTesting = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "MySettings|MoveMent")
	bool bHasGun = false;

	class APlayerController* pc;
	class ABallActor* ball;
	class UHandAnimInstance* leftHandAnim;
	class UHandAnimInstance* rightHandAnim;

public:
// LMH Decal component 추가 -> 수정: RenderTarget으로 변경 << 이민하 작성
	/*UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "MySettings|Components")
	class URenderTargetProcess* DrawComp;*/	

private:
	void RightTriggerDown();
	void RightTriggerUp();
	void RightTriggerValue(const struct FInputActionValue& value);
	void RightGripDown();
	void RightGripUp();
	void RightThumbstickDown();
	void RightThumbstickUp();
	void RightThumbstickTouch();
	void RightThumbstickAxis(const struct FInputActionValue& value);
	void RightADown();
	void RightAUP();
	void RightATouch();
	void RightBDown();
	void RightBUp();
	void RightBTouch();
	
};
