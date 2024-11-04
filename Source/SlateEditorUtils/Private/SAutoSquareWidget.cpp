// Fill out your copyright notice in the Description page of Project Settings.


#include "SAutoSquareWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAutoSquareWidget::Construct(const FArguments& InArgs)
{
	SquareAxis = InArgs._SquareAxis;
	
	ChildSlot
		[
			InArgs._Content.Widget
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FVector2D SAutoSquareWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
    // Retrieve the current size of the widget's parent container
    const FVector2D Size = GetCachedGeometry().GetLocalSize();

    // Determine square size based on the selected axis
    if (SquareAxis == EAxis::X)
    {
        // Use width as the basis for the square
        return FVector2D(Size.X, Size.X);
    }
    else
    {
        // Use height as the basis for the square
        return FVector2D(Size.Y, Size.Y);
    }
}

void SAutoSquareWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
    // 부모 위젯의 현재 크기
    const FVector2D ParentSize = AllottedGeometry.GetLocalSize();

    // SquareAxis에 따라 정사각형 크기 계산
    FVector2D NewDesiredSize;
    if (SquareAxis == EAxis::X)
    {
        NewDesiredSize = FVector2D(ParentSize.X, ParentSize.X);
    }
    else
    {
        NewDesiredSize = FVector2D(ParentSize.Y, ParentSize.Y);
    }

    ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ChildSlot.GetWidget(), FVector2D(0, 0), NewDesiredSize));
}