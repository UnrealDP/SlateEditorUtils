#include "SlateEditorUtils.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"

/**
 * 텍스트 레이블이 있는 체크박스를 생성하는 함수.
 * 체크박스와 텍스트가 나란히 배치된 UI 요소를 반환합니다.
 *
 * @param LabelText 체크박스 옆에 표시할 텍스트.
 * @param OnChangedCallback 체크박스 상태 변경 시 호출할 콜백 함수.
 * @return 체크박스와 텍스트가 포함된 SHorizontalBox 위젯을 반환.
 */
TSharedRef<SWidget> SlateEditorUtils::CreateCheckboxWithText(const FText& LabelText, FOnCheckStateChanged OnChangedCallback)
{
    return
        SNew(SHorizontalBox)

        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(SCheckBox)
                .OnCheckStateChanged(OnChangedCallback)
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(STextBlock)
                .Text(LabelText)
        ];
}

/**
 * 텍스트가 포함된 버튼을 생성하는 함수.
 * 버튼 클릭 시 호출할 콜백 함수와 함께 텍스트를 지정할 수 있습니다.
 *
 * @param ButtonText 버튼에 표시할 텍스트.
 * @param OnClickedCallback 버튼 클릭 시 호출할 콜백 함수.
 * @return 텍스트와 콜백이 포함된 버튼 위젯을 반환.
 */
TSharedRef<SWidget> SlateEditorUtils::CreateButtonWithText(const FText& ButtonText, FOnClicked OnClickedCallback)
{
    return
        SNew(SButton)
        .Text(ButtonText)
        .OnClicked(OnClickedCallback);
}
