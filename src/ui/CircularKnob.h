#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "FineControlSlider.h"

class CircularKnob : public FineControlSlider
{
   public:
    bool hitTest(int mx, int my) override;
};
