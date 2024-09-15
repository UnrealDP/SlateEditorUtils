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


// 선택 값을 강제로 설정하는 함수
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

// 콤보 박스 아이템 생성
TSharedRef<SWidget> SStringComboBoxWidget::GenerateComboBoxItem(TSharedPtr<FString> InOption)
{
    return SNew(STextBlock).Text(FText::FromString(*InOption));
}

// 콤보 박스 선택 변경 이벤트 처리
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

// 선택된 값 반환 (콤보 박스에서 사용)
FText SStringComboBoxWidget::GetSelectedText() const
{
    return SelectedOption.IsValid() ? FText::FromString(*SelectedOption) : FText::GetEmpty();
}