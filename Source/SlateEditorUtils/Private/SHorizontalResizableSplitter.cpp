// Fill out your copyright notice in the Description page of Project Settings.


#include "SHorizontalResizableSplitter.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHorizontalResizableSplitter::Construct(const FArguments& InArgs)
{
    Widgets = InArgs._Widgets;
    TArray<float> InitialRatios = InArgs._InitialRatios;

    checkf(Widgets.Num() == InitialRatios.Num(), TEXT("The number of Widgets and InitialRatios must be the same."));

    TSharedPtr<SSplitter> Splitter = SNew(SSplitter).Orientation(Orient_Horizontal);

    // 각 위젯과 초기 비율을 사용하여 슬롯 추가
    for (int32 i = 0; i < Widgets.Num(); ++i)
    {
        Splitter->AddSlot()
            .Value(InitialRatios[i])
            [
                Widgets[i].ToSharedRef()
            ];
    }

    ChildSlot
        [
            Splitter.ToSharedRef()
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
