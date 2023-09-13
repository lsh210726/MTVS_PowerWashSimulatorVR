// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorIndicatorActor.generated.h"

UCLASS()
class POWERWASH_API AFloorIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorIndicatorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category=MySettings)
	class USceneComponent* rootComp; //로테이션 로케이션을 조정하기 위한 씬컴포넌트
	UPROPERTY(EditAnywhere, Category=MySettings)
	class UNiagaraComponent* floorIndicator;
};
