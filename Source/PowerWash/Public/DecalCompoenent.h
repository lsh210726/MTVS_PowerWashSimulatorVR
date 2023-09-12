// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DecalCompoenent.generated.h"


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
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "LMH|Settings")
	bool IsPainting=true; //��� ���� ��� 
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "LMH|Settings")
	bool IsDrawing=false; //ȣ�� ������/���� 

	class APlayerController* pc;
	class AVRCharacter* player;

	UFUNCTION(BlueprintCallable, Category = "LMH|Function")
	void DoPainting();
	
	//���ʹ�ư click/release
	void LeftTriggerDown();
	void LeftTriggerUp();
	//MoveCompoenent �� left button Ŭ������ ������

	/*UFUNCTION(BlueprintCallable, Category = "LMH|Function")
	bool chec*/
	
};
