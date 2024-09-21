// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "ActorPreviewScene.h"

/**
 * 뷰포트 클라이언트: 카메라 제어 및 입력 처리
 */
class SLATEEDITORUTILS_API FActorPreviewViewportClient : public FEditorViewportClient
{
public:
	FActorPreviewViewportClient(FAdvancedPreviewScene& InPreviewScene, const TSharedRef<class SActorPreviewViewport>& InPreviewViewport);

	// FEditorViewportClient interface
	virtual void Tick(float DeltaSeconds) override;
	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
	virtual bool InputAxis(FViewport* InViewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad) override;

	/** Focus the viewport on the preview object */
	void FocusViewportOnBounds(const FBoxSphereBounds& Bounds, bool bInstant = false);

private:
	/** Reference back to the SActorPreviewViewport */
	TWeakPtr<class SActorPreviewViewport> PreviewViewportPtr;

	/** Advanced preview scene */
	FAdvancedPreviewScene* AdvancedPreviewScene;
};