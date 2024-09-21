// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"

/**
 * Struct를 선택할 수 있는 콤보 박스 위젯 클래스.
 */
DECLARE_DELEGATE_OneParam(FOnStructComboBoxSelectionChanged, UScriptStruct*);

class SLATEEDITORUTILS_API SStructComboBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStructComboBoxWidget)	{}
		SLATE_ARGUMENT(UScriptStruct*, BaseStruct) // 사용자가 제공하는 베이스 struct
		SLATE_ARGUMENT(bool, bIncludeBaseStruct)   // BaseStruct 포함 여부를 제어하는 옵션		
		SLATE_EVENT(FOnStructComboBoxSelectionChanged, OnSelectionChanged)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/**
	 * 현재 선택된 struct를 반환하는 함수.
	 * @return 선택된 UScriptStruct 포인터를 반환.
	 */
	inline UScriptStruct* GetSelectedStruct() const
	{
		return SelectedStruct;
	}

	/**
	 * 선택된 struct를 강제로 설정하는 함수.
	 * @param NewStruct 새롭게 설정할 UScriptStruct 포인터.
	 */
	void SetSelectedStruct(UScriptStruct* NewStruct);

private:
	// 콤보 박스에 표시될 Struct 목록
	TSharedPtr<TArray<UScriptStruct*>> StructOptions;

	// 현재 선택된 Struct
	UScriptStruct* SelectedStruct;

	// Struct 선택 변경 이벤트 델리게이트
	FOnStructComboBoxSelectionChanged OnSelectionChangedEvent;

	/**
	 * 콤보 박스 항목을 생성하는 함수.
	 * @param InStruct 생성할 UScriptStruct.
	 * @return 생성된 위젯을 반환.
	 */
	TSharedRef<SWidget> GenerateComboBoxItem(UScriptStruct* InStruct);

	/**
	 * 콤보 박스에서 선택이 변경되었을 때 호출되는 함수.
	 * @param NewSelection 새롭게 선택된 UScriptStruct.
	 * @param SelectInfo 선택 변경 정보.
	 */
	void OnComboBoxSelectionChanged(UScriptStruct* NewSelection, ESelectInfo::Type SelectInfo);

	/**
	 * 선택된 struct의 이름을 반환하는 함수.
	 * @return 선택된 struct의 이름을 FText로 반환.
	 */
	FText GetSelectedStructText() const;

	/**
	 * 특정 베이스 struct를 상속받은 struct를 찾아 목록을 구성하는 함수.
	 * @param BaseStruct 기준이 되는 베이스 UScriptStruct.
	 * @param bIncludeBaseStruct 베이스 struct를 목록에 포함할지 여부.
	 */
	void PopulateStructOptions(UScriptStruct*  BaseStruct, bool bIncludeBaseStruct);
};
