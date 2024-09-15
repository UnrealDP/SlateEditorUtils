// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"

/**
 * 
 */
class SLATEEDITORUTILS_API SStringComboBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStringComboBoxWidget)	{}
		SLATE_ARGUMENT(TArray<FString>, Options)  // 초기화 시 사용자가 제공하는 문자열 리스트
            SLATE_EVENT(SComboBox<TSharedPtr<FString>>::FOnSelectionChanged, OnSelectionChanged)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

    // 현재 선택된 값을 반환하는 함수
    inline FString GetSelectedValue() const
    {
        return SelectedOption.IsValid() ? *SelectedOption : FString();
    }

    // 선택 값을 강제로 설정하는 함수
    void SetSelectedValue(const FString& NewValue);

private:
    TSharedPtr<TArray<TSharedPtr<FString>>> OptionsSource;
    TSharedPtr<FString> SelectedOption;
    SComboBox<TSharedPtr<FString>>::FOnSelectionChanged OnSelectionChangedEvent;

    // 콤보 박스 아이템 생성
    TSharedRef<SWidget> GenerateComboBoxItem(TSharedPtr<FString> InOption);

    // 콤보 박스 선택 변경 이벤트 처리
    void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

    // 선택된 값 반환 (콤보 박스에서 사용)
    FText GetSelectedText() const;
};
