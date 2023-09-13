// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaterGunActor.generated.h"

UCLASS()
class POWERWASH_API AWaterGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterGunActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	class AVRCharacter* player;

public:
	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class USkeletalMeshComponent* meshComp;

};
