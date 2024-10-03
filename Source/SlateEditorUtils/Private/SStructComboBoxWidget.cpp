// Fill out your copyright notice in the Description page of Project Settings.


#include "SStructComboBoxWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

/// <summary>
/// 위젯을 초기화하는 함수. 콤보 박스를 구성하고, 기본 선택 항목을 설정.
/// </summary>
/// <param name="InArgs">구성에 필요한 인자들.</param>
void SStructComboBoxWidget::Construct(const FArguments& InArgs)
{
    // 이거 절대 다른 라인으로 내리지 말것 !!!!
    // void SStructComboBoxWidget::PopulateStructOptions(UScriptStruct* BaseStruct, bool bIncludeBaseStruct)
    // 위 함수 안에서 사용되니까 절대 PopulateStructOptions(InArgs._BaseStruct, bIncludeBaseStruct); 아래로 내리면 안됨
    StructOptions = MakeShared<TArray<UScriptStruct*>>();

    OnSelectionChangedEvent = InArgs._OnSelectionChanged;

    // bIncludeBaseStruct 옵션을 기본값으로 false로 설정
    const bool bIncludeBaseStruct = InArgs._bIncludeBaseStruct;

    // 사용자가 제공한 베이스 struct로부터 자식 struct를 찾아 목록을 구성
    PopulateStructOptions(InArgs._BaseStruct, bIncludeBaseStruct);

    // 기본 선택 항목
    if (StructOptions->Num() > 0)
    {
        SelectedStruct = (*StructOptions)[0];

        if (OnSelectionChangedEvent.IsBound())
        {
            OnSelectionChangedEvent.Execute(SelectedStruct);
        }
    }

    ChildSlot
        [
            SNew(SComboBox<UScriptStruct*>)
                .OptionsSource(StructOptions.Get())
                .InitiallySelectedItem(SelectedStruct)
                .OnGenerateWidget(this, &SStructComboBoxWidget::GenerateComboBoxItem)
                .OnSelectionChanged(this, &SStructComboBoxWidget::OnComboBoxSelectionChanged)
                [
                    SNew(STextBlock)
                        .Text(this, &SStructComboBoxWidget::GetSelectedStructText)
                ]
        ];

    if (SelectedStruct)
    {
        // 선택된 이벤트가 바인딩되어 있는 경우에만 호출
        if (OnSelectionChangedEvent.IsBound())
        {
            OnSelectionChangedEvent.Execute(SelectedStruct);
        }
    }
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

/// <summary>
/// 선택된 struct를 강제로 설정하는 함수.
/// </summary>
/// <param name="NewStruct">새롭게 설정할 UScriptStruct.</param>
void SStructComboBoxWidget::SetSelectedStruct(UScriptStruct* NewStruct)
{
    for (UScriptStruct* Struct : *StructOptions)
    {
        if (Struct == NewStruct)
        {
            SelectedStruct = Struct;
            break;
        }
    }
}

/// <summary>
/// 콤보 박스 항목을 생성하는 함수.
/// </summary>
/// <param name="InStruct">생성할 struct.</param>
/// <returns>생성된 위젯.</returns>
TSharedRef<SWidget> SStructComboBoxWidget::GenerateComboBoxItem(UScriptStruct* InStruct)
{
    return SNew(STextBlock).Text(FText::FromString(InStruct->GetName()));
}

/// <summary>
/// 콤보 박스에서 선택이 변경되었을 때 호출되는 함수.
/// </summary>
/// <param name="NewSelection">새롭게 선택된 struct.</param>
/// <param name="SelectInfo">선택 정보.</param>
void SStructComboBoxWidget::OnComboBoxSelectionChanged(UScriptStruct* NewSelection, ESelectInfo::Type SelectInfo)
{
    if (NewSelection)
    {
        SelectedStruct = NewSelection;

        // 선택된 이벤트가 바인딩되어 있는 경우에만 호출
        if (OnSelectionChangedEvent.IsBound())
        {
            OnSelectionChangedEvent.Execute(NewSelection);
        }
    }
}

/// <summary>
/// 현재 선택된 struct의 이름을 반환하는 함수.
/// </summary>
/// <returns>선택된 struct의 이름을 FText 형식으로 반환.</returns>
FText SStructComboBoxWidget::GetSelectedStructText() const
{
    return SelectedStruct ? FText::FromString(SelectedStruct->GetName()) : FText::GetEmpty();
}

/// <summary>
/// 특정 베이스 struct를 상속받은 모든 struct를 찾아 목록을 구성하는 함수.
/// </summary>
/// <param name="BaseStruct">기준이 되는 struct.</param>
/// <param name="bIncludeBaseStruct">베이스 struct를 목록에 포함할지 여부.</param>
void SStructComboBoxWidget::PopulateStructOptions(UScriptStruct* BaseStruct, bool bIncludeBaseStruct)
{
    for (TObjectIterator<UScriptStruct> It; It; ++It)
    {
        UScriptStruct* Struct = *It;

        // BaseStruct 포함 여부를 옵션으로 처리
        if (Struct == BaseStruct && !bIncludeBaseStruct)
        {
            // BaseStruct 자체를 제외할 경우, 추가하지 않고 continue
            continue;
        }

        // 해당 struct가 BaseStruct의 자식인지 확인
        if (Struct->IsChildOf(BaseStruct))
        {
            StructOptions->Add(Struct);
        }
    }
}
