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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LMH|Settings")
	class UMaterial* BrushMaterialTemplates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LMH|Settings")
	class UTextureRenderTarget2D* PaintingRenderTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LMH|Settings")
	class UMaterialInstanceDynamic* BrushMaterialInstance;

	

	UFUNCTION(BlueprintCallable,Category = "LMH|Settings")
	void DrawCar(const FHitResult& hitInfo);

	UFUNCTION(BlueprintCallable,Category = "LMH|Settings")
	void DrawSize(float drawSize);

	class AVRCharacter* player;
	//void (FHitResult HitResult);


	void SetBrushOpacity(float op);

	
};
