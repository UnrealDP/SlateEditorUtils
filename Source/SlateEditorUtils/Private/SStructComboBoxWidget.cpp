// Fill out your copyright notice in the Description page of Project Settings.


#include "SStructComboBoxWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
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
            OnSelectionChangedEvent.Execute(SelectedStruct, ESelectInfo::Type::Direct);
        }
    }
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

// 선택된 struct를 강제로 설정하는 함수
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

// 콤보 박스 아이템 생성
TSharedRef<SWidget> SStructComboBoxWidget::GenerateComboBoxItem(UScriptStruct* InStruct)
{
    return SNew(STextBlock).Text(FText::FromString(InStruct->GetName()));
}

// 콤보 박스 선택 변경 이벤트 처리
void SStructComboBoxWidget::OnComboBoxSelectionChanged(UScriptStruct* NewSelection, ESelectInfo::Type SelectInfo)
{
    if (NewSelection)
    {
        SelectedStruct = NewSelection;

        // 선택된 이벤트가 바인딩되어 있는 경우에만 호출
        if (OnSelectionChangedEvent.IsBound())
        {
            OnSelectionChangedEvent.Execute(NewSelection, SelectInfo);
        }
    }
}

// 선택된 struct의 이름을 반환
FText SStructComboBoxWidget::GetSelectedStructText() const
{
    return SelectedStruct ? FText::FromString(SelectedStruct->GetName()) : FText::GetEmpty();
}

// 특정 베이스 struct를 상속받은 struct를 찾아 목록을 구성
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
