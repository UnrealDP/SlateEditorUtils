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

	// Getter & Setter
	/** Returns the preview scene */
	TSharedRef<class FAdvancedPreviewScene> GetPreviewScene() { return PreviewScene.ToSharedRef(); }
	UWorld* GetPreviewWorld() const { return GetWorld(); }
	AActor* GetActor() const { return Actor; }
	// End of Getter & Setter

	void SpawnActorInPreviewWorld(UClass* ActorClass);

	template <typename T>
	T* ReplaceComponentToActor()
	{
		T* NewComponent = nullptr;

		if (Actor)
		{
			TArray<T*> OldComponents;
			Actor->GetComponents(OldComponents);
			for (T* Comp : OldComponents)
			{
				if (Comp)
				{
					Comp->DestroyComponent();
				}
			}

			NewComponent = NewObject<T>(Actor, NAME_None, RF_Transient);
			Actor->AttachToComponent(NewComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		else
		{
			NewComponent = NewObject<T>(GetTransientPackage(), NAME_None, RF_Transient);
		}
		
		return NewComponent;
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
	 * 프리뷰 월드(레벨)을 설정
	 * @param InWorld 설정할 World.
	 * @return 성공적으로 설정되었으면 true, 그렇지 않으면 false를 반환.
	 * !!!!!! 프리뷰는 월드 바꿔도 적용되지 않음 !!!!!!
	 */
	//bool SetPreviewWorld(class UWorld* InWorld);

	/**
	 * FGCObject 인터페이스 구현. 가비지 컬렉터에서 관리할 객체를 참조 목록에 추가.
	 * @param Collector 참조할 객체 수집기.
	 */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	void InitCamera();

	/** Call back for when the user changes preview scene settings in the UI */
	void OnAssetViewerSettingsChanged(const FName& InPropertyName);

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

	TObjectPtr<AActor> Actor;
};