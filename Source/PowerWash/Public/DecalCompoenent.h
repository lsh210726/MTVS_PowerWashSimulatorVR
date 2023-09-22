// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DecalCompoenent.generated.h"

UENUM(BlueprintType)
enum class EPaintColor : uint8
{
    Red,
    Yellow,
    Green
};

UENUM(BlueprintType)
enum class EEraseMode : uint8
{
    AllClear,
    FourStatge
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POWERWASH_API UDecalCompoenent : public UActorComponent
{
   GENERATED_BODY()

public:   
   // Sets default values for this component's properties
   UDecalCompoenent();

protected:
   // Called when the game starts
   virtual void BeginPlay() override;

public:   
   // Called every frame
   virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
   void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);
   UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "MySettings|Mode")
   bool IsPainting = true; //��� ���� ��� 
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings|Mode")
   bool IsDrawing=false; //ȣ�� ������/���� 
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings|Mode")
   EEraseMode eraseMode=EEraseMode::AllClear;

   UPROPERTY()
   TArray<class UMaterialInstance*> MI_Reds;

   UPROPERTY()
   TArray<class UMaterialInstance*> MI_Yellows;

   UPROPERTY()
   TArray<class UMaterialInstance*> MI_Greens;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings|PaintColor")
   EPaintColor Color = EPaintColor::Red;

   class APlayerController* pc;
   class AVRCharacter* player;
   class AWaterGunActor* gun;

   UPROPERTY()
   TArray<class UDecalComponent*> DecalComps;

   UPROPERTY()
   int sortOrder=0;
   UPROPERTY(BlueprintReadOnly)
   int PaintedArea=0;
   UPROPERTY(BlueprintReadWrite);
   int PaintedTotalArea=0;

   UPROPERTY()
   class UMaterialInstance* MI_Color;

   UFUNCTION(BlueprintCallable, Category = "MySettings|MyFunctions")
   void DoPainting(FVector Loc, UPrimitiveComponent* hitComp, FRotator rot, FVector DecalSize, EPaintColor pcolor);
   UFUNCTION(BlueprintCallable, Category = "MySettings|MyFunctions")
   void ErasePainting(FVector Loc, FVector DecalSize, EPaintColor pcolor);

   void setMat(int res, class UDecalComponent* decal,TArray<class UMaterialInstance*> mi);
   void DecalShoot(FHitResult HitResult);
   void AllClearMode(class UDecalComponent* decal);
   void FourStageMode(int res,class UDecalComponent* decal, TArray<class UMaterialInstance*> mi);

   //
   void LeftTriggerDown();
   void LeftTriggerUp();

   void PowerWashEffect();
   //MoveCompoenent �� left button Ŭ������ ������


   /*UFUNCTION(BlueprintCallable, Category = "LMH|Function")
   bool chec*/
   
};