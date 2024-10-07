// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPreviewViewport.h"
#include "ActorPreviewViewportClient.h"
#include "AssetViewerSettings.h"
#include "Slate/SceneViewport.h"

/// <summary>
/// 뷰포트 위젯을 생성하는 함수.
/// </summary>
/// <param name="InArgs">SLATE의 아규먼트.</param>
void SActorPreviewViewport::Construct(const FArguments& InArgs)
{
	PreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));

	SEditorViewport::Construct(SEditorViewport::FArguments());
}

/// <summary>
/// 소멸자. 프리뷰 어셋을 제거하고 참조를 정리.
/// </summary>
SActorPreviewViewport::~SActorPreviewViewport()
{
	ClearPreviewAsset();
}

void SActorPreviewViewport::SpawnActorInPreviewWorld(UClass* ActorClass)
{
	if (ActorClass && GetWorld())
	{
		UWorld* PreviewWorld = GetWorld();

		if (PreviewWorld)
		{
			FTransform ActorTransform;
			ActorTransform.SetLocation(FVector(0.0f, 0.0f, 100.0f));  // 원하는 위치 설정

			// 프리뷰 월드에 액터 생성
			Actor = PreviewWorld->SpawnActor<AActor>(ActorClass, ActorTransform);
			if (Actor)
			{
				UE_LOG(LogTemp, Log, TEXT("Actor %s created in the preview world!"), *Actor->GetName());
			}
		}
	}
}

/// <summary>
/// 가비지 컬렉터에서 참조할 객체들을 수집.
/// </summary>
/// <param name="Collector">참조할 객체 수집기.</param>
void SActorPreviewViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
	// 가비지 컬렉터가 미리보기 메쉬 컴포넌트를 수집하도록 추가
	Collector.AddReferencedObject(PreviewMeshComponent);
}

/// <summary>
/// 프리뷰 어셋을 설정. 스태틱 메쉬 또는 스켈레탈 메쉬만 지원.
/// </summary>
/// <param name="InAsset">설정할 스태틱 메쉬 또는 스켈레탈 메쉬.</param>
/// <returns>성공적으로 설정되었으면 true, 그렇지 않으면 false를 반환.</returns>
bool SActorPreviewViewport::SetPreviewAsset(UObject* InAsset)
{
	// 기존의 미리보기 메쉬를 제거
	ClearPreviewAsset();

	if (UStaticMesh* StaticMesh = Cast<UStaticMesh>(InAsset))
	{
		UStaticMeshComponent* NewStaticMeshComponent = ReplaceComponentToActor<UStaticMeshComponent>();
		PreviewScene->AddComponent(NewStaticMeshComponent, FTransform::Identity);
		NewStaticMeshComponent->SetStaticMesh(StaticMesh);
		PreviewMeshComponent = NewStaticMeshComponent;
		return true;
	}
	else if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(InAsset))
	{
		USkeletalMeshComponent* NewSkeletalMeshComponent = ReplaceComponentToActor<USkeletalMeshComponent>();
		PreviewScene->AddComponent(NewSkeletalMeshComponent, FTransform::Identity);
		NewSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
		PreviewMeshComponent = NewSkeletalMeshComponent;
		SkeletalMeshComponent = NewSkeletalMeshComponent;

		// 기본 애니메이션 재생
		NewSkeletalMeshComponent->PlayAnimation(nullptr, true);
		AnimInstance = NewSkeletalMeshComponent->GetAnimInstance();
		return true;
	}

	return false;
}

/// <summary>
/// 프리뷰 월드(레벨)을 설정
/// </summary>
/// <param name="InAsset">설정할 World.</param>
/// <returns>성공적으로 설정되었으면 true, 그렇지 않으면 false를 반환.</returns>
/// !!!!!! 프리뷰는 월드 바꿔도 적용되지 않음 !!!!!!
//bool SActorPreviewViewport::SetPreviewWorld(class UWorld* InWorld)
//{
//	UWorld* CrrWorld = GetWorld();
//	if (!CrrWorld)
//	{
//		return false;
//	}
//
//	ULevel* CrrLevel = InWorld->GetCurrentLevel();
//	if (!CrrLevel)
//	{
//		return false;
//	}
//
//	CrrWorld->SetCurrentLevel(CrrLevel);
//	RefreshViewport();
//	return true;
//}

/// <summary>
/// 현재 설정된 프리뷰 어셋을 제거.
/// </summary>
void SActorPreviewViewport::ClearPreviewAsset()
{
	// 설정된 프리뷰 메쉬가 있으면 제거하고, 컴포넌트와 참조를 초기화
	if (PreviewMeshComponent.IsValid())
	{
		PreviewScene->RemoveComponent(PreviewMeshComponent.Get());
		PreviewMeshComponent.Reset();
		SkeletalMeshComponent.Reset();
		AnimInstance.Reset();
	}
}

/// <summary>
/// 스켈레탈 메쉬의 애니메이션을 업데이트.
/// </summary>
/// <param name="DeltaTime">틱당 경과 시간.</param>
void SActorPreviewViewport::UpdateAnimation(float DeltaTime)
{
	if (SkeletalMeshComponent.IsValid() && AnimInstance.IsValid())
	{
		AnimInstance->UpdateAnimation(DeltaTime, false);
	}
}

/// <summary>
/// 뷰포트 UI 위젯을 반환.
/// </summary>
/// <returns>현재 뷰포트 위젯의 참조.</returns>
TSharedRef<class SEditorViewport> SActorPreviewViewport::GetViewportWidget()
{
	return SharedThis(this);
}

/// <summary>
/// 확장자를 반환. 현재 사용되지 않음.
/// </summary>
/// <returns>FExtender 객체의 공유 포인터.</returns>
TSharedPtr<FExtender> SActorPreviewViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

/// <summary>
/// 플로팅 버튼 클릭 시 호출되는 함수.
/// </summary>
void SActorPreviewViewport::OnFloatingButtonClicked()
{
}

/// <summary>
/// SEditorViewport 인터페이스에서 뷰포트 클라이언트를 생성.
/// </summary>
/// <returns>생성된 뷰포트 클라이언트.</returns>
TSharedRef<FEditorViewportClient> SActorPreviewViewport::MakeEditorViewportClient()
{
	ViewportClient = MakeShareable(new FActorPreviewViewportClient(*PreviewScene, SharedThis(this)));
	UAssetViewerSettings::Get()->OnAssetViewerSettingsChanged().AddRaw(this, &SActorPreviewViewport::OnAssetViewerSettingsChanged);

	/*
	* Realtime Mode : 뷰포트가 실시간 렌더링을 사용하는지 여부를 설정
	* SetRealtime 에서 bShouldInvalidateViewportWidget가 true로 설정되어 뷰포트가 다시 그려지도록 요청
	* 즉, 뷰포트가 즉시 화면에 반영되지 않는다면, 이 플래그가 변경된 후 다음 프레임에서 화면을 갱신
	* bIsRealtime이 false로 설정되면, 뷰포트는 실시간으로 화면을 갱신하지 않고, 특정 이벤트나 요청에 의해서만 갱신 (Focus, 카메라 움직임 등)
	*/
	ViewportClient->SetRealtime(true);

	// 카메라 초기 위치와 회전을 설정
	InitCamera();
	ViewportClient->bSetListenerPosition = false;
	ViewportClient->EngineShowFlags.EnableAdvancedFeatures();
	ViewportClient->EngineShowFlags.SetLighting(true);
	ViewportClient->EngineShowFlags.SetIndirectLightingCache(true);
	ViewportClient->EngineShowFlags.SetPostProcessing(true);
	ViewportClient->Invalidate();
	//ViewportClient->VisibilityDelegate.BindSP(this, &SMaterialEditor3DPreviewViewport::IsVisible);

	return ViewportClient.ToSharedRef();
}

void SActorPreviewViewport::InitCamera()
{
	FVector CameraLocation(-400.0f, 120.0f, 150.0f);
	FVector TargetLocation(0.0f, 120.0f, 100.0f);
	FRotator CameraRotation = (TargetLocation - CameraLocation).Rotation();

	ViewportClient->SetViewLocation(CameraLocation);
	ViewportClient->SetViewRotation(CameraRotation);
}

/** Call back for when the user changes preview scene settings in the UI */
void SActorPreviewViewport::OnAssetViewerSettingsChanged(const FName& InPropertyName)
{
	if (!PreviewScene)
	{
		return;
	}

	if (InPropertyName == GET_MEMBER_NAME_CHECKED(FPreviewSceneProfile, bRotateLightingRig) || InPropertyName == NAME_None)
	{
		UAssetViewerSettings* Settings = UAssetViewerSettings::Get();
		const int32 ProfileIndex = PreviewScene->GetCurrentProfileIndex();
		if (Settings->Profiles.IsValidIndex(ProfileIndex) &&
			Settings->Profiles[ProfileIndex].bRotateLightingRig
			&& !ViewportClient->IsRealtime())
		{
			ViewportClient->SetRealtime(true);
		}
	}
}

/// <summary>
/// 뷰포트 갱신 함수. 프리뷰 컴포넌트의 상태를 갱신.
/// </summary>
void SActorPreviewViewport::RefreshViewport()
{
	// reregister the preview components, so if the preview material changed it will be propagated to the render thread
	if (PreviewMeshComponent != nullptr)
	{
		PreviewMeshComponent->MarkRenderStateDirty();
	}
	SceneViewport->InvalidateDisplay();

	if (ViewportClient.IsValid() && PreviewScene.IsValid())
	{
		UAssetViewerSettings* Settings = UAssetViewerSettings::Get();
		const int32 ProfileIndex = PreviewScene->GetCurrentProfileIndex();
		if (Settings->Profiles.IsValidIndex(ProfileIndex))
		{
			PreviewScene->UpdateScene(Settings->Profiles[ProfileIndex]);
		}
		
		if (!ViewportClient->IsRealtime())
		{
			ViewportClient->SetRealtime(true);
		}
	}
}

/// <summary>
/// 뷰포트가 특정 탭에 추가할 때 Parent 등록하는 함수
/// </summary>
/// <param name="OwnerTab">추가될 탭의 참조.</param>
void SActorPreviewViewport::OnAddedToTab(const TSharedRef<SDockTab>& OwnerTab)
{
	ParentTab = OwnerTab;
}