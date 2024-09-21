// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPreviewSceneOld.h"
#include "Camera/CameraComponent.h"

FActorPreviewSceneOld::FActorPreviewSceneOld(ConstructionValues CVS)
    : FAdvancedPreviewScene(CVS)
{
    // 기본 카메라 설정
    DefaultCameraLocation = FVector(0, 0, 200);
    DefaultCameraRotation = FRotator(-45, 0, 0);
    CameraDistance = 400.0f;

    // 카메라 컴포넌트 생성 및 등록
    CameraComponent = NewObject<UCameraComponent>(GetTransientPackage());
    CameraComponent->RegisterComponentWithWorld(PreviewWorld);
    CameraComponent->SetWorldLocation(DefaultCameraLocation);
    CameraComponent->SetWorldRotation(DefaultCameraRotation);

    // 기본 카메라 설정 적용
    SetCameraTransform(DefaultCameraLocation, DefaultCameraRotation);
}

// 메쉬 또는 액터 설정
void FActorPreviewSceneOld::SetPreviewActor(AActor* InActor)
{
    PreviewActor = InActor;
}

// 뷰포트에서 메쉬 또는 액터를 랜더링
void FActorPreviewSceneOld::RenderToViewport(TSharedRef<SViewport> InViewport)
{
    if (PreviewActor)
    {
        PreviewActor->SetActorLocation(FVector::ZeroVector);
        PreviewActor->SetActorRotation(FRotator::ZeroRotator);
    }

    UpdateCamera(InViewport);  // 카메라 설정 적용
}

// 카메라 조작: 마우스 드래그로 회전
void FActorPreviewSceneOld::OnMouseDrag(FVector2D DragDelta)
{
    CameraRotation.Yaw += DragDelta.X;
    CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + DragDelta.Y, -80.0f, 80.0f);

    SetCameraTransform(CameraLocation, CameraRotation);
}

// 마우스 스크롤로 확대/축소
void FActorPreviewSceneOld::OnMouseScroll(float ScrollDelta)
{
    CameraDistance = FMath::Clamp(CameraDistance - ScrollDelta * 10.0f, 100.0f, 2000.0f);

    SetCameraTransform(CameraLocation, CameraRotation);
}

// 초기 카메라 설정으로 복귀
void FActorPreviewSceneOld::ResetCamera()
{
    CameraLocation = DefaultCameraLocation;
    CameraRotation = DefaultCameraRotation;
    CameraDistance = 400.0f;

    SetCameraTransform(CameraLocation, CameraRotation);
}

// 카메라 위치와 회전 설정
void FActorPreviewSceneOld::SetCameraTransform(FVector InLocation, FRotator InRotation)
{
    CameraLocation = InLocation - (InRotation.Vector() * CameraDistance);
    CameraComponent->SetWorldLocation(CameraLocation);
    CameraComponent->SetWorldRotation(InRotation);
}

void FActorPreviewSceneOld::UpdateCamera(TSharedRef<SViewport> InViewport)
{
    // 카메라 업데이트 로직: 필요 시 추가
}
