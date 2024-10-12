// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"

class FAdvancedPreviewScene;
class SEditorViewport;
class FEditorViewportClient;
class FActorPreviewViewportClient;
class UMeshComponent;
class USkeletalMeshComponent;
class UAnimInstance;

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
	TSharedRef<FAdvancedPreviewScene> GetPreviewScene() { return PreviewScene.ToSharedRef(); }
	UWorld* GetPreviewWorld() const { return GetWorld(); }
	AActor* GetActor() const { return Actor; }
	// End of Getter & Setter

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
	 * FGCObject 인터페이스 구현. 가비지 컬렉터에서 관리할 객체를 참조 목록에 추가.
	 * @param Collector 참조할 객체 수집기.
	 */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	/**
	 * 액터를 프리뷰 월드에 생성하는 함수.
	 * @param ActorClass 생성할 액터 클래스.
	 */
	TObjectPtr<AActor> SpawnActorInPreviewWorld(UClass* ActorClass);

	/**
	 * 액터 교체
	 * @param ActorClass 교체할 액터 클래스.
	 */
	TObjectPtr<AActor> ReplaceActorPreviewWorld(UClass* ActorClass);

	/**
	 * 스켈레탈 메쉬 설정
	 * @param SkeletalMesh 설정할 스켈레탈 메쉬.
	 */
	void SetSkeletalMesh(USkeletalMesh* SkeletalMesh);

	/**
	 * 스켈레탈 메쉬 교체
	 * @param SkeletalMesh 교체할 스켈레탈 메쉬.
	 */
	void ReplaceSkeletalMesh(USkeletalMesh* SkeletalMesh);

	/**
	 * UAnimInstance 설정
	 * @param NewAnimInterfaceClass 설정할 UAnimInstance 클래스.
	 */	
	void SetAnimInstance(UClass* AnimInstanceClass);

	/**
	 * UAnimInstance 교체
	 * @param NewAnimInterfaceClass 교체할 UAnimInstance 클래스.
	 */
	void ReplaceAnimInstancePreviewWorld(UClass* AnimInstanceClass);

	// 현재 설정된 프리뷰 어셋을 제거하는 함수.
	void ClearPreviewAsset();

	/**
	 * 프리뷰 월드(레벨)을 설정
	 * @param InWorld 설정할 World.
	 * @return 성공적으로 설정되었으면 true, 그렇지 않으면 false를 반환.
	 * !!!!!! 프리뷰는 월드 바꿔도 적용되지 않음 !!!!!!
	 */
	//bool SetPreviewWorld(class UWorld* InWorld);

	void InitCamera();

	/** Call back for when the user changes preview scene settings in the UI */
	void OnAssetViewerSettingsChanged(const FName& InPropertyName);

	/** Updates the skeletal mesh animation */
	void UpdateAnimation(float DeltaTime);

	void RefreshViewport();

	void OnAddedToTab(const TSharedRef<SDockTab>& OwnerTab);

	// ICommonEditorViewportToolbarInfoProvider interface
	virtual TSharedRef<SEditorViewport> GetViewportWidget() override;
	virtual TSharedPtr<FExtender> GetExtenders() const override;
	virtual void OnFloatingButtonClicked() override;
	// End of ICommonEditorViewportToolbarInfoProvider interface

protected:
	/**
	 * SEditorViewport 인터페이스. 뷰포트 클라이언트를 생성.
	 * @return 생성된 뷰포트 클라이언트.
	 */
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:

	/** The parent tab where this viewport resides */
	TWeakPtr<SDockTab> ParentTab;

	/** Reference to the preview scene */
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;

	/** Viewport client */
	TSharedPtr<FActorPreviewViewportClient> ViewportClient;

	/** Mesh component (static or skeletal) */
	TWeakObjectPtr<UMeshComponent> PreviewMeshComponent;

	/** Animation instance */
	UAnimInstance* AnimInstance;

	TObjectPtr<AActor> Actor;
};