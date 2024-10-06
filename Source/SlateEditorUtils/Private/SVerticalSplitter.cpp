// Fill out your copyright notice in the Description page of Project Settings.


#include "SVerticalSplitter.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SVerticalSplitter::Construct(const FArguments& InArgs)
{
    Widgets = InArgs._Widgets;
    SlotSizes = InArgs._SlotSizes;

    // Widgets와 SlotSizes의 수량이 같은지 체크
    checkf(Widgets.Num() == SlotSizes.Num(), TEXT("Widgets와 SlotSizes의 개수가 일치하지 않습니다!"));

    ChildSlot
        [
            SNew(SSplitter)
                .Orientation(EOrientation::Orient_Vertical)
                + SSplitter::Slot()
                [
                    SNew(SVerticalBox)
                        + SVerticalBox::Slot()
                        [
                            CreateWidgetSlots()
                        ]
                ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SVerticalSplitter::CreateWidgetSlots()
{
    TSharedPtr<SSplitter> Splitter = SNew(SSplitter).Orientation(Orient_Vertical);

    for (int32 Index = 0; Index < Widgets.Num(); ++Index)
    {
        Splitter->AddSlot()
            .Value(SlotSizes[Index]) // 각 위젯에 해당하는 비율 적용
            [
                Widgets[Index].ToSharedRef()
            ];
    }

    return Splitter.ToSharedRef();
}