#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class GlowButton : public juce::ToggleButton
{
   public:
    GlowButton(const juce::String& text);

    void paint(juce::Graphics& g) override;

   private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GlowButton)
};