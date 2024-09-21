// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPreviewViewport.h"
//#include "AdvancedPreviewScene.h"
#include "ActorPreviewViewportClient.h"
//#include "SEditorViewport.h"
//#include "Engine/StaticMesh.h"
//#include "Components/SkeletalMeshComponent.h"
//#include "Components/StaticMeshComponent.h"
//#include "Animation/AnimSingleNodeInstance.h"
//#include "Widgets/SCompoundWidget.h"
//#include "EditorViewportClient.h"
//#include "PreviewScene.h"
//#include "Animation/AnimInstance.h"

/// <summary>
/// 뷰포트 위젯을 생성하는 함수.
/// </summary>
/// <param name="InArgs">SLATE의 아규먼트.</param>
void SActorPreviewViewport::Construct(const FArguments& InArgs)
{
	CreatePreviewScene();

	SEditorViewport::Construct(SEditorViewport::FArguments());
}

/// <summary>
/// 소멸자. 프리뷰 어셋을 제거하고 참조를 정리.
/// </summary>
SActorPreviewViewport::~SActorPreviewViewport()
{
	ClearPreviewAsset();
}

/// <summary>
/// 가비지 컬렉터에서 참조할 객체들을 수집.
/// </summary>
/// <param name="Collector">참조할 객체 수집기.</param>
void SActorPreviewViewport::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(PreviewMeshComponent);
}

/// <summary>
/// 프리뷰 씬을 생성하고 초기화.
/// </summary>
void SActorPreviewViewport::CreatePreviewScene()
{
	PreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
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
		UStaticMeshComponent* NewStaticMeshComponent = NewObject<UStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient);
		NewStaticMeshComponent->SetStaticMesh(StaticMesh);
		PreviewScene->AddComponent(NewStaticMeshComponent, FTransform::Identity);
		PreviewMeshComponent = NewStaticMeshComponent;
		return true;
	}
	else if (USkeletalMesh* SkeletalMesh = Cast<USkeletalMesh>(InAsset))
	{
		USkeletalMeshComponent* NewSkeletalMeshComponent = NewObject<USkeletalMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient);
		NewSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
		PreviewScene->AddComponent(NewSkeletalMeshComponent, FTransform::Identity);
		PreviewMeshComponent = NewSkeletalMeshComponent;
		SkeletalMeshComponent = NewSkeletalMeshComponent;

		NewSkeletalMeshComponent->PlayAnimation(nullptr, true); // 기본 애니메이션 재생
		AnimInstance = NewSkeletalMeshComponent->GetAnimInstance();
		return true;
	}

	return false;
}

/// <summary>
/// 현재 설정된 프리뷰 어셋을 제거.
/// </summary>
void SActorPreviewViewport::ClearPreviewAsset()
{
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

TSharedRef<class SEditorViewport> SActorPreviewViewport::GetViewportWidget()
{
	return SharedThis(this);
}

TSharedPtr<FExtender> SActorPreviewViewport::GetExtenders() const
{
	TSharedPtr<FExtender> Result(MakeShareable(new FExtender));
	return Result;
}

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
	ViewportClient->SetRealtime(true);

	ViewportClient->SetViewLocation(FVector::ZeroVector);
	ViewportClient->SetViewRotation(FRotator(-15.0f, -90.0f, 0.0f));
	ViewportClient->SetViewLocationForOrbiting(FVector::ZeroVector);
	ViewportClient->bSetListenerPosition = false;
	ViewportClient->EngineShowFlags.EnableAdvancedFeatures();
	ViewportClient->EngineShowFlags.SetLighting(true);
	ViewportClient->EngineShowFlags.SetIndirectLightingCache(true);
	ViewportClient->EngineShowFlags.SetPostProcessing(true);
	ViewportClient->Invalidate();
	/*ViewportClient->SetViewLocation(FVector::ZeroVector);
	ViewportClient->SetViewRotation(FRotator(-15.0f, -90.0f, 0.0f));
	ViewportClient->SetViewLocationForOrbiting(FVector::ZeroVector);
	ViewportClient->bSetListenerPosition = false;
	ViewportClient->EngineShowFlags.EnableAdvancedFeatures();
	ViewportClient->EngineShowFlags.SetLighting(true);
	ViewportClient->EngineShowFlags.SetIndirectLightingCache(true);
	ViewportClient->EngineShowFlags.SetPostProcessing(true);
	ViewportClient->Invalidate();
	ViewportClient->VisibilityDelegate.BindSP(this, &SMaterialEditor3DPreviewViewport::IsVisible);*/

	return ViewportClient.ToSharedRef();
}

void SActorPreviewViewport::RefreshViewport()
{
	// reregister the preview components, so if the preview material changed it will be propagated to the render thread
	if (PreviewMeshComponent != nullptr)
	{
		PreviewMeshComponent->MarkRenderStateDirty();
	}
	//SceneViewport->InvalidateDisplay();

	if (ViewportClient.IsValid() && PreviewScene.IsValid())
	{
		const int32 ProfileIndex = PreviewScene->GetCurrentProfileIndex();
		//PreviewScene->UpdateScene(FPreviewSceneProfile());
		if (!ViewportClient->IsRealtime())
		{
			ViewportClient->SetRealtime(true);
		}
	}
}

void SActorPreviewViewport::OnAddedToTab(const TSharedRef<SDockTab>& OwnerTab)
{
	ParentTab = OwnerTab;
}