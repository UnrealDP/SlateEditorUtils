// Fill out your copyright notice in the Description page of Project Settings.


#include "SAutoAspectRatioWidthWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAutoAspectRatioWidthWidget::Construct(const FArguments& InArgs)
{
	WidthRatio = InArgs._WidthRatio;
	MaxWidth = InArgs._MaxWidth;

	ChildSlot
		[
			InArgs._Content.Widget
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FVector2D SAutoAspectRatioWidthWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FVector2D ParentSize = GetCachedGeometry().GetLocalSize();

	// 가로 크기 제한 적용
	float Width = FMath::Min(ParentSize.X, MaxWidth);
	float Height = Width * WidthRatio;

	return FVector2D(Width, Height);
}

void SAutoAspectRatioWidthWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const FVector2D ParentSize = AllottedGeometry.GetLocalSize();

	// 가로 크기 제한 적용
	float Width = FMath::Min(ParentSize.X, MaxWidth);
	float Height = Width * WidthRatio;

	// 가운데 정렬을 위한 오프셋 계산
	FVector2D Offset = (ParentSize - FVector2D(Width, Height)) * 0.5f;

	ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ChildSlot.GetWidget(), Offset, FVector2D(Width, Height)));
}