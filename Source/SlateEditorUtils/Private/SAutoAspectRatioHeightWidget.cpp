// Fill out your copyright notice in the Description page of Project Settings.


#include "SAutoAspectRatioHeightWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SAutoAspectRatioHeightWidget::Construct(const FArguments& InArgs)
{
	HeightRatio = InArgs._HeightRatio;
	MaxHeight = InArgs._MaxHeight;

	ChildSlot
		[
			InArgs._Content.Widget
		];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FVector2D SAutoAspectRatioHeightWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FVector2D ParentSize = GetCachedGeometry().GetLocalSize();

	// 세로 크기 제한 적용
	float Height = FMath::Min(ParentSize.Y, MaxHeight);
	float Width = Height * HeightRatio;

	return FVector2D(Width, Height);
}

void SAutoAspectRatioHeightWidget::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const FVector2D ParentSize = AllottedGeometry.GetLocalSize();

	// 세로 크기 제한 적용
	float Height = FMath::Min(ParentSize.Y, MaxHeight);
	float Width = Height * HeightRatio;

	// 가운데 정렬을 위한 오프셋 계산
	FVector2D Offset = (ParentSize - FVector2D(Width, Height)) * 0.5f;

	ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ChildSlot.GetWidget(), Offset, FVector2D(Width, Height)));
}