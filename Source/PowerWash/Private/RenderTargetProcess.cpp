// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderTargetProcess.h"
#include "Components/SceneComponent.h"
#include "Materials/Material.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include <Engine/Canvas.h>
#include <Kismet/KismetMathLibrary.h>
#include <Math/Color.h>
#include <Kismet/GameplayStatics.h>
#include "VRCharacter.h"

// Sets default values for this component's properties
URenderTargetProcess::URenderTargetProcess()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*Scence=CreateDefaultSubobject<USceneComponent>(TEXT("Scence"));*/
	
	BrushMaterialTemplates=CreateDefaultSubobject<UMaterial>(TEXT("Material"));

	ConstructorHelpers::FObjectFinder<UMaterial> temp_mat(TEXT("/Game/LMH/protomap/WhiteBoardMarker.WhiteBoardMarker"));
	if (temp_mat.Succeeded()) BrushMaterialTemplates = temp_mat.Object;
	
	PaintingRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("PaintingRenderTarget"));

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> temp_renderTarget(TEXT("/Game/LMH/protomap/1PaintBoard.1PaintBoard"));
	if (temp_renderTarget.Succeeded()) PaintingRenderTarget = temp_renderTarget.Object;

}


// Called when the game starts
void URenderTargetProcess::BeginPlay()
{
	Super::BeginPlay();
	//player=GetOwner<AVRCharacter>();
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(),PaintingRenderTarget, PaintingRenderTarget->ClearColor);
	
	BrushMaterialInstance=UMaterialInstanceDynamic::Create(BrushMaterialTemplates, this); //brush 생성
	//BrushMaterialInstance->SetTextureParameterValue(FName("RenderTarget"),PaintingRenderTarget); // RT parameter로 주기

}


// Called every frame
void URenderTargetProcess::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URenderTargetProcess::DrawCar(const FHitResult& hitInfo)
{
	/*if (player != nullptr)
	{
		return;
	}*/
	/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%s"), *hitInfo.GetActor()->GetName()), true, FVector2D(1, 1));*/
	FVector2D UV;
	
	//bool hit= player->IsHitFindCollisionUV(hitInfo, 0, UV);//UGameplayStatics::FindCollisionUV(hitInfo, 0, UV);
	bool hit= UGameplayStatics::FindCollisionUV(hitInfo, 0, UV);
	if (hit) 
	{
		//auto HitActor = Cast<AActor>(hitInfo.GetActor());
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("111111")), true, FVector2D(1, 1));

		FDrawToRenderTargetContext context;
		FLinearColor DrawLocation_color = UKismetMathLibrary::Conv_VectorToLinearColor(UKismetMathLibrary::Conv_Vector2DToVector(UV, 0.1));

		if (PaintingRenderTarget != nullptr )
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("22222222222")), true, FVector2D(1, 1));

			BrushMaterialInstance->SetVectorParameterValue(FName("DrawLocation"), DrawLocation_color);
			UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), PaintingRenderTarget, BrushMaterialInstance);
		}
	}	
}

void URenderTargetProcess::DrawSize(float drawSize)
{
	BrushMaterialInstance->SetScalarParameterValue(FName("Size"),drawSize);
}

//void URenderTargetProcess::CopyToMainCanvas()
//{
//	UCanvas* canvas;
//	FVector2D canvasSize;
//	FDrawToRenderTargetContext context;
//
//	//FVector location = GetActorLocation();
//	if (PaintingRenderTarget)
//	{		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, PaintingRenderTarget, canvas, canvasSize, context);
//
//
//	//canvas->K2_DrawTexture(PaintingRenderTargetCopy, FVector2D(0, 0), canvasSize, FVector2D(0, 0), FVector2D(
//	//	1, 1), FColor::White, BLEND_Opaque, 0.f, FVector2D(0.5, 0.5));
//
//
//	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, context);
//	}
//	
//
//}

