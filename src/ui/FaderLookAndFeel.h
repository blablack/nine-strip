#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class FaderLookAndFeel : public juce::LookAndFeel_V4
{
   public:
    FaderLookAndFeel();

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
                          float maxSliderPos, juce::Slider::SliderStyle style, juce::Slider& slider) override;

   private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderLookAndFeel)
};
