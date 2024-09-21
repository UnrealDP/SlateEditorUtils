// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorPreviewScene.h"
#include "Engine/DirectionalLight.h"
#include "AssetViewerSettings.h"

void FActorPreviewScene::SetPreviewWorld(UWorld* InWorld)
{
    if (!InWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid World provided for ActorPreviewScene"));
        return;
    }

    UWorld* CrrPreviewWorld = GetWorld();
    if (!CrrPreviewWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid CrrPreviewWorld ActorPreviewScene"));
        return;
    }

    ULevel* PersistentLevel = InWorld->PersistentLevel;
    if (!PersistentLevel)
	{
        UE_LOG(LogTemp, Warning, TEXT("Invalid PersistentLevel ActorPreviewScene::SetPreviewLevel"));
        return;
	}

    TArray<class ULevel*> test1 = CrrPreviewWorld->GetLevels();
    int32 AAAA1 = CrrPreviewWorld->GetActorCount();
    UE_LOG(LogTemp, Warning, TEXT("------ 1 --------- %d"), AAAA1);

    CrrPreviewWorld->SetCurrentLevel(PersistentLevel);

    TArray<class ULevel*> test2 = CrrPreviewWorld->GetLevels();
    int32 AAAA2 = CrrPreviewWorld->GetActorCount();
    UE_LOG(LogTemp, Warning, TEXT("------ 2 --------- %d"), AAAA2);

    if (DirectionalLight)
    {
        UE_LOG(LogTemp, Warning, TEXT("DirectionalLight DirectionalLight DirectionalLight"));
    }

    for (int32 LevelIndex = 0; LevelIndex < CrrPreviewWorld->GetNumLevels(); LevelIndex++)
    {
        ULevel* gLevel = CrrPreviewWorld->GetLevel(LevelIndex);
        for (TObjectPtr<AActor> ac : gLevel->Actors)
        {
            USceneComponent* Component = ac->GetRootComponent();
            //AddComponent(cmp, FTransform::Identity);

            //USceneComponent* SceneComp = Cast<USceneComponent>(Component);
            //if (SceneComp && SceneComp->GetAttachParent() == NULL)
            //{
            //    SceneComp->SetRelativeTransform(FTransform::Identity);
            //}

            //Component->RegisterComponentWithWorld(GetWorld());

            //if (bForceAllUsedMipsResident)
            //{
            //    // Add a mip streaming override to the new mesh
            //    UMeshComponent* pMesh = Cast<UMeshComponent>(Component);
            //    if (pMesh != NULL)
            //    {
            //        pMesh->SetTextureForceResidentFlag(true);
            //    }
            //}

            //{
            //    UStaticMeshComponent* pStaticMesh = Cast<UStaticMeshComponent>(Component);
            //    if (pStaticMesh != nullptr)
            //    {
            //        pStaticMesh->bEvaluateWorldPositionOffset = true;
            //        pStaticMesh->bEvaluateWorldPositionOffsetInRayTracing = true;
            //    }
            //}

            //GetScene()->UpdateSpeedTreeWind(0.0);
        }
    }

    FPreviewSceneProfile pf = FPreviewSceneProfile();
    UpdateScene(pf);

    UE_LOG(LogTemp, Log, TEXT("Preview level SetCurrentLevel: %s"), *InWorld->GetName());
}

void FActorPreviewScene::SetPreviewSkeletalMesh(USkeletalMesh* SkeletalMesh)
{
    // setup default scene
    AActor* Actor = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FTransform::Identity);
    SetPreviewActor(Actor);

    // Create the preview component
    USkeletalMeshComponent* SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(Actor);
    if (GEditor->PreviewPlatform.GetEffectivePreviewFeatureLevel() <= ERHIFeatureLevel::ES3_1)
    {
        SkeletalMeshComponent->SetMobility(EComponentMobility::Static);
    }
    AddComponent(SkeletalMeshComponent, FTransform::Identity);
    SetPreviewSkeletalMeshComponent(SkeletalMeshComponent);

    // set root component, so we can attach to it. 
    Actor->SetRootComponent(SkeletalMeshComponent);

    USkeletalMeshComponent* NewPreviewSkeletalMeshComponent = NewObject<USkeletalMeshComponent>(GetWorld()->GetCurrentLevel());
    NewPreviewSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);

    AddComponent(NewPreviewSkeletalMeshComponent, FTransform::Identity);
}
