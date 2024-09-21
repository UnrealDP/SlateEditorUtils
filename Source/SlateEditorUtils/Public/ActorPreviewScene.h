// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#pragma once

#include "CoreMinimal.h"
#include "AdvancedPreviewScene.h"

/**
 * 에디터 뷰포트에 액터를 렌더링하는 프리뷰 씬
 */
class SLATEEDITORUTILS_API FActorPreviewScene : public FAdvancedPreviewScene
{
public:
    // 생성자: ConstructionValues를 통해 프리뷰 씬을 설정
    FActorPreviewScene(ConstructionValues CVS)
        : FAdvancedPreviewScene(CVS) {}

    void SetPreviewWorld(UWorld* InWorld);

    void SetPreviewSkeletalMesh(USkeletalMesh* SkeletalMesh);


    void SetPreviewActor(AActor* InActor)
    {
        check(PreviewActor == nullptr || !PreviewActor->IsRooted());
        PreviewActor = InActor;
    }

    AActor* GetPreviewActor() const
    {
        return PreviewActor;
    }

    void SetPreviewSkeletalMeshComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
    {
        PreviewSkeletalMeshComponent = InSkeletalMeshComponent;
    }

    USkeletalMeshComponent* GetPreviewSkeletalMeshComponent() const
    {
        return PreviewSkeletalMeshComponent;
    }
    

private:
    TObjectPtr<ULevel> Level;  // 미리보기를 위한 레벨

    TObjectPtr<AActor> PreviewActor;  // 미리보기를 위한 액터
    TObjectPtr<USkeletalMeshComponent> PreviewSkeletalMeshComponent;  // 미리보기를 위한 스켈레탈 메쉬 컴포넌트
};

