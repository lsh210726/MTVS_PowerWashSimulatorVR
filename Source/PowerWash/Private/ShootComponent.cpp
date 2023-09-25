// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "WaterGunActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Kismet/KismetMathLibrary.h>



// Sets default values for this component's properties
UShootComponent::UShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//NGShootMuzzle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	//NGShooComp->SetupAttachment(RootComponent);
	// ...
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();
	//if (NGShootMuzzle) {
	//	// This spawns the chosen effect on the owning WeaponMuzzle SceneComponent
	//	NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(NGShootMuzzle, player->waterGun->muzzleMesh, NAME_None, FVector(0.f), UKismetMathLibrary::MakeRotFromX(player->waterGun->muzzleMesh->GetForwardVector()), EAttachLocation::Type::SnapToTarget, true);

	//	NiagaraComp->SetVisibility(false);
	//}

	// UKismetMathLibrary::MakeRotFromX(player->waterGun->muzzleMesh->GetRightVector())
}


// Called every frame
void UShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShootComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Triggered, this, &UShootComponent::RighttTriggerDown);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UShootComponent::OnNiagaraEffect);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UShootComponent::RighttTriggerDown);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UShootComponent::OffNiagaraEffect);
	enhancedInputComponent->BindAction(inputActions[7], ETriggerEvent::Triggered, this, &UShootComponent::RightHandMove);
	enhancedInputComponent->BindAction(inputActions[8], ETriggerEvent::Triggered, this, &UShootComponent::HorShot);
	enhancedInputComponent->BindAction(inputActions[9], ETriggerEvent::Triggered, this, &UShootComponent::ChangeAngle);
	enhancedInputComponent->BindAction(inputActions[10], ETriggerEvent::Triggered, this, &UShootComponent::ShowUI);
	enhancedInputComponent->BindAction(inputActions[11], ETriggerEvent::Triggered, this, &UShootComponent::UIClick);

}

void UShootComponent::RighttTriggerDown()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Fire"));
	if (player->bHasGun)player->waterGun->Shoot();
}

//이민하 추가, NiagaraEffect visiblity 조절
void UShootComponent::OnNiagaraEffect()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("111111111"));
	//NiagaraComp->SetVisibility(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("33333"));
		// Parameters can be set like this (see documentation for further info) - the names and type must match the user exposed parameter in the Niagara System
		//NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
	 
}

void UShootComponent::OffNiagaraEffect()
{
	//
}

void UShootComponent::RightHandMove(const struct FInputActionValue& value)
{
	//float rotX = value.X;
	//player->rightMotionController->SetRelativeRotation(player->rightMotionController->GetRelativeRotation() + handRot);
	// Axis2D 값 가져오기 (X는 Yaw, Y는 Pitch)



	FVector2D AxisValues = value.Get<FVector2D>();
	//UE_LOG(LogTemp, Warning, TEXT("value : %f , %f"), AxisValues.X,AxisValues.Y);

	if (player && player->rightMotionController)
	{
		// 회전 속도 설정
		float RotationSpeed = 100.f;

		// Delta 시간과 함께 사용하여 실제 게임에서 부드럽게 작동하도록 함
		float YawRotationAmount = AxisValues.X * RotationSpeed * GetWorld()->GetDeltaSeconds();
		float PitchRotationAmount = AxisValues.Y * RotationSpeed * GetWorld()->GetDeltaSeconds();

		// 현재 액터의 상대적인 로테이션 가져오기
		FRotator CurrentRotation = player->rightMotionController->GetRelativeRotation();

		// Yaw와 Pitch 값을 변경하여 새로운 로테이션 생성 
		if(CurrentRotation.Roll ==180) 
		{
			FRotator NewRotation(CurrentRotation.Pitch - PitchRotationAmount, CurrentRotation.Yaw -YawRotationAmount, CurrentRotation.Roll);
			player->rightMotionController->SetRelativeRotation(NewRotation);

		}
		else
		{
			FRotator NewRotation(CurrentRotation.Pitch + PitchRotationAmount, CurrentRotation.Yaw + YawRotationAmount, CurrentRotation.Roll);
			player->rightMotionController->SetRelativeRotation(NewRotation);

		}

		// 액터에 새로운 상대적인 로테이션 설정
		//player->rightMotionController->SetRelativeRotation(NewRotation);

		UE_LOG(LogTemp, Warning, TEXT("value : %f , %f, %f"), CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll);
	}
}

void UShootComponent::ChangeAngle()
{
	if (player->bHasGun)player->waterGun->ChangeAngle();
}

void UShootComponent::ShowUI()
{
	if (!bShowUI)bShowUI = true;
	else bShowUI = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, bShowUI);

}

void UShootComponent::HorShot()
{
	if (player->bHasGun)player->waterGun->shotRot();
}

void UShootComponent::UIClick()
{

}
