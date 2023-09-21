// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalCompoenent.h"
#include "Camera/PlayerCameraManager.h"
#include <Kismet/GameplayStatics.h>
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "VRCharacter.h"
#include "WaterGunActor.h"
#include "Components/ActorComponent.h"
#include <Components/PrimitiveComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/DecalComponent.h>
#include <Materials/MaterialInterface.h>
#include <Materials/MaterialInstance.h>
#include "MotionControllerComponent.h"

//enum class MI_Red_Name : uint8
//{
//    UMETA(DisplayName = "Value 1"),
//   Value2 UMETA(DisplayName = "Value 2"),
//   Value3 UMETA(DisplayName = "Value 3")
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
    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_2(TEXT("/Game/LMH/Decal2/MI_YellowDecal_2.MI_YellowDecal_2"));
    if (temp_Yellow_2.Succeeded()) MI_Yellows.Add(Cast<UMaterialInstance>(temp_Yellow_2.Object));
    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_3(TEXT("/Game/LMH/Decal2/MI_YellowDecal_3.MI_YellowDecal_3"));
    if (temp_Yellow_3.Succeeded()) MI_Yellows.Add(Cast<UMaterialInstance>(temp_Yellow_3.Object));
    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Yellow_4(TEXT("/Game/LMH/Decal2/MI_YellowDecal_4.MI_YellowDecal_4"));
    if (temp_Yellow_4.Succeeded()) MI_Yellows.Add(Cast<UMaterialInstance>(temp_Yellow_4.Object));

    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Green_1(TEXT("/Game/LMH/Decal2/MI_GreenDecal_1.MI_GreenDecal_1"));
    if (temp_Green_1.Succeeded()) MI_Greens.Add(temp_Green_1.Object);
    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Green_2(TEXT("/Game/LMH/Decal2/MI_GreenDecal_2.MI_GreenDecal_2"));
    if (temp_Green_2.Succeeded()) MI_Greens.Add(Cast<UMaterialInstance>(temp_Green_2.Object));
    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Green_3(TEXT("/Game/LMH/Decal2/MI_GreenDecal_3.MI_GreenDecal_3"));
    if (temp_Green_3.Succeeded()) MI_Greens.Add(Cast<UMaterialInstance>(temp_Green_3.Object));
    ConstructorHelpers::FObjectFinder<UMaterialInstance> temp_Green_4(TEXT("/Game/LMH/Decal2/MI_GreenDecal_4.MI_GreenDecal_4"));
    if (temp_Green_4.Succeeded()) MI_Greens.Add(Cast<UMaterialInstance>(temp_Green_4.Object));
}


// Called when the game starts
void UDecalCompoenent::BeginPlay()
{
    Super::BeginPlay();

    player = GetOwner<AVRCharacter>();
    if (player == nullptr) return;

    if (MI_Color == nullptr) MI_Color = MI_Reds[0];
    gun=player->waterGun;

}

// Called every frame
void UDecalCompoenent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    //if (!IsPainting)
    //{
    //    if (PaintedTotalArea > 0 && PaintedArea > 0)
    //    {
    //       /* GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%d"), PaintedTotalArea), true, FVector2D(1, 1));*/
    //       if ((PaintedArea % PaintedTotalArea) > 0)
    //       {
    //           double pp = (1 / (PaintedArea % PaintedTotalArea)*100);
    //           //double pp = (PaintedTotalArea % PaintedArea);

    //           GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%.2lf"), pp), true, FVector2D(1, 1));
    //       }
    //       
    //       //비율 
    //      
		  // /*if (pp < 40)
		  // {
    //        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%.2lf"), pp), true, FVector2D(1, 1));
		  // }*/  
    //    }
    //}
    /*if (player == nullptr) return;*/

    //if (player->handstate != EHandState::LMH) return;
    /*pc = Cast<APlayerController>(player->GetController());
    if (pc == nullptr) return;*/

    //임시로 카메라 매니저에서 ray
    //if (IsDrawing) {
    //   APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
    //   if (CameraManager)
    //   {
    //      FVector dir = CameraManager->GetTransformComponent()->GetForwardVector();
    //      FVector start = CameraManager->GetTransformComponent()->GetComponentLocation();
    //      FVector end = dir * 3000 + start;

    //      //ray cast

    //      FHitResult HitResult;
    //      FCollisionQueryParams Params;
    //      Params.AddIgnoredActor(GetOwner());

    //      // 
    //      if (GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params))
    //      {
    //         //DecalShoot(HitResult);
    //      }
    //   }
    //}
    //DrawTrajectory(throwDirection, throwPower, myMass);
}
void UDecalCompoenent::DecalShoot(FHitResult HitResult)
{
    FVector Loc = HitResult.Location;
    FVector ImpactPoint = HitResult.ImpactPoint;
    FVector normal = HitResult.Normal;
    FRotator rot = UKismetMathLibrary::MakeRotFromX(normal);
    FVector DecalSize = FVector(3);

    UPrimitiveComponent* hitComp = HitResult.GetComponent();

    if(!hitComp) return;

   // if () return;
    //얼룩 생성모드
    if (IsPainting)
    { 
        //생성 얼룩 색깔 parameter로 주기
        //PaintedArea+=1;
        DoPainting(Loc, hitComp, rot, DecalSize, Color);
    }
    //지우기 모드
    else {
        /*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), DecalComps.Num()), true, FVector2D(1, 1));*/
        //데칼 갯수가 1보다 많으면 for문 시작, 데칼 지우기
        ErasePainting(Loc, DecalSize, Color);
    }
    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), PaintedArea), true, FVector2D(1, 1));
    //DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1.f, 0.f, 1.f);
}

void UDecalCompoenent::AllClearMode(class UDecalComponent* decal)
{
    if (!decal->bHiddenInGame)
    {
        decal->SetHiddenInGame(true);
        --PaintedArea;
    }
}

void UDecalCompoenent::FourStageMode(int res, class UDecalComponent* decal, TArray<class UMaterialInstance*> mi)
{
    switch (res)
    {
    case 1:
        /*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), res), true, FVector2D(1, 1));*/
        decal->SetDecalMaterial(mi[1]);
        break;
    case 2:
        decal->SetDecalMaterial(mi[2]);
        break;
    case 3:
        decal->SetDecalMaterial(mi[3]);
        break;
    default:
        if (!decal->bHiddenInGame) {
            --PaintedArea;
            decal->SetHiddenInGame(true);
            /*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), PaintedArea), true, FVector2D(1, 1));*/
        }
        break;
    }
}

void UDecalCompoenent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
    //enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Started, this, &UDecalCompoenent::LeftTriggerDown);
    //enhancedInputComponent->BindAction(inputActions[2], ETriggerEvent::Completed, this, &UDecalCompoenent::LeftTriggerUp);
}
void UDecalCompoenent::LeftTriggerDown()
{
    /*if (player->handstate == EHandState::LMH) {
       GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("MouseLeftTriggerDown")), true, FVector2D(1, 1));
       IsDrawing = true;
    }*/
    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("MouseLeftTriggerDown")), true, FVector2D(1, 1));
    //IsDrawing = true;
}

void UDecalCompoenent::LeftTriggerUp()
{
    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("MouseLeftTriggerUp")), true, FVector2D(1, 1));
    //IsDrawing=false;
}

void UDecalCompoenent::PowerWashEffect()
{
    if (player->bHasGun && gun->meshComp->DoesSocketExist(TEXT("Muzzle")))
    {
        FVector muzzleLocation = gun->meshComp->GetSocketLocation(TEXT("Muzzle"));
        FRotator muzzleRotation = gun->meshComp->GetSocketRotation(TEXT("Muzzle"));
        USkeletalMeshSocket const* mySocket = nullptr;
        mySocket =gun-> meshComp->GetSocketByName(TEXT("Muzzle"));

        //WideShot(shotAngle);
    }

}

void UDecalCompoenent::DoPainting(FVector Loc, UPrimitiveComponent* hitComp, FRotator rot, FVector DecalSize, EPaintColor pcolor)
{
    switch (pcolor)
    {
    case EPaintColor::Red:
        if (MI_Color != MI_Reds[0]) sortOrder++;
        MI_Color = MI_Reds[0];
        break;
    case EPaintColor::Yellow:
        if (MI_Color != MI_Yellows[0]) sortOrder++;
        MI_Color = MI_Yellows[0];
        break;
    case EPaintColor::Green:
        if (MI_Color != MI_Greens[0]) sortOrder++;
        MI_Color = MI_Greens[0];
        break;
    default:
        if (MI_Color != MI_Reds[0]) sortOrder++;
        MI_Color = MI_Reds[0];
        break;
    }

    //if (DecalComps.Num() > 10) {
    //    for (UDecalComponent* decal : DecalComps)
    //    {
    //        if (decal) {
    //            double dist = FVector::Distance(Loc, decal->GetComponentLocation());
    //            if (dist && dist <= DecalSize.X)
    //            {
    //                if (IsValid(decal->GetDecalMaterial()))
    //                {
    //                    FString dName = decal->GetDecalMaterial()->GetName();
    //                    int res = dName[dName.Len() - 1] - '0';

    //                    switch (pcolor) {
    //                    case EPaintColor::Red:
    //                        setMat(res, decal, MI_Reds);
    //                        break;
    //                    case EPaintColor::Yellow:
    //                        setMat(res, decal, MI_Yellows);
    //                        break;
    //                    case EPaintColor::Green:
    //                        setMat(res, decal, MI_Greens);
    //                        break;
    //                    default:
    //                        setMat(res, decal, MI_Reds);
    //                        break;
    //                    }
    //                }
    //            }
    //        }
    //    }
    //    //GetAttachedActors();
    //    //double dist = FVector::Distance(Loc, decalcomp->GetComponentLocation());
    //    //if (dist && dist <= DecalSize.X)
    //    //{
    //    //    if (IsValid(decalcomp->GetDecalMaterial()))
    //    //    {
    //    //        FString dName = decalcomp->GetDecalMaterial()->GetName();
    //    //        int res = dName[dName.Len() - 1] - '0';
    //    //    }
    //    //}
    //    ///*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d"), DecalComps.Num()), true, FVector2D(1, 1));*/
    //}
    //else {}
        UDecalComponent* decalcomp = UGameplayStatics::SpawnDecalAttached(MI_Color, DecalSize, hitComp, FName("None"), Loc, rot, EAttachLocation::KeepWorldPosition);

        // GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%d\n"), PaintedArea), true, FVector2D(1, 1));
        if (decalcomp)
        {
            PaintedArea += 1;
            decalcomp->SetSortOrder(sortOrder);
            decalcomp->SetFadeScreenSize(0);
            DecalComps.Add(decalcomp);
        }
    
}

void UDecalCompoenent::ErasePainting(FVector Loc, FVector DecalSize, EPaintColor pcolor)
{
    if (DecalComps.Num() < 2) return;
    for (UDecalComponent* decal : DecalComps)
    {
        if (decal) {
            double dist = FVector::Distance(Loc, decal->GetComponentLocation());
            if (dist && dist <= DecalSize.X)
            {
                if (IsValid(decal->GetDecalMaterial()))
                {
                    FString dName = decal->GetDecalMaterial()->GetName();
                    int res = dName[dName.Len() - 1] - '0';

                    switch (pcolor) {
                    case EPaintColor::Red:
                        setMat(res,decal,MI_Reds);
                        break;
                    case EPaintColor::Yellow:
                        setMat(res, decal, MI_Yellows);
                        break;
                    case EPaintColor::Green:
                        setMat(res, decal, MI_Greens);
                        break;
                    default:
                        setMat(res, decal, MI_Reds);
                        break;
                    }
                }
            }
        }
    }
}

void UDecalCompoenent::setMat(int res, class UDecalComponent* decal, TArray<class UMaterialInstance*> mi)
{
    if(mi.Num()<3) return;

    
    if (eraseMode == EEraseMode::AllClear)
    {
       AllClearMode(decal);
    }
    else if (eraseMode == EEraseMode::FourStatge)
    {
        FourStageMode(res,decal,mi);
        
    }
}
