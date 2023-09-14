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
	bool IsPainting = true; //얼룩 생성 모드 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings|Mode")
	bool IsDrawing=false; //호스 누르기/떼기 
	
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings|Material")
	class UMaterialInstance* MI_Color;*/

	UPROPERTY()
	TArray<class UMaterialInstance*> MI_Reds;

	UPROPERTY()
	TArray<class UMaterialInstance*> MI_Yellows;
	//UPROPERTY()
	UPROPERTY()
	TArray<class UMaterialInstance*> MI_Greens;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MySettings|PaintColor")
	EPaintColor Color = EPaintColor::Red;

	class APlayerController* pc;
	class AVRCharacter* player;

	UPROPERTY()
	TArray<class UDecalComponent*> DecalComps;

	UFUNCTION(BlueprintCallable, Category = "MySettings|MyFunctions")
	void DoPainting(FVector Loc, UPrimitiveComponent* hitComp, FRotator rot, FVector DecalSize, EPaintColor pcolor);
	UFUNCTION(BlueprintCallable, Category = "MySettings|MyFunctions")
	void ErasePainting(FVector Loc, FVector DecalSize, EPaintColor pcolor);


	//
	void LeftTriggerDown();
	void LeftTriggerUp();
	//MoveCompoenent 의 left button 클릭여부 가져옴

	/*UFUNCTION(BlueprintCallable, Category = "LMH|Function")
	bool chec*/
	
};
