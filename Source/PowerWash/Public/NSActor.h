// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NSActor.generated.h"

UCLASS()
class POWERWASH_API ANSActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANSActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LMH|Effects")
	class UNiagaraSystem* NSSpriteSource;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LMH|Effects")
	//class UNiagaraSystem* NSShootSource;

	//TArray<class UNiagaraComponent*> NSSpritecomps;
	
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NSSpritecomp;

	UFUNCTION()
	void SetVisibleOnOff(bool isOn);

	void StartTimer();
	void TimerCallback();
};
