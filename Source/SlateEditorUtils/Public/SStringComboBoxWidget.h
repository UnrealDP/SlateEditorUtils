// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"

/**
 * SStringComboBoxWidget는 문자열 옵션을 제공하는 콤보 박스를 생성하는 위젯
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

    /**
     * 현재 선택된 값을 반환합니다.
     * @return 선택된 문자열 값
     */
    inline FString GetSelectedValue() const
    {
        return SelectedOption.IsValid() ? *SelectedOption : FString();
    }

    /**
     * 선택된 값을 강제로 설정하는 함수입니다.
     * @param NewValue 새롭게 설정할 문자열 값
     */
    void SetSelectedValue(const FString& NewValue);

private:
    // 콤보 박스에 표시될 문자열 옵션 리스트
    TSharedPtr<TArray<TSharedPtr<FString>>> OptionsSource;

    // 현재 선택된 옵션
    TSharedPtr<FString> SelectedOption;

    // 선택 변경 이벤트 핸들러
    SComboBox<TSharedPtr<FString>>::FOnSelectionChanged OnSelectionChangedEvent;

    /**
     * 콤보 박스의 아이템을 생성하는 함수입니다.
     * @param InOption 콤보 박스에 표시할 문자열 옵션
     * @return 생성된 위젯
     */
    TSharedRef<SWidget> GenerateComboBoxItem(TSharedPtr<FString> InOption);

    /**
     * 콤보 박스 선택 변경 이벤트를 처리하는 함수입니다.
     * @param NewSelection 새롭게 선택된 문자열 옵션
     * @param SelectInfo 선택 정보
     */
    void OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);

    /**
     * 선택된 값을 반환하는 함수입니다. 콤보 박스 내부에서 사용됩니다.
     * @return 선택된 값을 나타내는 FText 객체
     */
    FText GetSelectedText() const;
};
