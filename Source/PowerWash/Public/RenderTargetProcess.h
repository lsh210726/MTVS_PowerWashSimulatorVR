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

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="MySettings|LMH")
	class UTextureRenderTarget2D* PaintingRenderTarget2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|LMH")
	class UMaterialInstanceDynamic* BrushMaterialInstance;

	

	UFUNCTION(BlueprintCallable,Category = "MySettings|LMH")
	void DrawCar(const FHitResult& hitInfo, FVector muzzleLocation);

	UFUNCTION(BlueprintCallable,Category = "MySettings|LMH")
	void DrawSize(float drawSize);

	UFUNCTION(BlueprintNativeEvent)
	void SetCarMaterial();

	//class UStaticMeshComponent* 
	class AVRCharacter* player;
	//void (FHitResult HitResult);

	void SetBrushOpacity(float op);

	TArray<class ANSActor*> NsSpriteActor;

	//UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "MySettings|LMH")
	//TSubclassOf<class ANSActor> NSSpriteFactory;

	int setnum= 60;
	int NSIdx=0;

	UPROPERTY()
	class UMaterialInstance* MI_Puddle;

	//FVector DecalSize=FVector(1, 0.05, 0.05);
	FVector DecalSize=FVector(5, 5, 5);

	UPROPERTY(VisibleAnywhere)
	TArray<class UDecalComponent*> DecalActors;

	UPROPERTY(EditAnywhere, Category = "MySettings|LMH")
	class UParticleSystem* BeamParticles;

	void PowerWashEffect(FVector point, FVector muzzleLocation);


};
