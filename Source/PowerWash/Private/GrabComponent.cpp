// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "VRCharacter.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include "MotionControllerComponent.h"
#include "PickUpActor.h"
#include <Components/TextRenderComponent.h>
#include <Components/BoxComponent.h>

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
	if (grabbedObject != nullptr)
	{
		deltaLoc = player->leftMotionController->GetComponentLocation() - prevLoc;//위치변화값
		prevLoc = player->leftMotionController->GetComponentLocation();//이전위치값 갱신

		deltaRot = player->leftMotionController->GetComponentQuat() - prevRot.Inverse();
		prevRot = player->leftMotionController->GetComponentQuat();
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
	if (grabbedObject != nullptr)
	{
		return;
	}


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

	//overlap
	TArray<FOverlapResult> hitInfos;
	FVector startLoc = player->leftHand->GetComponentLocation();
	if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(30)))
	{
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			if (APickUpActor* pickObj = Cast<APickUpActor>(hitInfo.GetActor()))
			{
				pickObj->Grabbed(player->leftHand);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Grab!"));
				grabbedObject = pickObj;
				break;//만약 가장 가까운 것 하나만 잡고 싶은 경우 여기에 break을 걸 것
			}
			UE_LOG(LogTemp, Warning, TEXT("Grab Object : %s"), *hitInfo.GetComponent()->GetName());

		}

	}
	DrawDebugSphere(GetWorld(), startLoc, 30, 12, FColor::Red, false, 1, 0, 0.1f);
}

void UGrabComponent::ReleaseObject()
{
	if(grabbedObject!=nullptr)
	{
		//물체를 손에서 분리하고 물리 능력을 활성화한다
		grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);//그 자리에서 연결 끊기
		grabbedObject->boxComp->SetSimulatePhysics(true);

		//물체의 던지는 방향에 따른 힘(선형, 회전력)을 가한다
		if (!deltaLoc.IsNearlyZero())
		{
			grabbedObject->boxComp->AddImpulse(deltaLoc.GetSafeNormal() * throwPower);
			grabbedObject->boxComp->AddTorqueInRadians(deltaRot.GetRotationAxis() * rotSpeed);
		}
		grabbedObject = nullptr;
	}
}

void UGrabComponent::leftHandMove(const struct FInputActionValue& value)
{
	FVector direction = value.Get<FVector>();
	player->leftMotionController->SetRelativeLocation(player->leftMotionController->GetRelativeLocation() + direction.GetSafeNormal());
}

