#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
   public:
    KnobLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider& slider) override;
};
