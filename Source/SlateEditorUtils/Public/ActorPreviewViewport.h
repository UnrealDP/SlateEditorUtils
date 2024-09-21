// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"

/**
* 액터용 프리뷰 뷰포트 위젯.
* 스태틱 메쉬 및 스켈레탈 메쉬를 미리보기하며, 스켈레탈 메쉬의 경우 애니메이션을 재생할 수 있습니다.
*/
class SLATEEDITORUTILS_API SActorPreviewViewport : public SEditorViewport, public FGCObject, public ICommonEditorViewportToolbarInfoProvider
{
public:
	SLATE_BEGIN_ARGS(SActorPreviewViewport) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	~SActorPreviewViewport();

	virtual FString GetReferencerName() const override
	{
		return TEXT("SActorPreviewViewport");
	}

	/**
	 * 프리뷰 어셋을 설정. 스태틱 메쉬 또는 스켈레탈 메쉬만 지원.
	 * @param InAsset 설정할 스태틱 메쉬 또는 스켈레탈 메쉬.
	 * @return 성공적으로 설정되었으면 true, 그렇지 않으면 false를 반환.
	 */
	bool SetPreviewAsset(UObject* InAsset);

	// 현재 설정된 프리뷰 어셋을 제거하는 함수.
	void ClearPreviewAsset();

	/**
	 * FGCObject 인터페이스 구현. 가비지 컬렉터에서 관리할 객체를 참조 목록에 추가.
	 * @param Collector 참조할 객체 수집기.
	 */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	/** Returns the preview scene */
	TSharedRef<class FAdvancedPreviewScene> GetPreviewScene() { return PreviewScene.ToSharedRef(); }

	/** Updates the skeletal mesh animation */
	void UpdateAnimation(float DeltaTime);


	void RefreshViewport();

	void OnAddedToTab(const TSharedRef<SDockTab>& OwnerTab);

	// ICommonEditorViewportToolbarInfoProvider interface
	virtual TSharedRef<class SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;
	// End of ICommonEditorViewportToolbarInfoProvider interface

protected:
	/**
	 * SEditorViewport 인터페이스. 뷰포트 클라이언트를 생성.
	 * @return 생성된 뷰포트 클라이언트.
	 */
	virtual TSharedRef<class FEditorViewportClient> MakeEditorViewportClient() override;

private:
	// 프리뷰 씬을 생성하고 초기화하는 함수.
	void CreatePreviewScene();

	/** The parent tab where this viewport resides */
	TWeakPtr<SDockTab> ParentTab;

	/** Mesh component (static or skeletal) */
	TWeakObjectPtr<class UMeshComponent> PreviewMeshComponent;

	/** Reference to the preview scene */
	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;

	/** Viewport client */
	TSharedPtr<class FActorPreviewViewportClient> ViewportClient;

	/** Skeletal mesh component */
	TWeakObjectPtr<class USkeletalMeshComponent> SkeletalMeshComponent;

	/** Animation instance */
	TWeakObjectPtr<class UAnimInstance> AnimInstance;
};