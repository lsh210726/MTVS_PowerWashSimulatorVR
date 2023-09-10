// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POWERWASH_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	float interval = 0.1f;//몇초에 한번 계산할지 간격

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	int32 segment = 50; //몇번 반복해서 계산할지 반복회수

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	FVector throwDirection = FVector(1,0,1);

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	float throwPower = 500;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	float myMass = 1;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|MoveMent")
	TSubclassOf<class AFloorIndicatorActor> indicator_BP;//생성할 인디케이터 액터


private:
	class AVRCharacter* player;
	bool bIsShowLine = false;
	TArray<FVector> linePositions;
	class AFloorIndicatorActor* indicatorActor; //생성한 인디케이터 액터
	
	void Move(const struct FInputActionValue& value);
	void Rotate(const struct FInputActionValue& value);
	void LeftTriggerDown();
	void LeftTriggerUp();

	void DrawTrajectory(FVector dir, float power, float mass);
	void DrawBazierCurve();
	void DrawReflectionVector(FVector dir, float entireDist);
};
