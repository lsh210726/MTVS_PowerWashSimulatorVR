// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MuzzleActor.generated.h"

UCLASS()
class POWERWASH_API AMuzzleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuzzleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class AVRCharacter* player;
	FTimerHandle MyTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings|Components")
	class UStaticMeshComponent* meshComp;

	class AWaterGunActor* waterGun;
	int32 rotCounter = 0;
	bool letsRotate = false;

	void rotateEvent();
	void rotateMuzzle();
};
