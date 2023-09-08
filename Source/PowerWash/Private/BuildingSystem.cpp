                                                                                                                                                                                                                                                              // Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem.h"
#include <Engine/StaticMeshActor.h>
#include "VRCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UBuildingSystem::UBuildingSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBuildingSystem::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UBuildingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	moveMesh();
}

void UBuildingSystem::moveMesh()
{
    AVRCharacter* PlayerCharacter = Cast<ACharacter>(GetOwner());

    if (PlayerCharacter)
    {
        FVector Start = PlayerCharacter->GetActorLocation(); // 시작 위치는 플레이어 캐릭터의 현재 위치
        FVector End = Start + PlayerCharacter->GetActorForwardVector() * 1000; // 끝 위치는 플레이어 캐릭터의 전방 일정 거리

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;

        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
        {
            HitLocation = HitResult.Location; // 충돌 위치 저장

            if (TemporaryActor)
            {
                TemporaryActor->Destroy(); // 이전에 생성된 임시 액터가 있다면 제거
            }

            FRotator SpawnRotation;
            TemporaryActor = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), HitLocation, SpawnRotation);

            if (TemporaryActor)
            {
                UStaticMeshComponent* Staticmeshcomponent = TemporaryActor->GetStaticMeshComponent();

                if (Staticmeshcomponent)
                {
                    Staticmeshcomponent->SetMobility(EComponentMobility::Movable);
                    Staticmeshcomponent->SetStaticMesh(TemporaryMesh);
                }
            }
        }
    }
}

	
void UBuildingSystem::ConfirmPlacement()
{
    if (TemporaryActor)
    {
        TemporaryActor->Destroy();  // 임시 액터 제거

        FRotator SpawnRotation;
        AStaticMeshActor* PermanentSpawned = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), HitLocation, SpawnRotation);

        if (PermanentSpawned)
        {
            UStaticMeshComponent* staticmeshcomponent = PermanentSpawned->GetStaticMeshComponent();

            if (staticmeshcomponent)
            {
                staticmeshcomponent->SetMobility(EComponentMobility::Movable);
                staticmeshcomponent->SetStaticMesh(PermanentMesh);
            }
        }
    }

}

