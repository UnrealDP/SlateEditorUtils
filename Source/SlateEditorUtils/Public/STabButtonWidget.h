// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "Widgets/SCompoundWidget.h"
//
//DECLARE_DELEGATE_OneParam(FOnTabChanged, int32);
//
///**
// * STabButtonWidget는 N개 탭 버튼을 생성하고 탭 변경 이벤트 호출해주는 위젯.
// */
//class SLATEEDITORUTILS_API STabButtonWidget : public SCompoundWidget
//{
//public:
//	SLATE_BEGIN_ARGS(STabButtonWidget) {}
//		SLATE_ARGUMENT(TArray<FString>, TabNames)  // 탭 이름들
//		SLATE_EVENT(FOnTabChanged, OnTabChanged)   // 탭 변경 이벤트
//	SLATE_END_ARGS()
//
//	/** Constructs this widget with InArgs */
//	void Construct(const FArguments& InArgs);
//
//private:
//    /**
//     * 탭 버튼들을 생성하는 함수.
//     * @return 생성된 탭 버튼들을 포함하는 위젯.
//     */
//    TSharedRef<SWidget> CreateTabButtons();
//
//    /**
//    * 특정 탭이 선택되었을 때 호출되는 함수.
//    * @param TabIndex 선택된 탭의 인덱스.
//    */
//    void OnTabSelected(int32 TabIndex);
//
//private:
//    /** 선택된 탭의 인덱스 */
//    int32 SelectedTabIndex;
//
//    /** 탭 이름 목록 */
//    TArray<FString> TabNames;
//
//    /** 탭 변경 이벤트 */
//    FOnTabChanged OnTabChanged;
//};

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FOnTabChanged, int32);

/**
 * STabButtonWidget는 N개 탭 버튼을 생성하고 탭 변경 이벤트 호출해주는 위젯.
 */
class SLATEEDITORUTILS_API STabButtonWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(STabButtonWidget) {}
        SLATE_ARGUMENT(TArray<FString>, TabNames)  // 탭 이름들
            SLATE_EVENT(FOnTabChanged, OnTabChanged)   // 탭 변경 이벤트
    SLATE_END_ARGS()

        /** Constructs this widget with InArgs */
        void Construct(const FArguments& InArgs);

private:
    TSharedRef<SWidget> CreateTabButtons();
    void OnTabSelected(int32 TabIndex);

private:
    int32 SelectedTabIndex;
    TArray<FString> TabNames;
    FOnTabChanged OnTabChanged;
};
