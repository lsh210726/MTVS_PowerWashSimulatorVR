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
	//	deltaLoc = player->leftMotionController->GetComponentLocation() - prevLoc;//위치변화값
	//	prevLoc = player->leftMotionController->GetComponentLocation();//이전위치값 갱신

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
				grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//그 자리에서 연결 끊기
				grabbedObject->Attached(grabbedObject->waterGun->meshComp, "Muzzle");
				grabbedObject = nullptr;
				player->bHasMuzzle = false;
				player->waterGun->shotRot();

				//놓았을 떄 초기화
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
//line trace 방식
	
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


	//sweep 방식
	//FHitResult hitInfo;
	//FVector startLoc = player->rightHand->GetComponentLocation();

	////DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Red, false, 1, 0, 3.0f);
	//DrawDebugSphere(GetWorld(), startLoc, 20, 12, FColor::Red, false, 1, 0, 0.1f);

	//if (GetWorld()->SweepSingleByProfile(hitInfo, startLoc, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))//구체 만들고 충돌하기
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
	//			player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Left, 1.0f, false);//물체를 잡으면 진동하기
	//			break;//만약 가장 가까운 것 하나만 잡고 싶은 경우 여기에 break을 걸 것
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
					pickObj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//그 자리에서 연결 끊기
					pickObj->Attached(player->leftHand, "GrabPoint");
					grabbedObject = pickObj;
					player->bHasMuzzle = true;
					player->MenuUI->SetHiddenInGame(false);
					//player->MenuUI->SetRelativeLocation(FVector(40, 0, 0));

					FTimerHandle GravityTimerHandle;
					float GravityTime=0.1f;

					GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
						{
							// 코드 구현
							player->MenuUI->SetRelativeLocation(FVector(40, 0, 0));

							// TimerHandle 초기화
							GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
						}), GravityTime, false);	// 반복하려면 false를 true로 변경

					//player->MenuUI->playAnima

					//UWheelWidget* wui = Cast<UWheelWidget>(player->WheelUI);
					//wui->isShow = true;

					player->isWheelMenuHidden = false;

					//잡았을 때 손 회전값 계산
					isRot = true;
					firstHandRot = player->leftHand->GetRelativeRotation();

					player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Left, 1.0f, false);//물체를 잡으면 진동하기
				}
				//break;
			}
			//else if (hitInfo.GetComponent()->GetName() == "MuzzleHolder")//만약 캐릭터가 머즐홀더에서 손을 잡는다면
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("MuzzleHolder Grab"));
			//	if (player->bHasMuzzle)
			//	{
			//		UE_LOG(LogTemp, Warning, TEXT("true"));
			//		player->WheelUI->SetHiddenInGame(true);//휠위젯을 가린다
			//		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UGrabComponent::UIHider, 0.5f, false);
			//		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			//			{
			//				player->WheelUI->SetHiddenInGame(true);//휠위젯을 가린다
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
	//	//물체를 손에서 분리하고 물리 능력을 활성화한다
	//	grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//그 자리에서 연결 끊기
	//	grabbedObject->boxComp->SetSimulatePhysics(true);

	//	//물체의 던지는 방향에 따른 힘(선형, 회전력)을 가한다
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
			if (AWaterGunActor* pickObj = Cast<AWaterGunActor>(hitInfo.GetActor()))//만약 물총에서 트리거를 놓았다면
			{
				if(player->bHasMuzzle)
				{
					UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetComponent()->GetName());
					if (grabbedObject != nullptr)
					{
						grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//그 자리에서 연결 끊기
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
								// 코드 구현
								player->MenuUI->SetHiddenInGame(true);
								player->MenuUI->SetRelativeLocation(FVector(40, 0, 100));
								// TimerHandle 초기화
								GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
							}), GravityTime, false);	// 반복하려면 false를 true로 변경

						//놓았을 떄 초기화
						isRot = false;
						//firstHandRot = player->leftHand->GetRelativeRotation();
					}
					break;
				}
			}
			//else if (hitInfo.GetComponent()->GetName() == "MuzzleHolder")//만약 머즐홀더에서 트리거를 놓았다면
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("MuzzleHolder release"));
			//	if (player->bHasMuzzle)
			//	{
			//		player->WheelUI->SetHiddenInGame(false);//휠위젯을 보여준다
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
	player->WheelUI->SetHiddenInGame(true);//휠위젯을 가린다
	UE_LOG(LogTemp, Warning, TEXT("UI Hide"));
}

void UGrabComponent::changeHiddenWheelUI()
{
//	if (player->WheelUI != nullptr && player->WheelUI->bHiddenInGame) player->WheelUI->SetHiddenInGame(false);//if hidden in game is true
//	else if(player->WheelUI != nullptr && !player->WheelUI->bHiddenInGame) player->WheelUI->SetHiddenInGame(true);
	
}

