#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// LookAndFeel_V4 whose combo-box and text-button fonts follow the editor's scale.
//
// The stock rules cap both fonts at 16 px, which is right at the 600x600 base size but leaves the preset bar
// with tiny text once the window is enlarged. The cap is multiplied by the scale the editor passes in from
// resized(); everything else is inherited unchanged, so at 1x this draws exactly like the default look.
class ScaledLookAndFeel : public juce::LookAndFeel_V4
{
   public:
    void setUiScale(float scale) { uiScale = scale; }

    juce::Font getComboBoxFont(juce::ComboBox& box) override;
    juce::Font getTextButtonFont(juce::TextButton& button, int buttonHeight) override;

   private:
    float uiScale = 1.0f;
};
