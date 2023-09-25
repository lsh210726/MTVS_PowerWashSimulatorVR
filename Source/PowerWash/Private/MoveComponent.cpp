// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Components/TextRenderComponent.h"
#include "VRCharacter.h"
#include "MotionControllerComponent.h"
#include "DrawDebugHelpers.h"
#include "BallActor.h"
#include <Components/CapsuleComponent.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraDataInterfaceArrayFunctionLibrary.h>
#include "FloorIndicatorActor.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRCharacter>();
	FTimerHandle massTimer;
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]()
	{
		if (player->ball != nullptr)
		{
			//player->ball->meshComp->SetSimulatePhysics(true);
			//myMass = player->ball->meshComp->GetMass();
			player->ball->meshComp->SetSimulatePhysics(false);
		}
	}));
	
	//�ڷ���Ʈ �� ����
	if(indicator_BP!=nullptr)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		indicatorActor = GetWorld()->SpawnActor<AFloorIndicatorActor>(indicator_BP, FVector::ZeroVector, FRotator::ZeroRotator, params);
		indicatorActor->floorIndicator->SetVisibility(false);
	}
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if(bIsShowLine) 
	{
		DrawTrajectory(throwDirection, throwPower, myMass);
		//DrawBazierCurve();
	}
	//�ݻ� ���� �׸���
	//FVector shoot = FRotationMatrix(player->leftMotionController->GetComponentRotation()).GetUnitAxis(EAxis::X);
	//DrawReflectionVector(FVector::ForwardVector, 300);
}

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")


void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
 	enhancedInputComponent->BindAction(inputActions[0], ETriggerEvent::Triggered, this, &UMoveComponent::Move);
 	enhancedInputComponent->BindAction(inputActions[0], ETriggerEvent::Completed, this, &UMoveComponent::Move);
 	enhancedInputComponent->BindAction(inputActions[1], ETriggerEvent::Triggered, this, &UMoveComponent::Rotate);
 	enhancedInputComponent->BindAction(inputActions[1], ETriggerEvent::Completed, this, &UMoveComponent::Rotate);
 //	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UMoveComponent::RighttTriggerDown);
	//enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UMoveComponent::RightTriggerUp);
}

void UMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D controllerInput = value.Get<FVector2D>();
	//player->leftLog->SetText(FText::FromString(FString::Printf(TEXT("x: %.2f\r\ny: %.2f"), controllerInput.X, controllerInput.Y)));

	FVector forwardVec = FRotationMatrix(player->pc->GetControlRotation()).GetUnitAxis(EAxis::X);
	FVector rightVec = FRotationMatrix(player->pc->GetControlRotation()).GetUnitAxis(EAxis::Y);

	player->AddMovementInput(forwardVec, controllerInput.X);
	player->AddMovementInput(rightVec, controllerInput.Y);
}

void UMoveComponent::Rotate(const FInputActionValue& value)
{
	FVector2D rightConInput = value.Get<FVector2D>();

	player->rightLog->SetText(FText::FromString(FString::Printf(TEXT("%.2f\r\n%.2f"), rightConInput.X, rightConInput.Y)));

	if (player->pc != nullptr)
	{
		player->pc->AddYawInput(rightConInput.X);
		player->pc->AddPitchInput(rightConInput.Y);
	}
}

void UMoveComponent::RighttTriggerDown()
{
	bIsShowLine = true;

	if (player->ball != nullptr)
	{
		player->ball->meshComp->SetSimulatePhysics(true);
		player->ball->meshComp->SetEnableGravity(false);
		FVector shootDir = player->leftMotionController->GetComponentTransform().TransformVector(FVector::ForwardVector);
		player->ball->meshComp->AddImpulse(shootDir * 500);
	}
}

void UMoveComponent::RightTriggerUp()
{
	bIsShowLine = false;

	if (player->ball != nullptr)
	{
		player->ball->meshComp->SetSimulatePhysics(true);

		FVector relativeDir = player->leftMotionController->GetComponentTransform().TransformVector(throwDirection);
		player->ball->meshComp->AddImpulse(relativeDir * throwPower);
	}

	float duration = 0.50f;
	player->pc->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, duration, FLinearColor::Black, true);

	FTimerHandle moveHandle;
	GetWorld()->GetTimerManager().SetTimer(moveHandle, FTimerDelegate::CreateLambda([&]()
		{
			//������ġ�� ���� �̵���Ų��
			FVector targetLoc = linePositions[linePositions.Num() - 1];
			targetLoc.Z += player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
			player->SetActorLocation(targetLoc, true);

			//Ʈ���Ÿ� ������ ������ �迭 ���� �ʱ�ȭ�Ѵ� > ���� ���� �ȵǰ�
			TArray<FVector> tempArr = { FVector::ZeroVector };
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(player->lineFx, FName("PointArray"), tempArr);

			//�ڷ���Ʈ ���� �Ⱥ��̰� �Ѵ�
			if (indicatorActor)
			{
				indicatorActor->floorIndicator->SetVisibility(false);
			}
		}), duration, false);
	
}

void UMoveComponent::DrawTrajectory(FVector dir, float power, float mass)
{
	linePositions.Empty();

	UWorld* world = GetWorld();
	FVector startLoc = player->leftMotionController->GetComponentLocation();
	FVector direction = player->leftMotionController->GetComponentTransform().TransformVector(dir);//���� ���밪 dir�� ��Ʈ�ѷ� ��밪���� ��ȯ

	for (int32 i = 0; i < segment; i++)
	{
		float elapsedTime = interval * i;//����ð�
		float gravity = world->GetGravityZ();//�߷�
		FVector downForce = FVector(0,0,0.5f * gravity * elapsedTime * elapsedTime * elapsedTime * mass * mass);

		FVector endLoc = startLoc + direction * power * elapsedTime + downForce;

		//���� ������ �� ���� ������ �浹üũ
		FHitResult hitInfo;
		if (i > 0 && world->LineTraceSingleByChannel(hitInfo, linePositions[i - 1], endLoc, ECC_Visibility))
		{
			endLoc = hitInfo.ImpactPoint;//���� �浹�� �ִٸ� �浹����ġ�� ���������� ����
			linePositions.Add(endLoc);//������ġ �߰�
			break;
		}

		linePositions.Add(endLoc);
	}

	//for (int i = 0; i < linePositions.Num() - 1; i++)
	//{
	//	DrawDebugLine(world, linePositions[i], linePositions[i + 1], FColor::Red, false, 0, 0, 2);//�� ������ ���̻��̿� �� �׸���

	//}
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(player->lineFx, FName("PointArray"), linePositions);
	//�ٴ� �� ǥ��
	if (indicatorActor != nullptr)
	{
		indicatorActor->SetActorLocation(linePositions[linePositions.Num() - 1]);//���� ���� ���κп� �ε������� ��ġ
		indicatorActor->floorIndicator->SetVisibility(true); //�ε������� �Ⱥ��̰��ߴ��� �ٽ� ���̰�
	}
}

void UMoveComponent::DrawBazierCurve()
{
	FVector startLoc = player->leftMotionController->GetComponentLocation();//���� ��ġ
	FVector relativeDir = player->leftMotionController->GetComponentTransform().TransformVector(throwDirection);//�����ǥ�� ��ȯ
	FHitResult hitInfo;

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, startLoc + relativeDir * throwPower, ECC_Visibility))
	{
		float heightDist = FMath::Abs(hitInfo.ImpactPoint.Z - player->leftMotionController->GetComponentLocation().Z);//����Ʈ �������� ���� ����&���밪
		FVector point1 = startLoc;//������ġ
		FVector point2 = hitInfo.ImpactPoint + FVector::UpVector * heightDist *1.5f;
		FVector point3 = hitInfo.ImpactPoint;
		float alpha = 0;
		linePositions.Empty();//�迭 �ʱ�ȭ

		for (int32 i = 0; i < 11; ++i)
		{
			FVector lineStart = FMath::Lerp<FVector>(point1, point2, alpha);
			FVector lineEnd = FMath::Lerp<FVector>(point2, point3, alpha);
			FVector linePoint = FMath::Lerp<FVector>(lineStart, lineEnd, alpha);
			alpha += 0.1f;//���� ���������� ���İ� ���ݾ� ����

			linePositions.Add(linePoint);//������� �� �迭�� ����
		}

		for (int32 i = 0; i < linePositions.Num()-1; i++)
		{
			DrawDebugLine(GetWorld(), linePositions[i], linePositions[i + 1], FColor::Green, false, 0, 0, 2);
		}
	}
}

void UMoveComponent::DrawReflectionVector(FVector dir, float entireDist)
{
	float remainedDist = entireDist;
	FVector shootDir = player->leftMotionController->GetComponentTransform().TransformVector(dir);
	FVector startLoc = player->leftMotionController->GetComponentLocation();
	UWorld* world = GetWorld();
	FHitResult hitInfo;
	TArray<FVector> points;
	points.Add(startLoc);

	while (true)
	{
		if (remainedDist > 0 /*����� �� ������ ������ ��� �ݻ簢 ����� �ϸ� �ȵ�*/&& world->LineTraceSingleByChannel(hitInfo, startLoc /*������*/, startLoc + shootDir * remainedDist /*������*/, ECC_Visibility))
		{
			//�ε����� �ݻ簢 ���, �ݻ簢�� �߻簢���� ����
			float projection = FVector::DotProduct/*����*/(shootDir * -1, hitInfo.ImpactNormal);
			startLoc = hitInfo.ImpactPoint;
			FVector reflectionVec = shootDir + 2 * hitInfo.ImpactNormal * projection;
			shootDir = reflectionVec; //���ΰ��� �ݻ簢���� ����
			remainedDist = remainedDist = FMath::Max(hitInfo.Distance,0/*Ȥ�ø𸣴ϱ� �ּҰ� ����*/); //���� �Ÿ� ����
			points.Add(hitInfo.ImpactPoint);
		}
		else
		{
			//�Ⱥε����� �����ϱ�
			FVector endLoc = startLoc + shootDir * remainedDist;
			points.Add(endLoc);
			break;
		}
	}

	for (int32 i = 0; i < points.Num() - 1; i++)
	{
		DrawDebugLine(world, points[i], points[i + 1], FColor::Red, false, 0, 0, 2);
	}
}

