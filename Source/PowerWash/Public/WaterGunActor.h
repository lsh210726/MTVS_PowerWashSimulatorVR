// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Source\PowerWash\Public\VRCharacter.h"
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
	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UStaticMeshComponent* muzzleMesh;
	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UArrowComponent* arrowComp;
	UPROPERTY(EditAnywhere, Category="MySettings|Components")//물줄기 거리
	float shootPower=300;
	UPROPERTY(EditAnywhere, Category="MySettings|Components")//넓게 쏠 때 발사할 물줄기 개수
	int shotTime=10;

	FVector muzzleLocation;
	FRotator muzzleRotation;
	float shotAngle = 0;

	UFUNCTION()
	void Shoot();

	void WideShot(float degree, bool horShot);
	void ShootWater( FVector muzzleFwdVec);
	void ChangeAngle();



};
