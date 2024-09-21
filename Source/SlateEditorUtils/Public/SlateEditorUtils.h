#pragma once

#include "CoreMinimal.h"


class SLATEEDITORUTILS_API SlateEditorUtils
{
public:
    /**
    * 텍스트 레이블이 있는 체크박스를 생성하는 함수.
    * @param LabelText 체크박스 옆에 표시할 텍스트.
    * @param OnChangedCallback 체크박스 상태 변경 시 호출할 콜백 함수.
    * @return 체크박스와 텍스트가 포함된 SHorizontalBox 위젯을 반환.
    */
    static TSharedRef<SWidget> CreateCheckboxWithText(const FText& LabelText, FOnCheckStateChanged OnChangedCallback);

    /**
    * 텍스트가 포함된 버튼을 생성하는 함수.
    * @param ButtonText 버튼에 표시할 텍스트.
    * @param OnClickedCallback 버튼 클릭 시 호출할 콜백 함수.
    * @return 텍스트와 콜백이 포함된 버튼 위젯을 반환.
    */
    static TSharedRef<SWidget> CreateButtonWithText(const FText& ButtonText, FOnClicked OnClickedCallback);
};
