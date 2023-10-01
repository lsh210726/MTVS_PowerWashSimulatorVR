// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "VRCharacter.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include "MotionControllerComponent.h"
#include "PickUpActor.h"
#include <Components/TextRenderComponent.h>
#include <Components/BoxComponent.h>
#include "MuzzleActor.h"
#include "WaterGunActor.h"
#include "Components/WidgetComponent.h"
#include "wheelWidget.h"



// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//if (grabbedObject != nullptr)
	//{
	//	deltaLoc = player->leftMotionController->GetComponentLocation() - prevLoc;//��ġ��ȭ��
	//	prevLoc = player->leftMotionController->GetComponentLocation();//������ġ�� ����

	//	deltaRot = player->leftMotionController->GetComponentQuat() - prevRot.Inverse();
	//	prevRot = player->leftMotionController->GetComponentQuat();
	//}
	if (isRot)
	{
		float f = player->leftHand->GetRelativeRotation().Roll - firstHandRot.Roll;
		
		if (f >= 30)
		{
			UE_LOG(LogTemp, Warning, TEXT("%f"), f);
			isRot = false;
			if(player->waterGun->MuzzleActor!=nullptr)player->waterGun->MuzzleActor->rotateEvent();
			if (grabbedObject != nullptr)
			{
				grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//�� �ڸ����� ���� ����
				grabbedObject->Attached(grabbedObject->waterGun->meshComp, "Muzzle");
				grabbedObject = nullptr;
				player->bHasMuzzle = false;
				player->waterGun->shotRot();

				//������ �� �ʱ�ȭ
				isRot = false;
				//firstHandRot = player->leftHand->GetRelativeRotation();
			}
		}
	}
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent, TArray<UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject);
	enhancedInputComponent->BindAction(inputActions[4], ETriggerEvent::Triggered, this, &UGrabComponent::leftHandMove);
}

void UGrabComponent::GrabObject()
{
	//if (grabbedObject != nullptr)
	//{
	//	return;
	//}


#pragma region lineTrace Type
//line trace ���
	
	//FHitResult hitInfo;
	//FVector startLoc = player->rightMotionController->GetComponentLocation(); 
	//FVector endLoc = startLoc + player->rightMotionController->GetUpVector()*-50.0f;
	//DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Red, false, 1, 0, 3.0f);

	//if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, FName("PickUp")))
	//{
	//	APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor()); 
	//	if (pickObject != nullptr)
	//	{
	//		pickObject->Grabbed(player->rightHand);
	//	}
	//	player->rightLog->SetText(FText::FromString(FString::Printf(TEXT("Grab Object : %s"), *hitInfo.GetActor()->GetName())));
	//	UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetActor()->GetName());
	//}
	//else
	//{
	//	player->rightLog->SetText(FText::FromString(FString::Printf(TEXT("No Hit"))));
	//	UE_LOG(LogTemp, Warning, TEXT("No Hit"));

	//}
#pragma endregion

#pragma region sweepGrab


	//sweep ���
	//FHitResult hitInfo;
	//FVector startLoc = player->rightHand->GetComponentLocation();

	////DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Red, false, 1, 0, 3.0f);
	//DrawDebugSphere(GetWorld(), startLoc, 20, 12, FColor::Red, false, 1, 0, 0.1f);

	//if (GetWorld()->SweepSingleByProfile(hitInfo, startLoc, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))//��ü ����� �浹�ϱ�
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetComponent()->GetName());

	//	if (APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor()))
	//	{
	//		pickObject->Grabbed(player->rightHand);
	//	}
	//}
#pragma endregion

#pragma region grabObj


	////overlapGrab
	//TArray<FOverlapResult> hitInfos;
	//FVector startLoc = player->leftHand->GetComponentLocation();
	//if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(30)))
	//{
	//	for (const FOverlapResult& hitInfo : hitInfos)
	//	{
	//		if (APickUpActor* pickObj = Cast<APickUpActor>(hitInfo.GetActor()))
	//		{
	//			pickObj->Grabbed(player->leftHand);
	//			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grab!"));
	//			grabbedObject = pickObj;
	//			player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Left, 1.0f, false);//��ü�� ������ �����ϱ�
	//			break;//���� ���� ����� �� �ϳ��� ��� ���� ��� ���⿡ break�� �� ��
	//		}
	//		//UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetComponent()->GetName());

	//	}

	//}
	//DrawDebugSphere(GetWorld(), startLoc, 30, 12, FColor::Red, false, 1, 0, 0.1f);
#pragma endregion

		//overlapGrab
	TArray<FOverlapResult> hitInfos;
	FVector startLoc = player->leftHand->GetComponentLocation();
	if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(10)))
	{
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetComponent()->GetName());

			if (AMuzzleActor* pickObj = Cast<AMuzzleActor>(hitInfo.GetActor()))
			{
				if(!player->bHasMuzzle)
				{
					pickObj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//�� �ڸ����� ���� ����
					pickObj->Attached(player->leftHand, "GrabPoint");
					grabbedObject = pickObj;
					player->bHasMuzzle = true;
					player->MenuUI->SetHiddenInGame(false);
					//player->MenuUI->SetRelativeLocation(FVector(40, 0, 0));

					FTimerHandle GravityTimerHandle;
					float GravityTime=0.1f;

					GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
						{
							// �ڵ� ����
							player->MenuUI->SetRelativeLocation(FVector(40, 0, 0));

							// TimerHandle �ʱ�ȭ
							GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
						}), GravityTime, false);	// �ݺ��Ϸ��� false�� true�� ����

					//player->MenuUI->playAnima

					//UWheelWidget* wui = Cast<UWheelWidget>(player->WheelUI);
					//wui->isShow = true;

					player->isWheelMenuHidden = false;

					//����� �� �� ȸ���� ���
					isRot = true;
					firstHandRot = player->leftHand->GetRelativeRotation();

					player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Left, 1.0f, false);//��ü�� ������ �����ϱ�
				}
				//break;
			}
			//else if (hitInfo.GetComponent()->GetName() == "MuzzleHolder")//���� ĳ���Ͱ� ����Ȧ������ ���� ��´ٸ�
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("MuzzleHolder Grab"));
			//	if (player->bHasMuzzle)
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("true"));
			//		player->WheelUI->SetHiddenInGame(true);//�������� ������
			//		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGrabComponent::UIHider, 0.5f, false);
			//		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			//			{
			//				player->WheelUI->SetHiddenInGame(true);//�������� ������
			//				UE_LOG(LogTemp, Warning, TEXT("UI Hide"));
			//			});
			//	}
			//}

		}

	}
	//DrawDebugSphere(GetWorld(), startLoc, 10, 12, FColor::Red, false, 1, 0, 0.1f);
}

void UGrabComponent::ReleaseObject()
{

	//if(grabbedObject!=nullptr)
	//{
	//	//��ü�� �տ��� �и��ϰ� ���� �ɷ��� Ȱ��ȭ�Ѵ�
	//	grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//�� �ڸ����� ���� ����
	//	grabbedObject->boxComp->SetSimulatePhysics(true);

	//	//��ü�� ������ ���⿡ ���� ��(����, ȸ����)�� ���Ѵ�
	//	if (!deltaLoc.IsNearlyZero())
	//	{
	//		grabbedObject->boxComp->AddImpulse(deltaLoc.GetSafeNormal() * throwPower);
	//		grabbedObject->boxComp->AddTorqueInRadians(deltaRot.GetRotationAxis() * rotSpeed);
	//	}
	//	grabbedObject = nullptr;
	//}

	TArray<FOverlapResult> hitInfos;
	FVector startLoc = player->leftHand->GetComponentLocation();
	if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(10)))
	{
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			if (AWaterGunActor* pickObj = Cast<AWaterGunActor>(hitInfo.GetActor()))//���� ���ѿ��� Ʈ���Ÿ� ���Ҵٸ�
			{
				if(player->bHasMuzzle)
				{
					UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetComponent()->GetName());
					if (grabbedObject != nullptr)
					{
						grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//�� �ڸ����� ���� ����
						grabbedObject->Attached(grabbedObject->waterGun->meshComp, "Muzzle");
						grabbedObject = nullptr;
						player->bHasMuzzle = false;
						//player->MenuUI->SetHiddenInGame(true);
						//player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));

						player->isWheelMenuHidden = true;

						FTimerHandle GravityTimerHandle;
						float GravityTime = 0.5f;

						GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
							{
								// �ڵ� ����
								player->MenuUI->SetHiddenInGame(true);
								player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
								// TimerHandle �ʱ�ȭ
								GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
							}), GravityTime, false);	// �ݺ��Ϸ��� false�� true�� ����

						//������ �� �ʱ�ȭ
						isRot = false;
						//firstHandRot = player->leftHand->GetRelativeRotation();
					}
					break;
				}
			}
			//else if (hitInfo.GetComponent()->GetName() == "MuzzleHolder")//���� ����Ȧ������ Ʈ���Ÿ� ���Ҵٸ�
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("MuzzleHolder release"));
			//	if (player->bHasMuzzle)
			//	{
			//		player->WheelUI->SetHiddenInGame(false);//�������� �����ش�
			//	}
			//}
		}
	}
}

void UGrabComponent::leftHandMove(const struct FInputActionValue& value)
{
	FVector direction = value.Get<FVector>();
	player->leftMotionController->SetRelativeLocation(player->leftMotionController->GetRelativeLocation() + direction.GetSafeNormal());
}

void UGrabComponent::UIHider()
{
	player->WheelUI->SetHiddenInGame(true);//�������� ������
	UE_LOG(LogTemp, Warning, TEXT("UI Hide"));
}

void UGrabComponent::changeHiddenWheelUI()
{
//	if (player->WheelUI != nullptr && player->WheelUI->bHiddenInGame) player->WheelUI->SetHiddenInGame(false);//if hidden in game is true
//	else if(player->WheelUI != nullptr && !player->WheelUI->bHiddenInGame) player->WheelUI->SetHiddenInGame(true);
	
}

