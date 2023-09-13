// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderTargetProcess.h"
#include "Components/SceneComponent.h"
#include "Materials/Material.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include <Engine/Canvas.h>

// Sets default values for this component's properties
URenderTargetProcess::URenderTargetProcess()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*Scence=CreateDefaultSubobject<USceneComponent>(TEXT("Scence"));*/
	
	BrushMaterialTemplates=CreateDefaultSubobject<UMaterial>(TEXT("Material"));

	ConstructorHelpers::FObjectFinder<UMaterial> temp_mat(TEXT("/Game/LMH/RenderTarget/M_RenderTarget_Brush_Version1.M_RenderTarget_Brush_Version1"));
	if (temp_mat.Succeeded())
	{
		BrushMaterialTemplates = temp_mat.Object;
	}

	PaintingRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("PaintingRenderTarget"));

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> temp_renderTarget(TEXT("/Game/LMH/RenderTarget/RT_Painting.RT_Painting"));
	if (temp_renderTarget.Succeeded())
	{
		PaintingRenderTarget = temp_renderTarget.Object;
	}



}


// Called when the game starts
void URenderTargetProcess::BeginPlay()
{
	Super::BeginPlay();

	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(),PaintingRenderTarget, PaintingRenderTarget->ClearColor);
	
	BrushMaterialInstance=UMaterialInstanceDynamic::Create(BrushMaterialTemplates, this);
	BrushMaterialInstance->SetTextureParameterValue(FName("RenderTarget"),PaintingRenderTarget);

}


// Called every frame
void URenderTargetProcess::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URenderTargetProcess::DrawBrush()
{
	UCanvas* canvas;
	FVector2D canvasSize;
	FDrawToRenderTargetContext context;

	//FVector location = GetActorLocation();
	if (PaintingRenderTargetCopy)
	{
		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, PaintingRenderTargetCopy, canvas, canvasSize, context);

		canvas->SetDrawColor(0,0,0,0);
		canvas->K2_DrawMaterial(BrushMaterialInstance, FVector2D(0, 0), canvasSize, FVector2D(0, 0), FVector2D(1, 1), 0.f, FVector2D(0, 0));


		UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, context);
	}
	
	
}

void URenderTargetProcess::CopyToMainCanvas()
{
	UCanvas* canvas;
	FVector2D canvasSize;
	FDrawToRenderTargetContext context;

	//FVector location = GetActorLocation();
	if (PaintingRenderTarget)
	{		UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, PaintingRenderTarget, canvas, canvasSize, context);


	canvas->K2_DrawTexture(PaintingRenderTargetCopy, FVector2D(0, 0), canvasSize, FVector2D(0, 0), FVector2D(
		1, 1), FColor::White, BLEND_Opaque, 0.f, FVector2D(0.5, 0.5));


	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, context);
	}
	

}

