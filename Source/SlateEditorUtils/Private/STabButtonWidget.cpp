// Fill out your copyright notice in the Description page of Project Settings.


//#include "STabButtonWidget.h"
//#include "SlateOptMacros.h"
//
//BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
//void STabButtonWidget::Construct(const FArguments& InArgs)
//{
//    TabNames = InArgs._TabNames;
//    OnTabChanged = InArgs._OnTabChanged;
//
//    // 기본적으로 첫 번째 탭 선택
//    SelectedTabIndex = 0;
//
//    ChildSlot
//        [
//            SNew(SVerticalBox)
//
//                + SVerticalBox::Slot()
//                .AutoHeight()
//                [
//                    CreateTabButtons()  // 동적으로 탭 버튼 생성
//                ]
//
//                + SVerticalBox::Slot()
//                .AutoHeight()
//                [
//                    SNew(STextBlock)
//                        .Text_Lambda([this]() -> FText
//                            {
//                                if (SelectedTabIndex >= 0 && SelectedTabIndex < TabNames.Num())
//                                {
//                                    return FText::FromString("Selected Tab: " + TabNames[SelectedTabIndex]);
//                                }
//                                return FText::FromString("No Tab Selected");
//                            })
//                ]
//        ];
//}
//END_SLATE_FUNCTION_BUILD_OPTIMIZATION
//
///// <summary>
///// 동적으로 탭 버튼을 생성하는 함수.
///// </summary>
///// <returns>생성된 탭 버튼들을 포함하는 HorizontalBox 위젯.</returns>
//TSharedRef<SWidget> STabButtonWidget::CreateTabButtons()
//{
//    TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);
//
//    for (int32 TabIndex = 0; TabIndex < TabNames.Num(); ++TabIndex)
//    {
//        HorizontalBox->AddSlot()
//            .AutoWidth()
//            .Padding(5)
//            [
//                SNew(SButton)
//                    .Text(FText::FromString(TabNames[TabIndex]))
//                    .OnClicked_Lambda([this, TabIndex]() -> FReply
//                        {
//                            this->OnTabSelected(TabIndex);
//                            return FReply::Handled();
//                        })
//                    // 탭 버튼의 스타일을 설정
//                    .ButtonStyle(FAppStyle::Get(), "Button")
//                    .ForegroundColor(FLinearColor::White) // 글자 색상을 흰색으로 설정
//            ];
//    }
//
//    return HorizontalBox;
//}
//
///// <summary>
///// 특정 탭이 선택되었을 때 호출되는 함수. 선택된 탭 인덱스를 설정하고 탭 변경 이벤트를 호출.
///// </summary>
///// <param name="TabIndex">선택된 탭의 인덱스.</param>
//void STabButtonWidget::OnTabSelected(int32 TabIndex)
//{
//    SelectedTabIndex = TabIndex;
//    OnTabChanged.ExecuteIfBound(TabIndex);  // 탭 변경 이벤트 호출
//}

#include "STabButtonWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void STabButtonWidget::Construct(const FArguments& InArgs)
{
    TabNames = InArgs._TabNames;
    OnTabChanged = InArgs._OnTabChanged;

    // 기본적으로 첫 번째 탭 선택
    SelectedTabIndex = 0;

    ChildSlot
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    CreateTabButtons()  // 동적으로 탭 버튼 생성
                ]
        ];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> STabButtonWidget::CreateTabButtons()
{
    TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);

    for (int32 TabIndex = 0; TabIndex < TabNames.Num(); ++TabIndex)
    {
        HorizontalBox->AddSlot()
            .AutoWidth()
            [
                SNew(SButton)
                    .Text(FText::FromString(TabNames[TabIndex]))
                    .OnClicked_Lambda([this, TabIndex]() -> FReply
                        {
                            this->OnTabSelected(TabIndex);
                            return FReply::Handled();
                        })
                    // 탭 스타일 적용: 선택된 탭은 활성화된 탭 스타일로 보이도록 설정
                    .ButtonStyle(FAppStyle::Get(), "Button")
                    .ForegroundColor(SelectedTabIndex == TabIndex ? FLinearColor::Yellow : FLinearColor::White)  // 선택된 탭은 노란색으로 표시
            ];
    }

    return HorizontalBox;
}

void STabButtonWidget::OnTabSelected(int32 TabIndex)
{
    SelectedTabIndex = TabIndex;
    OnTabChanged.ExecuteIfBound(TabIndex);  // 탭 변경 이벤트 호출

    // 탭 버튼을 새로고침
    if (ChildSlot.GetWidget().ToSharedPtr())
    {
        ChildSlot
            [
                SNew(SVerticalBox)

                    + SVerticalBox::Slot()
                    .AutoHeight()
                    [
                        CreateTabButtons()  // 선택된 상태 반영
                    ]
            ];
    }
}
