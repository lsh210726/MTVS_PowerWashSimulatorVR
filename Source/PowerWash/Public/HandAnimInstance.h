// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POWERWASH_API UHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings|Animation")
	float PoseAlphaGrasp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings|Animation")
	float PoseAlphaIndexCurl = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings|Animation")
	float PoseAlphaPoint = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings|Animation")
	float PoseAlphaThumbUp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings|Animation")
	bool isLeft = false;
};
