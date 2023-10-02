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
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NSActor.h"
#include <Components/ActorComponent.h>
#include <Components/AudioComponent.h>
#include <Sound/SoundCue.h>

// Sets default values for this component's properties
URenderTargetProcess::URenderTargetProcess()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/*Scence=CreateDefaultSubobject<USceneComponent>(TEXT("Scence"));*/
	
	BrushMaterialTemplates=CreateDefaultSubobject<UMaterial>(TEXT("Material"));

	ConstructorHelpers::FObjectFinder<UMaterial> temp_mat(TEXT("/Game/LMH/protomap/Brush.Brush"));
	if (temp_mat.Succeeded()) BrushMaterialTemplates = temp_mat.Object;
	
	PaintingRenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("PaintingRenderTarget"));

	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> temp_renderTarget(TEXT("/Game/LMH/protomap/1PaintBoard.1PaintBoard"));
	if (temp_renderTarget.Succeeded()) PaintingRenderTarget = temp_renderTarget.Object;

	//ConstructorHelpers::FClassFinder<ANSActor> temp_NSActor(TEXT("/Game/LMH/betamap/BP_NSActor.BP_NSActor_C"));
	//if (temp_NSActor.Succeeded())
	//{
	//	NSSpriteFactory=temp_NSActor.Class;
	//}

}


// Called when the game starts
void URenderTargetProcess::BeginPlay()
{
	Super::BeginPlay();
	//player=GetWorld()-> GetActorOfClass()

	//player = Cast<AVRCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRCharacter::StaticClass()));
	//if (!player) return;

	NsSpriteActor.SetNum(setnum);
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(),PaintingRenderTarget, PaintingRenderTarget->ClearColor);
	
	BrushMaterialInstance=UMaterialInstanceDynamic::Create(BrushMaterialTemplates, this); //brush 생성
	//BrushMaterialInstance->SetTextureParameterValue(FName("RenderTarget"),PaintingRenderTarget); // RT parameter로 주기
	//
	
	UWorld* world=GetWorld();
	for (int i = 0; i < setnum; i++)
	{
		if (world)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			
			ANSActor* ns = world->SpawnActor<ANSActor>(ANSActor::StaticClass(), FVector(0,0,0),FRotator(0,0,0), Params);
			if (ns) NsSpriteActor[i] = ns;
		}
		

		/*TArray<UActorComponent*> NSs = NsSpriteActor[i]->GetComponentsByClass(UNiagaraComponent::StaticClass());
		UNiagaraComponent* NS = Cast<UNiagaraComponent>(NSs[0]);
		NS->SetVisibility(false);*/
	}

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

	/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("%s"), *hitInfo.GetActor()->GetName()), true, FVector2D(1, 1));*/
	
	
	//bool hit= player->IsHitFindCollisionUV(hitInfo, 0, UV);//UGameplayStatics::FindCollisionUV(hitInfo, 0, UV);
	
	FVector2D UV;
	bool hit= UGameplayStatics::FindCollisionUV(hitInfo, 0, UV);

	//UE_LOG(LogTemp, Warning, TEXT("UV: %s"), hit ? *FString("True") : *FString("False"));

	if (hit) 
	{
// 		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("UV: %.2f, %.2f"), UV.X, UV.Y), true, FVector2D(1, 1));
// 		UE_LOG(LogTemp, Warning, TEXT("UV: %.2f, %.2f"), UV.X, UV.Y);

		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("111111")), true, FVector2D(1, 1));


		FVector point = hitInfo.ImpactPoint; 
		if (NSIdx>= setnum) NSIdx=0;
		if (NsSpriteActor[NSIdx])
		{	
			/*GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("nsidx:%d"),NSIdx), true, FVector2D(1, 1));*/
			NsSpriteActor[NSIdx]->SetActorLocation(point);
			NsSpriteActor[NSIdx++]->SetVisibleOnOff(true);

			//if (SprayCueComponent->bIsPaused) SprayCueComponent->SetPaused(false);
		}
		
		FDrawToRenderTargetContext context;
		FLinearColor DrawLocation_color = UKismetMathLibrary::Conv_VectorToLinearColor(UKismetMathLibrary::Conv_Vector2DToVector(UV, 0.1));

		if (PaintingRenderTarget != nullptr )
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("22222222222")), true, FVector2D(1, 1));

			BrushMaterialInstance->SetVectorParameterValue(FName("DrawLocation"), DrawLocation_color);
			UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(), PaintingRenderTarget, BrushMaterialInstance);

			//if (!SprayCueComponent->bIsPaused) SprayCueComponent->SetPaused(true);

		}
	}
}

void URenderTargetProcess::DrawSize(float drawSize)
{
	BrushMaterialInstance->SetScalarParameterValue(FName("Size"),drawSize);
}

void URenderTargetProcess::SetBrushOpacity(float op)
{
	if (op<=0 ) op=0;
	else if(op>1) op=1;
	BrushMaterialInstance->SetScalarParameterValue(FName("op"),op);
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
//void URenderTargetProcess::SetupPlayerInputComponent(UEnhancedInputComponent* enhancedInputComponent, TArray<UInputAction*> inputActions)
//{
//	enhancedInputComponent->BindAction(player->inputActions[2], ETriggerEvent::Started, this, &URenderTargetProcess::LeftMouseClick);
//	enhancedInputComponent->BindAction(player->inputActions[2], ETriggerEvent::Completed, this, &URenderTargetProcess::LeftMouseRelased);
//}
//
//void URenderTargetProcess::LeftMouseClick()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("111111")), true, FVector2D(1, 1));
//	if (SprayCueComponent->bIsPaused) SprayCueComponent->SetPaused(false);
//}
//
//void URenderTargetProcess::LeftMouseRelased()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, FString::Printf(TEXT("2222")), true, FVector2D(1, 1));
//	if (!SprayCueComponent->bIsPaused) SprayCueComponent->SetPaused(true);
//}
