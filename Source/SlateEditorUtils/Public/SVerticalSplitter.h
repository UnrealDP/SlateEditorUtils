// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLATEEDITORUTILS_API SVerticalSplitter : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SVerticalSplitter) {}
		SLATE_ARGUMENT(TArray<TSharedPtr<SWidget>>, Widgets) // n개의 위젯
		SLATE_ARGUMENT(TArray<float>, SlotSizes) // 각 위젯의 비율
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> CreateWidgetSlots();

private:
	TArray<TSharedPtr<SWidget>> Widgets;
	TArray<float> SlotSizes;
};
