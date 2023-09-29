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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category="MySettings|Components")
	class USkeletalMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MySettings|Components")
	class UStaticMeshComponent* muzzleMesh;
	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UArrowComponent* arrowComp;
	UPROPERTY(EditAnywhere, Category="MySettings|Components")//물줄기 거리
	float shootPower=300;
	UPROPERTY(EditAnywhere, Category="MySettings|Components")//넓게 쏠 때 발사할 물줄기 개수
	int shotTime=20;
	UPROPERTY(EditAnywhere, Category = "MySettings|Components")//세로로 쏠지 가로로 쏠지
	bool horShot = false;


	FVector muzzleLocation;
	FRotator muzzleRotation;
	float shotAngle = 0;
	FRotator currRot;

	FRotator StartRotation;
	FRotator TargetRotation;
	float RotationTime = 0;
	double StartTime = 0;

	bool oneTime = true;

	class AMuzzleActor* MuzzleActor;
	

	UFUNCTION()
	void Shoot();

	void WideShot(float degree);
	void ShootWater( FVector muzzleFwdVec);
	void ChangeAngle();
	void shotRot();
	void grabReleseGun();

	//이민하 추가
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "MySettings|Functions")
	void SpawnSteamEffect(FVector muzzleFwdVec);
	UPROPERTY(BlueprintReadWrite)
	FVector MuzzleAngle;


};
