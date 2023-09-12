// Fill out your copyright notice in the Description page of Project Settings.


#include "DecalCompoenent.h"
#include "Camera/PlayerCameraManager.h"
#include <Kismet/GameplayStatics.h>

UENUM(BlueprintType)
enum class NozzleMode : uint8
{
	/*Value1 UMETA(DisplayName = "Value 1"),
	Value2 UMETA(DisplayName = "Value 2"),*/
	Value3 UMETA(DisplayName = "Value 3")
};
// Sets default values for this component's properties
UDecalCompoenent::UDecalCompoenent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDecalCompoenent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDecalCompoenent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//임시로 카메라 매니저에서 ray
	AActor* Owner = GetOwner();
	if(Owner==nullptr) return;

	pc = Cast<APlayerController>(Owner->GetInstigatorController());

	if (pc == nullptr) return;

	//APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	//if (CameraManager)
	//{
	//	FVector dir= CameraManager->GetTransformComponent()->GetForwardVector();
	//	FVector start= CameraManager->GetTransformComponent()->GetComponentLocation();
	//	FVector end= dir*3000+start;

	//	//ray cast

	//	FHitResult HitResult;
	//	FCollisionQueryParams Params;
	//	Params.AddIgnoredActor(GetOwner());
	//	
	//	

	//	if (GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility, Params))
	//	{
	//		
	//		// 충돌한 오브젝트에 대한 처리
	//		//AActor* HitActor = HitResult.GetActor();
	//		//if (HitActor)
	//		//{
	//		//	
	//		//}

	//		FVector Loc = HitResult.Location;
	//		//FRotator ImpactNormalRotation = HitResult.ImpactNormal.Rotation();
	//		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1.f, 0.f, 1.f);
	//	}
	//	
	//}

}

void UDecalCompoenent::DoPainting()
{

}

