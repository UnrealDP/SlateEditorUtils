// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AdvancedPreviewScene.h"
#include "Camera/CameraComponent.h"

/**
 * 에디터 뷰포트에 액터를 렌더링하고, 카메라를 조작할 수 있는 프리뷰 씬
 */
class SLATEEDITORUTILS_API FActorPreviewSceneOld : public FAdvancedPreviewScene
{
public:
    FActorPreviewSceneOld(ConstructionValues CVS);

    // 메쉬 또는 액터 설정
    void SetPreviewActor(AActor* InActor);

    // 뷰포트에서 메쉬 또는 액터를 렌더링
    void RenderToViewport(TSharedRef<SViewport> InViewport);

    // 카메라 조작: 마우스 드래그로 회전, 스크롤로 확대/축소
    void OnMouseDrag(FVector2D DragDelta);

    // 마우스 스크롤로 확대/축소
    void OnMouseScroll(float ScrollDelta);

    // 초기 카메라 설정으로 복귀
    void ResetCamera();

private:
    // 카메라 위치와 회전 설정
    void SetCameraTransform(FVector InLocation, FRotator InRotation);

    void UpdateCamera(TSharedRef<SViewport> InViewport);

    FVector DefaultCameraLocation;
    FRotator DefaultCameraRotation;
    FVector CameraLocation;
    FRotator CameraRotation;
    float CameraDistance;

    AActor* PreviewActor = nullptr;  // 현재 렌더링 중인 액터

    UCameraComponent* CameraComponent = nullptr;  // 카메라 컴포넌트
};
