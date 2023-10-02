// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RenderTargetProcess.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POWERWASH_API URenderTargetProcess : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URenderTargetProcess();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

//	//void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);
//
//private:
//	void LeftMouseClick();
//	void LeftMouseRelased();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="MySettings|LMH")
	class UMaterial* BrushMaterialTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="MySettings|LMH")
	class UTextureRenderTarget2D* PaintingRenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|LMH")
	class UMaterialInstanceDynamic* BrushMaterialInstance;

	

	UFUNCTION(BlueprintCallable,Category = "MySettings|LMH")
	void DrawCar(const FHitResult& hitInfo);

	UFUNCTION(BlueprintCallable,Category = "MySettings|LMH")
	void DrawSize(float drawSize);

	class AVRCharacter* player;
	//void (FHitResult HitResult);

	void SetBrushOpacity(float op);

	TArray<class ANSActor*> NsSpriteActor;

	//UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "MySettings|LMH")
	//TSubclassOf<class ANSActor> NSSpriteFactory;

	int setnum= 60;
	int NSIdx=0;
	
};
