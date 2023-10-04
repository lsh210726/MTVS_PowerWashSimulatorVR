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
#include <Components/AudioComponent.h>
#include <Sound/SoundCue.h>
#include <Kismet/GameplayStatics.h>
#include <Camera/PlayerCameraManager.h>
#include <Particles/ParticleSystemComponent.h>



// Sets default values for this component's properties
UShootComponent::UShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//NGShootMuzzle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	//NGShooComp->SetupAttachment(RootComponent);
	// ...

	//�̹��� �߰�
	static ConstructorHelpers::FObjectFinder<USoundCue> temp_Cue(TEXT("/Game/LMH/Sound/SprayQue.SprayQue"));
	if (temp_Cue.Succeeded()) SprayCue = temp_Cue.Object;
	SprayCueComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SprayComp"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SteamParticleAsset(TEXT("/Game/LMH/betamap/P_Steam_Lit.P_Steam_Lit"));
	if (SteamParticleAsset.Succeeded()) SteamParticles = SteamParticleAsset.Object;
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();

	//�̹��� �߰�
	if (SprayCue) {
		SprayCueComponent->SetSound(SprayCue);
		SprayCueComponent->Activate(true);
		SprayCueComponent->SetPaused(true);
	}
	SteamComp = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		SteamParticles,
		FVector(0,0,-100),
		FRotator::ZeroRotator,
		true
	);
	if (SteamComp) SteamComp->SetVisibility(false);


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
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Triggered, this, &UShootComponent::OnNiagaraEffectTrigger);
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

//�̹��� �߰�, sound �߰�
void UShootComponent::OnNiagaraEffect()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("111111111"));
	//NiagaraComp->SetVisibility(true);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("33333"));
		// Parameters can be set like this (see documentation for further info) - the names and type must match the user exposed parameter in the Niagara System
		//NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
	//if (SprayCueComponent->bIsPaused) SprayCueComponent->SetPaused(false); 

	
}

void UShootComponent::OffNiagaraEffect()
{
	//if (!SprayCueComponent->bIsPaused) SprayCueComponent->SetPaused(true);
	if (SteamComp) SteamComp->SetVisibility(false);


}

void UShootComponent::OnNiagaraEffectTrigger()
{
	if (SteamComp) {
		if (!SteamComp->IsVisible()) SteamComp->SetVisibility(true);
	}
}

void UShootComponent::RightHandMove(const struct FInputActionValue& value)
{
	//float rotX = value.X;
	//player->rightMotionController->SetRelativeRotation(player->rightMotionController->GetRelativeRotation() + handRot);
	// Axis2D �� �������� (X�� Yaw, Y�� Pitch)



	//FVector2D AxisValues = value.Get<FVector2D>();
	////UE_LOG(LogTemp, Warning, TEXT("value : %f , %f"), AxisValues.X,AxisValues.Y);

	//if (player && player->rightMotionController)
	//{
	//	// ȸ�� �ӵ� ����
	//	float RotationSpeed = 100.f;

	//	// Delta �ð��� �Բ� ����Ͽ� ���� ���ӿ��� �ε巴�� �۵��ϵ��� ��
	//	float YawRotationAmount = AxisValues.X * RotationSpeed * GetWorld()->GetDeltaSeconds();
	//	float PitchRotationAmount = AxisValues.Y * RotationSpeed * GetWorld()->GetDeltaSeconds();

	//	// ���� ������ ������� �����̼� ��������
	//	FRotator CurrentRotation = player->rightMotionController->GetRelativeRotation();

	//	// Yaw�� Pitch ���� �����Ͽ� ���ο� �����̼� ���� 
	//	if(CurrentRotation.Roll ==180) 
	//	{
	//		FRotator NewRotation(CurrentRotation.Pitch - PitchRotationAmount, CurrentRotation.Yaw -YawRotationAmount, CurrentRotation.Roll);
	//		player->rightMotionController->SetRelativeRotation(NewRotation);

	//	}
	//	else
	//	{
	//		FRotator NewRotation(CurrentRotation.Pitch + PitchRotationAmount, CurrentRotation.Yaw + YawRotationAmount, CurrentRotation.Roll);
	//		player->rightMotionController->SetRelativeRotation(NewRotation);

	//	}

	//	// ���Ϳ� ���ο� ������� �����̼� ����
	//	//player->rightMotionController->SetRelativeRotation(NewRotation);

	//	UE_LOG(LogTemp, Warning, TEXT("value : %f , %f, %f"), CurrentRotation.Pitch, CurrentRotation.Yaw, CurrentRotation.Roll);
	//}


	FVector2D direction = value.Get<FVector2D>();
	FVector dir3 = FVector(0,direction.X, direction.Y);
	player->rightMotionController->SetRelativeLocation(player->rightMotionController->GetRelativeLocation() + dir3.GetSafeNormal());
	player->rightMotionController->SetRelativeRotation(FRotator(90, 0, 0));
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
