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
{// ������ ���� ���� ���
    FVector ConeDirection = FRotationMatrix::MakeFromX(FVector(1.f, 0.f, 0.f)).Rotator().RotateVector(FVector(0.f, 1.f, 0.f));
    
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("%s"), *ConeDirection.ToString()), true, FVector2D(1, 1));
    // �� ���� Ʈ���̽��� ȸ�� ���� ���
    float AngleIncrement = ConeAngle / (NumTraces - 1);
    FRotator Rotation(0.f);

    for (int32 i = 0; i < NumTraces; ++i)
    {
        // ���� ȸ���� ���� ���� ���
        FVector TraceDirection = Rotation.RotateVector(ConeDirection);

        // ���� ��ġ�� ���� ��ġ ���
        FVector StartLocation = Origin;
        FVector EndLocation = Origin + TraceDirection * ConeLength;

        TArray<FHitResult> OutHits;
        FCollisionQueryParams CollisionParams;

        // ���� Ʈ���̽� ����
        bool bHit = GetWorld()->LineTraceMultiByChannel(
            OutHits,
            StartLocation,
            EndLocation,
            ECollisionChannel::ECC_Visibility,
            CollisionParams
        );

        if (bHit)
        {
            // �浹�� ��ü�鿡 ���� ó��
            for (const FHitResult& Hit : OutHits)
            {
                AActor* HitActor = Hit.GetActor();
                if (HitActor)
                {
                    // ó�� ���� �ۼ�
                    // HitActor�� ��ȣ�ۿ� �� �ʿ��� �۾� ����
                }
            }
        }

        // ����׿����� ���� �׸���
        DrawDebugLine(
            GetWorld(),
            StartLocation,
            EndLocation,
            FColor::Red,
            false, 4.f, 0.f,
            10.f);

        // ���� ȸ�� ������ ������Ʈ
        Rotation.Yaw += AngleIncrement;
    }
}

