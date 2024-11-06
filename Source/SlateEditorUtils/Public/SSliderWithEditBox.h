// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SSlider;

DECLARE_DELEGATE_OneParam(FOnSliderValueChanged, float);

/**
 * 
 */
class SLATEEDITORUTILS_API SSliderWithEditBox : public SCompoundWidget
{
public:
	enum class EEditBoxPosition : uint8
	{
		Left,
		Right
	};

	SLATE_BEGIN_ARGS(SSliderWithEditBox) {}
		SLATE_ARGUMENT(float, MinValue)
		SLATE_ARGUMENT(float, MaxValue)
		SLATE_ARGUMENT(float, InitialValue)
		SLATE_ARGUMENT(EEditBoxPosition, EditBoxPosition)
		SLATE_EVENT(FOnSliderValueChanged, OnSliderValueChanged)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	void OnInternalSliderValueChanged(float NewValue);
	void OnEditBoxCommitted(const FText& NewText, ETextCommit::Type CommitType);

private:
	float MinValue = 0.0f;
	float MaxValue = 1.0f;

	TSharedPtr<SSlider> SliderPtr = nullptr;
	TSharedPtr<SEditableTextBox> EditBoxPtr = nullptr;
	FOnSliderValueChanged SliderValueChangedDelegate = nullptr;
};
