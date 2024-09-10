#pragma once

#include "CoreMinimal.h"


class SLATEEDITORUTILS_API SlateEditorUtils
{
public:
    // Declares a function to create a checkbox with text
    static TSharedRef<SWidget> CreateCheckboxWithText(const FText& LabelText, FOnCheckStateChanged OnChangedCallback);

    // Declares a function to create a button with text
    static TSharedRef<SWidget> CreateButtonWithText(const FText& ButtonText, FOnClicked OnClickedCallback);
};
