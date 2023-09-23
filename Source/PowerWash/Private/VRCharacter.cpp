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
#include "HandAnimComponent.h"
#include "DecalCompoenent.h"
#include "WaterGunActor.h"
#include "Components/DecalComponent.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h>
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h>
#include "Components/WidgetInteractionComponent.h"
#include "WidgetPointerComponent.h"
#include "RenderTargetProcess.h"
#include "Components/BoxComponent.h"



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

	rightWidgetPointer = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Right Widget Pointer"));
	rightWidgetPointer->SetupAttachment(rightHand);
	rightWidgetPointer->SetRelativeRotation(FRotator(0, 90, 0));

	lineFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Line Effect"));//�ڷ���Ʈ ��ġ���� �̾����� �� ���̾ư���
	lineFx->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	//������Ʈ ����
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	handAnimComp = CreateDefaultSubobject<UHandAnimComponent>(TEXT("Hand Anim Component"));
	shootComp = CreateDefaultSubobject<UShootComponent>(TEXT("Shoot Component"));

	//LMH RenderTarget component �߰�
	//DrawComp = CreateDefaultSubobject<URenderTargetProcess>(TEXT("RenderTargerProcess Component"));

	//���� ������ ������Ʈ �߰�
	widgetPointerComp = CreateDefaultSubobject<UWidgetPointerComponent>(TEXT("Widget Pointer Component"));

	muzzleHolder = CreateDefaultSubobject<UBoxComponent>(TEXT("MuzzleHolder"));
	muzzleHolder->SetupAttachment(RootComponent);
	muzzleHolder->SetWorldScale3D(FVector(0.5));
	muzzleHolder->SetRelativeLocation(FVector(40, -50, 0));
	muzzleHolder->SetCollisionProfileName(FName("OverlapAllDynamic"));

	showMeUIPlace = CreateDefaultSubobject<UBoxComponent>(TEXT("ShowMeUIPlace"));//왼손이 이곳에 오버랩되면 UI가 나옴
	showMeUIPlace->SetupAttachment(RootComponent);
	showMeUIPlace->SetWorldScale3D(FVector(0.25,0.5,0.25));
	showMeUIPlace->SetRelativeLocation(FVector(30, 0, -20));
	//showMeUIPlace->SetCollisionProfileName(FName("OverlapAllDynamic"));
	//showMeUIPlace->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);//몸에 오버랩 안되게

	leftHandOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandOverlapBox"));//손이 이상하게 오버랩이 안되서 박스를 붙이기
	leftHandOverlapBox->SetupAttachment(leftMotionController);
	leftHandOverlapBox->SetWorldScale3D(FVector(0.2));
	leftHandOverlapBox->SetRelativeLocation(FVector(0, 0, -10));
	leftHandOverlapBox->SetCollisionProfileName(FName("OverlapAllDynamic"));

	GunHolder = CreateDefaultSubobject<UBoxComponent>(TEXT("GunHolder"));
	GunHolder->SetupAttachment(RootComponent);
	GunHolder->SetWorldScale3D(FVector(0.5));
	GunHolder->SetRelativeLocation(FVector(40, 50, 0));
	GunHolder->SetCollisionProfileName(FName("OverlapAllDynamic"));

#pragma region key bind


	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(TEXT("/Game/LSH_WorkSpace/Inputs/IMC_VRInput.IMC_VRInput"));
	if (tempIMC.Succeeded())
	{
		imc_VRmap = tempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_Move(TEXT("/Game/LSH_WorkSpace/Inputs/IA_PlayerMove.IA_PlayerMove"));
	if (tempIA_Move.Succeeded())
	{
		inputActions.Add(tempIA_Move.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_Look(TEXT("/Game/LSH_WorkSpace/Inputs/IA_PlayerRotation.IA_PlayerRotation"));
	if (tempIA_Look.Succeeded())
	{
		inputActions.Add(tempIA_Look.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_RT(TEXT("/Game/LSH_WorkSpace/Inputs/IA_RightTrigger.IA_RightTrigger"));
	if (tempIA_RT.Succeeded())
	{
		inputActions.Add(tempIA_RT.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_LG(TEXT("/Game/LSH_WorkSpace/Inputs/IA_LeftGrip.IA_LeftGrip"));
	if (tempIA_LG.Succeeded())
	{
		inputActions.Add(tempIA_LG.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_RHM(TEXT("/Game/LSH_WorkSpace/Inputs/IA_LeftHandMove.IA_LeftHandMove"));
	if (tempIA_RHM.Succeeded())
	{
		inputActions.Add(tempIA_RHM.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_RTV(TEXT("/Game/LSH_WorkSpace/Inputs/IA_RightTrigger_value.IA_RightTrigger_value"));
	if (tempIA_RTV.Succeeded())
	{
		inputActions.Add(tempIA_RTV.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_LGV(TEXT("/Game/LSH_WorkSpace/Inputs/IA_LeftGrip_value.IA_LeftGrip_value"));
	if (tempIA_LGV.Succeeded())
	{
		inputActions.Add(tempIA_LGV.Object);
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_RWW(TEXT("/Game/LSH_WorkSpace/Inputs/IA_RightHandMove.IA_RightHandMove"));
	if (tempIA_RWW.Succeeded()) inputActions.Add(tempIA_RWW.Object);
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_LBB(TEXT("/Game/LSH_WorkSpace/Inputs/IA_LeftBtn.IA_LeftBtn"));
	if (tempIA_LBB.Succeeded()) inputActions.Add(tempIA_LBB.Object);
	ConstructorHelpers::FObjectFinder<UInputAction> tempIA_RBB(TEXT("/Game/LSH_WorkSpace/Inputs/IA_RightBtn.IA_RightBtn"));
	if (tempIA_RBB.Succeeded()) inputActions.Add(tempIA_RBB.Object);
#pragma endregion
}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// ��� �α� �ʱ�ȭ
	leftLog->SetText(FText::FromString("Left Log..."));
	rightLog->SetText(FText::FromString("Right Log..."));

	// �Ӹ� ��� ������ ����
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
	//�׽�Ʈ �� ���� üũ
	if(bIsTesting&&myBall_bp!=nullptr)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ball = GetWorld()->SpawnActor<ABallActor>(myBall_bp, leftMotionController->GetComponentLocation(), FRotator::ZeroRotator, param);
		ball->meshComp->SetSimulatePhysics(false);
		ball->AttachToComponent(leftMotionController, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	//�� ���� �ִϸ��̼� �ν��Ͻ� ��������
	leftHandAnim = Cast<UHandAnimInstance>(leftHand->GetAnimInstance());
	rightHandAnim = Cast<UHandAnimInstance>(rightHand->GetAnimInstance());

	if (leftHandAnim != nullptr) leftHandAnim->isLeft = true;

	if(rightHandAnim) rightHandAnim->PoseAlphaGrasp = 1.0f;

	showMeUIPlace->OnComponentBeginOverlap.AddDynamic(this, &AVRCharacter::OnComponentBeginOverlap);
	showMeUIPlace->OnComponentEndOverlap.AddDynamic(this, &AVRCharacter::OnOverlapEnd);
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
		handAnimComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
		shootComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
		widgetPointerComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);

#pragma region inputTest
		//������ ��Ʈ�ѷ� �Է� �׽�Ʈ ���ε�
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
	}
}

void AVRCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UIOnOff();
	UE_LOG(LogTemp, Warning, TEXT("Overlap Object : %s"),*OtherComp->GetName());

}

void AVRCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UIOnOff();
}

void AVRCharacter::UIOnOff_Implementation()//오버랩될때 호출될 함수
{
	UE_LOG(LogTemp, Warning, TEXT("UI Show"));
	//}
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

	//����ڰ� �ٶ󺸰� �ִ� ������ �������� �ٽ� ����(ȸ��, ��ġ)
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
