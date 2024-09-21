// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPreviewViewportClient.h"
#include "ActorPreviewViewport.h"

/// <summary>
/// FActorPreviewViewportClient 생성자 함수. 
/// 기본 뷰포트 클라이언트를 설정하고 미리보기 씬과 뷰포트를 초기화.
/// </summary>
/// <param name="InPreviewScene">미리보기 씬 객체.</param>
/// <param name="InPreviewViewport">미리보기 뷰포트 위젯.</param>
FActorPreviewViewportClient::FActorPreviewViewportClient(FAdvancedPreviewScene& InPreviewScene, const TSharedRef<SActorPreviewViewport>& InPreviewViewport)
	: FEditorViewportClient(nullptr, &InPreviewScene, StaticCastSharedRef<SEditorViewport>(InPreviewViewport))
	, PreviewViewportPtr(InPreviewViewport)
{
	// 기본적인 그리드, 피벗, 월드 박스 등의 그리기 설정
	DrawHelper.bDrawPivot = false;
	DrawHelper.bDrawWorldBox = false;
	DrawHelper.bDrawKillZ = false;
	DrawHelper.bDrawGrid = false;
	DrawHelper.GridColorAxis = FColor(80, 80, 80);
	DrawHelper.GridColorMajor = FColor(72, 72, 72);
	DrawHelper.GridColorMinor = FColor(64, 64, 64);
	DrawHelper.PerspectiveGridSize = UE_OLD_HALF_WORLD_MAX1;

	/**
	 * 기본 카메라 설정: 조명 모드로 설정
	 * 조명(Lighting)이 적용된 상태에서 씬을 렌더링하는 모드
	 * 언리얼 엔진의 뷰 모드(View Mode) 중 하나로, 조명, 그림자, 재질(Material), 반사(Reflection) 등 다양한 렌더링 요소가 모두 활성화된 상태에서 씬 랜더링
	 * - VMI_Lit 모드 기능
	 *		조명 효과: 씬 내의 조명(Light)이 오브젝트에 적용되어 현실감 있는 음영이 나타납니다.
	 *		그림자 표현: 오브젝트의 그림자가 조명에 따라 정확하게 표현됩니다.
	 *		재질 렌더링: 오브젝트의 머티리얼(Material)과 텍스처(Texture)가 제대로 렌더링됩니다.
	 *		반사 효과: 씬에 반사되는 빛이나 오브젝트가 표현됩니다.
	 */
	SetViewMode(VMI_Lit);

	/**
	 * 카메라 오브잇(Orbit) 기능은 카메라가 특정 지점을 중심으로 회전하는 기능을 의미
	 * 즉, 카메라가 한 점을 바라보며 그 점을 중심으로 원형 궤도를 그리면서 회전하는 방식
	 * 주로 3D 뷰포트에서 오브젝트를 다양한 각도에서 관찰하기 위해 사용하는 기능
	 */
	bUsingOrbitCamera = true;

	AdvancedPreviewScene = &InPreviewScene;
}

void FActorPreviewViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);
	// 미리보기 씬의 월드를 틱
	PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
}

/// <summary>
/// 키 입력 처리를 위한 함수.
/// </summary>
/// <param name="EventArgs">키 입력 이벤트 파라미터.</param>
/// <returns>입력이 처리되었는지 여부를 반환.</returns>
bool FActorPreviewViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	bool bHandled = FEditorViewportClient::InputKey(EventArgs);
	return bHandled;
}

/// <summary>
/// 축 입력 처리를 위한 함수. 마우스 축 또는 키보드 축 입력을 처리.
/// </summary>
/// <param name="InViewport">뷰포트 객체.</param>
/// <param name="DeviceId">입력 장치 ID.</param>
/// <param name="Key">입력된 키.</param>
/// <param name="Delta">변화량.</param>
/// <param name="DeltaTime">프레임 간 시간 차이.</param>
/// <param name="NumSamples">샘플 수.</param>
/// <param name="bGamepad">게임패드 여부.</param>
/// <returns>축 입력이 처리되었는지 여부를 반환.</returns>
bool FActorPreviewViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId DeviceId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	return FEditorViewportClient::InputAxis(InViewport, DeviceId, Key, Delta, DeltaTime, NumSamples, bGamepad);
}

/// <summary>
/// 뷰포트 카메라를 선택된 오브젝트의 바운드를 기준으로 이동.
/// </summary>
/// <param name="Bounds">선택된 오브젝트의 바운드.</param>
/// <param name="bInstant">즉시 이동할지 여부.</param>
/// <remarks>
/// 오브젝트 선택 시, 뷰포트 초기화 시, 또는 '포커스 맞추기' 기능을 사용할 때 호출
/// </remarks>
void FActorPreviewViewportClient::FocusViewportOnBounds(const FBoxSphereBounds& Bounds, bool bInstant)
{
	// 선택된 오브젝트의 중심을 기준으로 카메라의 위치 계산
	const FVector Position = Bounds.Origin;
	const float Radius = Bounds.SphereRadius;
	const float DistanceFromSphere = Radius / FMath::Tan(FMath::DegreesToRadians(ViewFOV / 2.0f));

	// 카메라의 오프셋 벡터 계산
	FViewportCameraTransform& ViewTransform = GetViewTransform();
	FVector CameraOffsetVector = ViewTransform.GetRotation().Vector() * -DistanceFromSphere;

	// 카메라를 선택된 오브젝트의 중심으로 이동시키고, 부드러운 전환 여부를 설정
	ViewTransform.SetLookAt(Position);
	ViewTransform.TransitionToLocation(Position + CameraOffsetVector, PreviewViewportPtr, bInstant);
}