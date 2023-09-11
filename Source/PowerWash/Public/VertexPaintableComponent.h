//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "VertexPaintableComponent.generated.h"
//
///*
// * Inspirations and sources:
// * - http://www.orfeasel.com/vertex-painting-at-runtime/
// * - https://github.com/alanedwardes/UE4VertexColorSpread
// * - https://www.raywenderlich.com/6817-dynamic-mesh-painting-in-unreal-engine-4
// * - http://unrealpaths.blogspot.com/2016/06/vertex-painting-on-meshes.html
// * - https://www.youtube.com/watch?v=lkxZ1DMRQPg
// */
//
///**
//Channel used for spreading vertex colors.
//*/
//UENUM()
//namespace AVertexColorSpreadChannel
//{
//  enum Channel
//  {
//    Red,
//    Green,
//    Blue,
//    Alpha,
//  };
//}
//
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
//class POWERWASH_API UVertexPaintableComponent : public UActorComponent
//{
//  GENERATED_BODY()  
//
//public:  
//  // Sets default values for this component's properties
//  UVertexPaintableComponent();
//
//  UFUNCTION(BlueprintCallable, Category=VertexPainting)
//  void PaintVertexAtLocation(FVector HitLocation, float PaintLerpProgress = 1.0f);
//
//  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=VertexPainting)
//  FColor TargetBaseColor = FColor::White;
//
// protected:
//  // Called when the game starts
//  virtual void BeginPlay() override;
//
//  /**
//  Create LOD info entries, and ensure the override vertex color buffer exists.
//  */
//  void InitialiseLODInfoAndBuffers();
//
//  /**
//  Create a new override vertex color buffer.
//  */
//  void InitialiseInstancedOverrideVertexColorBuffer(FStaticMeshComponentLODInfo* InstanceMeshLODInfo,
//                                                    FStaticMeshLODResources& LODModel);
//  
//  /**
//  Get the intensity of the selected channel (see Channel)
//  */
//  int32 GetNearestVertIndex(FVector Position,FStaticMeshLODResources& LODModel);
//
//  /**
//  Get the intensity of the selected channel (see Channel)
//  */
//  //uint8 GetIntensity(FColor Color);
//
//  /**
//  Set the intensity of the selected channel to the given value (see Channel)
//  */
////  void SetIntensity(FColor *Color, uint8 Intensity);
//
//  UPROPERTY(BlueprintReadOnly)
//  UStaticMeshComponent* StaticMeshComponent;    
//
//    /** Purposely hidden */
//   
//};