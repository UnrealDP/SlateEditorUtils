// Fill out your copyright notice in the Description page of Project Settings.


#include "SStringComboBoxWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SStringComboBoxWidget::Construct(const FArguments& InArgs)
{
    OptionsSource = MakeShared<TArray<TSharedPtr<FString>>>();
    OnSelectionChangedEvent = InArgs._OnSelectionChanged;

    // 문자열 옵션 리스트 초기화
    for (const FString& Option : InArgs._Options)
    {
        OptionsSource->Add(MakeShared<FString>(Option));
    }

    // 기본 선택 항목
    if (OptionsSource->Num() > 0)
    {
        SelectedOption = (*OptionsSource)[0];
    }

    ChildSlot
        [
            SNew(SComboBox<TSharedPtr<FString>>)
                .OptionsSource(OptionsSource.Get())
                .InitiallySelectedItem(SelectedOption)
                .OnGenerateWidget(this, &SStringComboBoxWidget::GenerateComboBoxItem)
                .OnSelectionChanged(this, &SStringComboBoxWidget::OnComboBoxSelectionChanged)
                [
                    SNew(STextBlock)
                        .Text(this, &SStringComboBoxWidget::GetSelectedText)
                ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


/// <summary>
/// 선택 값을 강제로 설정하는 함수.
/// </summary>
/// <param name="NewValue">새롭게 설정할 값.</param>
void SStringComboBoxWidget::SetSelectedValue(const FString& NewValue)
{
    for (const TSharedPtr<FString>& Option : *OptionsSource)
    {
        if (*Option == NewValue)
        {
            SelectedOption = Option;
            break;
        }
    }
}

/// <summary>
/// 콤보 박스 항목을 생성하는 함수.
/// </summary>
/// <param name="InOption">생성할 항목의 문자열 포인터.</param>
/// <returns>생성된 위젯.</returns>
TSharedRef<SWidget> SStringComboBoxWidget::GenerateComboBoxItem(TSharedPtr<FString> InOption)
{
    return SNew(STextBlock).Text(FText::FromString(*InOption));
}

/// <summary>
/// 콤보 박스 선택 변경 이벤트 처리.
/// </summary>
/// <param name="NewSelection">새롭게 선택된 항목의 문자열 포인터.</param>
/// <param name="SelectInfo">선택 정보 타입.</param>
void SStringComboBoxWidget::OnComboBoxSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
    if (NewSelection.IsValid())
    {
        SelectedOption = NewSelection;

        // 선택된 이벤트가 바인딩되어 있는 경우에만 호출
        if (OnSelectionChangedEvent.IsBound())
        {
            OnSelectionChangedEvent.Execute(NewSelection, SelectInfo);
        }
    }
}

/// <summary>
/// 선택된 값 반환 (콤보 박스에서 사용).
/// </summary>
/// <returns>선택된 문자열 값. 값이 없을 경우 빈 텍스트를 반환.</returns>
FText SStringComboBoxWidget::GetSelectedText() const
{
    return SelectedOption.IsValid() ? FText::FromString(*SelectedOption) : FText::GetEmpty();
}