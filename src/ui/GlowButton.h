#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class GlowButton : public juce::ToggleButton
{
   public:
    GlowButton(const juce::String& buttonText);

    // Multiplies the base-design pixel sizes (font, corner radii, bevel); the editor sets it from resized().
    void setUiScale(float scale) { uiScale = scale; }

    void paint(juce::Graphics& g) override;

   private:
    float uiScale = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlowButton)
};