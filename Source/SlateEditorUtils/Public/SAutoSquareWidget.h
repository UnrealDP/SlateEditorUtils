// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATEEDITORUTILS_API SAutoSquareWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SAutoSquareWidget) : _SquareAxis(EAxis::X) {}
		SLATE_ARGUMENT(EAxis::Type, SquareAxis)
		SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

private:
	EAxis::Type SquareAxis = EAxis::X;
};
