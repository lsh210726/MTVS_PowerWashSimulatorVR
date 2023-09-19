// Fill out your copyright notice in the Description page of Project Settings.


#include "DirtyStainComponent.h"

// Sets default values for this component's properties
UDirtyStainComponent::UDirtyStainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDirtyStainComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDirtyStainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDirtyStainComponent::CreateDirtyStatin(FVector Origin, float ConeLength = 1000.f, int NumTraces=10, float ConeAngle = 60.f)
{// 원뿔의 방향 벡터 계산
    FVector ConeDirection = FRotationMatrix::MakeFromX(FVector(1.f, 0.f, 0.f)).Rotator().RotateVector(FVector(0.f, 1.f, 0.f));
    
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s"), *ConeDirection.ToString()), true, FVector2D(1, 1));
    // 각 라인 트레이스의 회전 각도 계산
    float AngleIncrement = ConeAngle / (NumTraces - 1);
    FRotator Rotation(0.f);

    for (int32 i = 0; i < NumTraces; ++i)
    {
        // 현재 회전된 방향 벡터 계산
        FVector TraceDirection = Rotation.RotateVector(ConeDirection);

        // 시작 위치와 종료 위치 계산
        FVector StartLocation = Origin;
        FVector EndLocation = Origin + TraceDirection * ConeLength;

        TArray<FHitResult> OutHits;
        FCollisionQueryParams CollisionParams;

        // 라인 트레이스 실행
        bool bHit = GetWorld()->LineTraceMultiByChannel(
            OutHits,
            StartLocation,
            EndLocation,
            ECollisionChannel::ECC_Visibility,
            CollisionParams
        );

        if (bHit)
        {
            // 충돌한 객체들에 대한 처리
            for (const FHitResult& Hit : OutHits)
            {
                AActor* HitActor = Hit.GetActor();
                if (HitActor)
                {
                    // 처리 로직 작성
                    // HitActor와 상호작용 등 필요한 작업 수행
                }
            }
        }

        // 디버그용으로 선분 그리기
        DrawDebugLine(
            GetWorld(),
            StartLocation,
            EndLocation,
            FColor::Red,
            false, 4.f, 0.f,
            10.f);

        // 다음 회전 각도로 업데이트
        Rotation.Yaw += AngleIncrement;
    }
}

