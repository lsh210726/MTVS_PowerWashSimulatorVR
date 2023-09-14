// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalCompoenent.h"
#include "Camera/PlayerCameraManager.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "VRCharacter.h"
#include <Components/PrimitiveComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/DecalComponent.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstance.h>

//enum class MI_Red_Name : uint8
//{
//	 UMETA(DisplayName = "Value 1"),
//	Value2 UMETA(DisplayName = "Value 2"),
//	Value3 UMETA(DisplayName = "Value 3")
//};

// Sets default values for this component's properties
UDecalCompoenent::UDecalCompoenent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	  
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Red_1(TEXT("/Game/LMH/Decal2/MI_RedDecal_1.MI_RedDecal_1"));
	if (temp_Red_1.Succeeded()) MI_Reds.Add(temp_Red_1.Object);
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Red_2(TEXT("/Game/LMH/Decal2/MI_RedDecal_2.MI_RedDecal_2"));
	if (temp_Red_2.Succeeded()) MI_Reds.Add(Cast<UMaterialInstance>(temp_Red_2.Object));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Red_3(TEXT("/Game/LMH/Decal2/MI_RedDecal_3.MI_RedDecal_3"));
	if (temp_Red_3.Succeeded()) MI_Reds.Add(Cast<UMaterialInstance>(temp_Red_3.Object));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Red_4(TEXT("/Game/LMH/Decal2/MI_RedDecal_4.MI_RedDecal_4"));
	if (temp_Red_4.Succeeded()) MI_Reds.Add(Cast<UMaterialInstance>(temp_Red_4.Object));
	

	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_1(TEXT("/Game/LMH/Decal2/MI_YellowDecal_1.MI_YellowDecal_1"));
	if (temp_Yellow_1.Succeeded()) MI_Yellows.Add(temp_Yellow_1.Object);
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_2(TEXT("/Game/LMH/Decal2/MI_YellowDecal_1.MI_YellowDecal_2"));
	if (temp_Yellow_2.Succeeded()) MI_Yellows.Add(Cast<UMaterialInstance>(temp_Yellow_2.Object));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_3(TEXT("/Game/LMH/Decal2/MI_YellowDecal_1.MI_YellowDecal_3"));
	if (temp_Yellow_3.Succeeded()) MI_Yellows.Add(Cast<UMaterialInstance>(temp_Yellow_3.Object));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_4(TEXT("/Game/LMH/Decal2/MI_YellowDecal_1.MI_YellowDecal_4"));
	if (temp_Yellow_4.Succeeded()) MI_Yellows.Add(Cast<UMaterialInstance>(temp_Yellow_4.Object));
}


// Called when the game starts
void UDecalCompoenent::BeginPlay()
{
	Super::BeginPlay();

	player = GetOwner<AVRCharacter>();
	if (player == nullptr) return;

}


// Called every frame
void UDecalCompoenent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (player == nullptr) return;

	//if (player->handstate != EHandState::LMH) return;
	/*pc = Cast<APlayerController>(player->GetController());
	if (pc == nullptr) return;*/

	//임시로 카메라 매니저에서 ray
	if (IsDrawing) {
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
		if (CameraManager)
		{
			FVector dir = CameraManager->GetTransformComponent()->GetForwardVector();
			FVector start = CameraManager->GetTransformComponent()->GetComponentLocation();
			FVector end = dir * 3000 + start;

			//ray cast

			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params))
			{
				FVector Loc = HitResult.Location;
				FVector ImpactPoint = HitResult.ImpactPoint;
				FVector normal = HitResult.Normal;
				FRotator rot = UKismetMathLibrary::MakeRotFromX(normal);
				FVector DecalSize = FVector(2, 2, 2);

				UPrimitiveComponent* hitComp = HitResult.GetComponent();

				//얼룩 생성모드
				if (IsPainting)
				{
					//생성 얼룩 색깔 parameter로 주기
					DoPainting(Loc, hitComp, rot, DecalSize,Color);	
				}
				else {
					/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), DecalComps.Num()), true, FVector2D(1, 1));*/

					//데칼 갯수가 1보다 많으면 for문 시작, 데칼 지우기				
					ErasePainting(Loc, DecalSize,Color);
					
				}
				DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1.f, 0.f, 1.f);
			}
		}
	}
}
void UDecalCompoenent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UDecalCompoenent::LeftTriggerDown);
	enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UDecalCompoenent::LeftTriggerUp);
}
void UDecalCompoenent::LeftTriggerDown()
{
	/*if (player->handstate == EHandState::LMH) {
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("MouseLeftTriggerDown")), true, FVector2D(1, 1));
		IsDrawing = true;
	}*/
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("MouseLeftTriggerDown")), true, FVector2D(1, 1));
	IsDrawing = true;
}

void UDecalCompoenent::LeftTriggerUp()
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("MouseLeftTriggerUp")), true, FVector2D(1, 1));
	IsDrawing=false;
}


void UDecalCompoenent::DoPainting(FVector Loc, UPrimitiveComponent* hitComp, FRotator rot, FVector DecalSize, EPaintColor pcolor)
{
	UMaterialInstance* MI_Color=nullptr;

	switch (pcolor)
	{
	case EPaintColor::Red:
		MI_Color = MI_Reds[0];
		break;
	case EPaintColor::Yellow:
		MI_Color = MI_Yellows[0];
		break;
	case EPaintColor::Green:
		MI_Color = MI_Greens[0];
		break;
	default:
		break;
	}
	if (MI_Color == nullptr) return;

	UDecalComponent* decalcomp = UGameplayStatics::SpawnDecalAttached(MI_Color, DecalSize, hitComp, FName("None"), Loc, rot, EAttachLocation::KeepWorldPosition);

	if (decalcomp)
	{
		decalcomp->SetFadeScreenSize(0);
		DecalComps.Add(decalcomp);
		/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), DecalComps.Num()), true, FVector2D(1, 1));*/
	}
}

void UDecalCompoenent::ErasePainting(FVector Loc,FVector DecalSize,EPaintColor pcolor)
{
	if (DecalComps.Num() < 2) return;
	for (UDecalComponent* decal : DecalComps)
	{
		if (decal) {
			double dist = FVector::Distance(Loc, decal->GetComponentLocation());
			if (dist && dist <= DecalSize.X )
			{
				if (IsValid(decal->GetDecalMaterial()))
				{
					FString dName = decal->GetDecalMaterial()->GetName();
					int res = dName[dName.Len() - 1] - '0';

					switch (pcolor) {
					case EPaintColor::Red:
						switch (res)
						{
						case 1:
							/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), res), true, FVector2D(1, 1));*/
							decal->SetDecalMaterial(MI_Reds[1]);
							break;
						case 2:
							decal->SetDecalMaterial(MI_Reds[2]);
							break;
						case 3:
							decal->SetDecalMaterial(MI_Reds[3]);
							break;

						default:
							decal->SetHiddenInGame(true);
							break;
						}
						break;
					case EPaintColor::Yellow:
						switch (res)
						{
						case 1:
							/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), res), true, FVector2D(1, 1));*/
							decal->SetDecalMaterial(MI_Yellows[1]);
							break;
						case 2:
							decal->SetDecalMaterial(MI_Yellows[2]);
							break;
						case 3:
							decal->SetDecalMaterial(MI_Yellows[3]);
							break;

						default:
							decal->SetHiddenInGame(true);
							break;
						}
						break;

					default:
						
						break;
					}				
				}
			}
		}

	}
}

