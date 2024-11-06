// Fill out your copyright notice in the Description page of Project Settings.


#include "SSliderWithEditBox.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SSlider.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSliderWithEditBox::Construct(const FArguments& InArgs)
{
    MinValue = InArgs._MinValue;
    MaxValue = InArgs._MaxValue;
    float InitialValue = InArgs._InitialValue;
    EEditBoxPosition EditBoxPosition = InArgs._EditBoxPosition;
    SliderValueChangedDelegate = InArgs._OnSliderValueChanged;

    TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

    // EditBox의 위치를 enum을 통해 설정
    if (EditBoxPosition == EEditBoxPosition::Left)
    {
        HorizontalBox->AddSlot()
            .AutoWidth()
            .Padding(2)
            [
                SAssignNew(EditBoxPtr, SEditableTextBox)
                    .Text(FText::AsNumber(InitialValue))
                    .OnTextCommitted(this, &SSliderWithEditBox::OnEditBoxCommitted)
            ];

        HorizontalBox->AddSlot()
            .FillWidth(1.0f)
            .Padding(2)
            [
                SAssignNew(SliderPtr, SSlider)
                    .MinValue(MinValue)
                    .MaxValue(MaxValue)
                    .Value(InitialValue)
                    .StepSize(0.1f)
                    .OnValueChanged(this, &SSliderWithEditBox::OnInternalSliderValueChanged)
            ];
    }
    else // EEditBoxPosition::Right
    {
        HorizontalBox->AddSlot()
            .FillWidth(1.0f)
            .Padding(2)
            [
                SAssignNew(SliderPtr, SSlider)
                    .MinValue(MinValue)
                    .MaxValue(MaxValue)
                    .Value(InitialValue)
                    .OnValueChanged(this, &SSliderWithEditBox::OnInternalSliderValueChanged)
            ];

        HorizontalBox->AddSlot()
            .AutoWidth()
            .Padding(2)
            [
                SAssignNew(EditBoxPtr, SEditableTextBox)
                    .Text(FText::AsNumber(InitialValue))
                    .OnTextCommitted(this, &SSliderWithEditBox::OnEditBoxCommitted)
            ];
    }

    ChildSlot
        [
            HorizontalBox
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSliderWithEditBox::OnInternalSliderValueChanged(float NewValue)
{
    FString NewText = FString::SanitizeFloat(FMath::RoundToFloat(NewValue * 100) / 100);
    EditBoxPtr->SetText(FText::FromString(NewText));

    // 외부 델리게이트 호출
    if (SliderValueChangedDelegate.IsBound())
    {
        SliderValueChangedDelegate.Execute(NewValue);
    }
}

void SSliderWithEditBox::OnEditBoxCommitted(const FText& NewText, ETextCommit::Type CommitType)
{
    float NewValue = FCString::Atof(*NewText.ToString());
    if (NewValue >= MinValue && NewValue <= MaxValue)
    {
        SliderPtr->SetValue(NewValue);
        // 슬라이더 값을 변경할 때 델리게이트 호출
        if (SliderValueChangedDelegate.IsBound())
        {
            SliderValueChangedDelegate.Execute(NewValue);
        }
    }
}
