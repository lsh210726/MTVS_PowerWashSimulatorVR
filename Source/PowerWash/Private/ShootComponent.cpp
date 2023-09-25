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

//�̹��� �߰�, NiagaraEffect visiblity ����
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
	// Axis2D �� �������� (X�� Yaw, Y�� Pitch)



	FVector2D AxisValues = value.Get<FVector2D>();
	//UE_LOG(LogTemp, Warning, TEXT("value : %f , %f"), AxisValues.X,AxisValues.Y);

	if (player && player->rightMotionController)
	{
		// ȸ�� �ӵ� ����
		float RotationSpeed = 100.f;

		// Delta �ð��� �Բ� ����Ͽ� ���� ���ӿ��� �ε巴�� �۵��ϵ��� ��
		float YawRotationAmount = AxisValues.X * RotationSpeed * GetWorld()->GetDeltaSeconds();
		float PitchRotationAmount = AxisValues.Y * RotationSpeed * GetWorld()->GetDeltaSeconds();

		// ���� ������ ������� �����̼� ��������
		FRotator CurrentRotation = player->rightMotionController->GetRelativeRotation();

		// Yaw�� Pitch ���� �����Ͽ� ���ο� �����̼� ���� 
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

		// ���Ϳ� ���ο� ������� �����̼� ����
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
