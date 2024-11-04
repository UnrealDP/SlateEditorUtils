// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATEEDITORUTILS_API SAutoAspectRatioWidthWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAutoAspectRatioWidthWidget) : _WidthRatio(1.0f), _MaxWidth(FLT_MAX) {}
		SLATE_ARGUMENT(float, WidthRatio)  // 가로 기준의 세로 비율
		SLATE_ARGUMENT(float, MaxWidth)    // 가로 최대 사이즈
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

private:
	// 가로 기준의 세로 비율
	float WidthRatio = 1.0f;

	// 가로 최대 사이즈
	float MaxWidth = FLT_MAX;
};
