// Fill out your copyright notice in the Description page of Project Settings.
#include "ActorPreviewViewportClient.h"
#include "ActorPreviewViewport.h"

FActorPreviewViewportClient::FActorPreviewViewportClient(FAdvancedPreviewScene& InPreviewScene, const TSharedRef<SActorPreviewViewport>& InPreviewViewport)
	: FEditorViewportClient(nullptr, &InPreviewScene, StaticCastSharedRef<SEditorViewport>(InPreviewViewport))
	, PreviewViewportPtr(InPreviewViewport)
{
	// Setup defaults for the common draw helper.
	DrawHelper.bDrawPivot = false;
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = false;
	DrawHelper.GridColorAxis = FColor(80, 80, 80);
	DrawHelper.GridColorMajor = FColor(72, 72, 72);
	DrawHelper.GridColorMinor = FColor(64, 64, 64);
	DrawHelper.PerspectiveGridSize = UE_OLD_HALF_WORLD_MAX1;

	// 기본 카메라 설정
	SetViewMode(VMI_Lit);
	bUsingOrbitCamera = true;

	AdvancedPreviewScene = &InPreviewScene;
}

void FActorPreviewViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);
	PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
}

bool FActorPreviewViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	bool bHandled = FEditorViewportClient::InputKey(EventArgs);
	return bHandled;
}

bool FActorPreviewViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	return FEditorViewportClient::InputAxis(InViewport, DeviceId, Key, Delta, DeltaTime, NumSamples, bGamepad);
}

void FActorPreviewViewportClient::FocusViewportOnBounds(const FBoxSphereBounds& Bounds, bool bInstant)
{
	// 카메라가 선택된 오브젝트를 바라보도록 이동
	const FVector Position = Bounds.Origin;
	const float Radius = Bounds.SphereRadius;
	const float DistanceFromSphere = Radius / FMath::Tan(FMath::DegreesToRadians(ViewFOV / 2.0f));

	FViewportCameraTransform& ViewTransform = GetViewTransform();
	FVector CameraOffsetVector = ViewTransform.GetRotation().Vector() * -DistanceFromSphere;

	ViewTransform.SetLookAt(Position);
	ViewTransform.TransitionToLocation(Position + CameraOffsetVector, PreviewViewportPtr, bInstant);
}