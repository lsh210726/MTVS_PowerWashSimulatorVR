// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"
#include "Components/TextRenderComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MoveComponent.h"
#include "BallActor.h"
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "GrabComponent.h"
#include "DecalCompoenent.h"
#include "Components/DecalComponent.h"


// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	hmdCam = CreateDefaultSubobject<UCameraComponent>(TEXT("HMD Camera"));
	hmdCam->SetupAttachment(RootComponent);

	hmdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD Mesh"));
	hmdMesh->SetupAttachment(hmdCam);

	leftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	leftMotionController->SetupAttachment(RootComponent);
	leftMotionController->SetTrackingMotionSource(FName("Left"));

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand Mesh"));
	leftHand->SetupAttachment(leftMotionController);

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log"));
	leftLog->SetupAttachment(leftMotionController);
	leftLog->SetRelativeLocation(FVector(0, 0, 15));
	leftLog->SetRelativeRotation(FRotator(0, 180, 0));
	leftLog->SetWorldSize(10);
	leftLog->SetTextRenderColor(FColor(255, 255, 0, 255));
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextCenter);

	rightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));
	rightMotionController->SetupAttachment(RootComponent);
	rightMotionController->SetTrackingMotionSource(FName("Right"));

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand Mesh"));
	rightHand->SetupAttachment(rightMotionController);

	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log"));
	rightLog->SetupAttachment(rightMotionController);
	rightLog->SetRelativeLocation(FVector(0, 0, 15));
	rightLog->SetRelativeRotation(FRotator(0, -90, 0));
	rightLog->SetWorldSize(10);
	rightLog->SetTextRenderColor(FColor(255, 255, 0, 255));
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextCenter);

	lineFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Line Effect"));//텔레포트 위치까지 이어지는 선 나이아가라
	lineFx->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	//컴포넌트 패턴
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));

	//LMH decal component 추가
	decalComp = CreateDefaultSubobject<UDecalCompoenent>(TEXT("Decal Component"));
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 양손 로그 초기화
	leftLog->SetText(FText::FromString("Left Log..."));
	rightLog->SetText(FText::FromString("Right Log..."));

	// 머리 장비 기준점 설정
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	pc = GetController<APlayerController>();

	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSys != nullptr)
		{
			subSys->AddMappingContext(imc_VRmap, 0);
		}
	}
	//테스트 볼 생성 체크
	if(bIsTesting&&myBall_bp!=nullptr)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ball = GetWorld()->SpawnActor<ABallActor>(myBall_bp, leftMotionController->GetComponentLocation(), FRotator::ZeroRotator, param);
		ball->meshComp->SetSimulatePhysics(false);
		ball->AttachToComponent(leftMotionController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		moveComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
		grabComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);

#pragma region inputTest
		//오른손 컨트롤러 입력 테스트 바인딩
		//enhancedInputComponent->BindAction(inputActions[0], ETriggerEvent::Started, this, &AVRCharacter::RightTriggerDown);
		//enhancedInputComponent->BindAction(inputActions[0], ETriggerEvent::Completed, this, &AVRCharacter::RightTriggerUp);
		//enhancedInputComponent->BindAction(inputActions[1], ETriggerEvent::Triggered, this, &AVRCharacter::RightTriggerValue);
		//enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &AVRCharacter::RightGripDown);
		//enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &AVRCharacter::RightGripUp);
		//enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Started, this, &AVRCharacter::RightThumbstickDown);
		//enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Completed, this, &AVRCharacter::RightThumbstickUp);
		//enhancedInputComponent->BindAction(inputActions[4], ETriggerEvent::Started, this, &AVRCharacter::RightThumbstickTouch);
		//enhancedInputComponent->BindAction(inputActions[5], ETriggerEvent::Triggered, this, &AVRCharacter::RightThumbstickAxis);
		//enhancedInputComponent->BindAction(inputActions[6], ETriggerEvent::Started, this, &AVRCharacter::RightADown);
		//enhancedInputComponent->BindAction(inputActions[6], ETriggerEvent::Completed, this, &AVRCharacter::RightAUP);
		//enhancedInputComponent->BindAction(inputActions[7], ETriggerEvent::Started, this, &AVRCharacter::RightATouch);
		//enhancedInputComponent->BindAction(inputActions[8], ETriggerEvent::Started, this, &AVRCharacter::RightBDown);
		//enhancedInputComponent->BindAction(inputActions[8], ETriggerEvent::Completed, this, &AVRCharacter::RightBUp);
		//enhancedInputComponent->BindAction(inputActions[9], ETriggerEvent::Started, this, &AVRCharacter::RightBTouch);
#pragma endregion inputTest

		//LMH Decal component 추가
		decalComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
	}
}

void AVRCharacter::RightTriggerDown()
{
	rightLog->SetText(FText::FromString("Right Trigger Down!"));
}

void AVRCharacter::RightTriggerUp()
{
	rightLog->SetText(FText::FromString("Right Trigger Up!"));

}

void AVRCharacter::RightTriggerValue(const struct FInputActionValue& value)
{
	float input = value.Get<float>();
	FString inputLog = FString::Printf(TEXT("Trigger pressed %.2f percent"), input);
	rightLog->SetText(FText::FromString(inputLog));
}

void AVRCharacter::RightGripDown()
{
	rightLog->SetText(FText::FromString("Right Grip Down!"));

}

void AVRCharacter::RightGripUp()
{
	rightLog->SetText(FText::FromString("Right Grip Up!"));
}

void AVRCharacter::RightThumbstickDown()
{
	rightLog->SetText(FText::FromString("Right Thumbstick Down!"));

}

void AVRCharacter::RightThumbstickUp()
{
	rightLog->SetText(FText::FromString("Right Thumbstick Up!"));

}

void AVRCharacter::RightThumbstickTouch()
{
	rightLog->SetText(FText::FromString("Right Thumbstick Touched!"));

}

void AVRCharacter::RightThumbstickAxis(const struct FInputActionValue& value)
{
	FVector input = value.Get<FVector>();
	FString inputLog = FString::Printf(TEXT("ThumbStick\r\nx: %.2f\r\ny: %.2f"), input.X,input.Y);
	rightLog->SetText(FText::FromString(inputLog));
}

void AVRCharacter::RightADown()
{
	rightLog->SetText(FText::FromString("Right A Button Down!"));

	//사용자가 바라보고 있는 방향을 정면으로 다시 정렬(회전, 위치)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVRCharacter::RightAUP()
{
	rightLog->SetText(FText::FromString("Right A Button Up!"));

}

void AVRCharacter::RightATouch()
{
	rightLog->SetText(FText::FromString("Right A Button Touched!"));

}

void AVRCharacter::RightBDown()
{
	rightLog->SetText(FText::FromString("Right B Button Down!"));

}

void AVRCharacter::RightBUp()
{
	rightLog->SetText(FText::FromString("Right B Button Up!"));

}

void AVRCharacter::RightBTouch()
{
	rightLog->SetText(FText::FromString("Right B Button Touched!"));

}
