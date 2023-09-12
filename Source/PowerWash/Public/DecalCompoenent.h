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

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "LMH|Settings")
	bool Painting=true; //��� ���� ��� 
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "LMH|Settings")
	bool IsDrawing=false; //ȣ�� ������/���� 

	class APlayerController* pc;

	UFUNCTION(BlueprintCallable, Category = "LMH|Function")
	void DoPainting();

	//MoveCompoenent �� left button Ŭ������ ������

	/*UFUNCTION(BlueprintCallable, Category = "LMH|Function")
	bool chec*/
	
};
