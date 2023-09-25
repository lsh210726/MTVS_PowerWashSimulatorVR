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
	
	//텔레포트 링 생성
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
	//반사 벡터 그리기
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
			//최종위치로 나를 이동시킨다
			FVector targetLoc = linePositions[linePositions.Num() - 1];
			targetLoc.Z += player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
			player->SetActorLocation(targetLoc, true);

			//트리거를 놓으면 라인의 배열 값을 초기화한다 > 선이 생성 안되게
			TArray<FVector> tempArr = { FVector::ZeroVector };
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(player->lineFx, FName("PointArray"), tempArr);

			//텔레포트 링을 안보이게 한다
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
	FVector direction = player->leftMotionController->GetComponentTransform().TransformVector(dir);//월드 절대값 dir을 컨트롤러 상대값으로 변환

	for (int32 i = 0; i < segment; i++)
	{
		float elapsedTime = interval * i;//경과시간
		float gravity = world->GetGravityZ();//중력
		FVector downForce = FVector(0,0,0.5f * gravity * elapsedTime * elapsedTime * elapsedTime * mass * mass);

		FVector endLoc = startLoc + direction * power * elapsedTime + downForce;

		//이전 지점과 새 지점 사이의 충돌체크
		FHitResult hitInfo;
		if (i > 0 && world->LineTraceSingleByChannel(hitInfo, linePositions[i - 1], endLoc, ECC_Visibility))
		{
			endLoc = hitInfo.ImpactPoint;//만약 충돌이 있다면 충돌한위치를 끝지점으로 설정
			linePositions.Add(endLoc);//최종위치 추가
			break;
		}

		linePositions.Add(endLoc);
	}

	//for (int i = 0; i < linePositions.Num() - 1; i++)
	//{
	//	DrawDebugLine(world, linePositions[i], linePositions[i + 1], FColor::Red, false, 0, 0, 2);//각 지점들 사이사이에 선 그리기

	//}
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(player->lineFx, FName("PointArray"), linePositions);
	//바닥 링 표시
	if (indicatorActor != nullptr)
	{
		indicatorActor->SetActorLocation(linePositions[linePositions.Num() - 1]);//선의 제일 끝부분에 인디케이터 위치
		indicatorActor->floorIndicator->SetVisibility(true); //인디케이터 안보이게했던거 다시 보이게
	}
}

void UMoveComponent::DrawBazierCurve()
{
	FVector startLoc = player->leftMotionController->GetComponentLocation();//시작 위치
	FVector relativeDir = player->leftMotionController->GetComponentTransform().TransformVector(throwDirection);//상대좌표로 변환
	FHitResult hitInfo;

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, startLoc + relativeDir * throwPower, ECC_Visibility))
	{
		float heightDist = FMath::Abs(hitInfo.ImpactPoint.Z - player->leftMotionController->GetComponentLocation().Z);//임팩트 포인터의 높이 설정&절대값
		FVector point1 = startLoc;//시작위치
		FVector point2 = hitInfo.ImpactPoint + FVector::UpVector * heightDist *1.5f;
		FVector point3 = hitInfo.ImpactPoint;
		float alpha = 0;
		linePositions.Empty();//배열 초기화

		for (int32 i = 0; i < 11; ++i)
		{
			FVector lineStart = FMath::Lerp<FVector>(point1, point2, alpha);
			FVector lineEnd = FMath::Lerp<FVector>(point2, point3, alpha);
			FVector linePoint = FMath::Lerp<FVector>(lineStart, lineEnd, alpha);
			alpha += 0.1f;//포문 돌릴때마다 알파값 조금씩 증가

			linePositions.Add(linePoint);//만들어진 값 배열에 저장
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
		if (remainedDist > 0 /*닿았을 떄 끝나는 지점일 경우 반사각 계산을 하며 안됨*/&& world->LineTraceSingleByChannel(hitInfo, startLoc /*시작점*/, startLoc + shootDir * remainedDist /*종료점*/, ECC_Visibility))
		{
			//부딛히면 반사각 계산, 반사각을 발사각으로 갱신
			float projection = FVector::DotProduct/*내적*/(shootDir * -1, hitInfo.ImpactNormal);
			startLoc = hitInfo.ImpactPoint;
			FVector reflectionVec = shootDir + 2 * hitInfo.ImpactNormal * projection;
			shootDir = reflectionVec; //새로계산된 반사각으로 갱신
			remainedDist = remainedDist = FMath::Max(hitInfo.Distance,0/*혹시모르니까 최소값 설정*/); //남은 거리 갱신
			points.Add(hitInfo.ImpactPoint);
		}
		else
		{
			//안부딛히면 계산안하기
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

