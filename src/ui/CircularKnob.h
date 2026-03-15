#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

class CircularKnob : public juce::Slider
{
   public:
    bool hitTest(int mx, int my) override;
};
