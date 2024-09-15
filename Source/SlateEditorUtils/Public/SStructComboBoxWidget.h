// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"

/**
 * 
 */

//DECLARE_DELEGATE_OneParam(FOnStructComboBoxSelectionChanged, UScriptStruct*);

class SLATEEDITORUTILS_API SStructComboBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStructComboBoxWidget)	{}
		SLATE_ARGUMENT(UScriptStruct*, BaseStruct) // 사용자가 제공하는 베이스 struct
		SLATE_ARGUMENT(bool, bIncludeBaseStruct)   // BaseStruct 포함 여부를 제어하는 옵션		
		SLATE_EVENT(SComboBox<UScriptStruct*>::FOnSelectionChanged, OnSelectionChanged)
		//SLATE_EVENT(FOnStructComboBoxSelectionChanged, OnSelectionChanged)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// 현재 선택된 struct를 반환하는 함수
	inline UScriptStruct* GetSelectedStruct() const
	{
		return SelectedStruct;
	}

	// 선택된 struct를 강제로 설정하는 함수
	void SetSelectedStruct(UScriptStruct* NewStruct);

private:
	TSharedPtr<TArray<UScriptStruct*>> StructOptions;
	UScriptStruct* SelectedStruct;
	SComboBox<UScriptStruct*>::FOnSelectionChanged OnSelectionChangedEvent;
	//FOnStructComboBoxSelectionChanged OnSelectionChangedEvent;

	// 콤보 박스 아이템 생성
	TSharedRef<SWidget> GenerateComboBoxItem(UScriptStruct* InStruct);

	// 콤보 박스 선택 변경 이벤트 처리
	void OnComboBoxSelectionChanged(UScriptStruct* NewSelection, ESelectInfo::Type SelectInfo);

	// 선택된 struct의 이름을 반환
	FText GetSelectedStructText() const;

	// 특정 베이스 struct를 상속받은 struct를 찾아 목록을 구성
	void PopulateStructOptions(UScriptStruct*  BaseStruct, bool bIncludeBaseStruct);
};
