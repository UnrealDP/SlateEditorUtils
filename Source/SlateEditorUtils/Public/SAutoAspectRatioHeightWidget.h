// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATEEDITORUTILS_API SAutoAspectRatioHeightWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAutoAspectRatioHeightWidget) : _HeightRatio(1.0f), _MaxHeight(FLT_MAX) {}
		SLATE_ARGUMENT(float, HeightRatio)  // 세로 기준의 가로 비율
		SLATE_ARGUMENT(float, MaxHeight)    // 세로 최대 사이즈
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

private:
	// 세로 기준의 가로 비율
	float HeightRatio;

	// 세로 최대 사이즈
	float MaxHeight;
};
