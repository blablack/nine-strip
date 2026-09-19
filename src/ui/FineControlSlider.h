#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// juce::Slider with a fine-control modifier and no mouse-pointer hiding.
//
// A stock Slider switches to "velocity mode" while Ctrl/Alt/Cmd is held, which hides the
// mouse pointer and moves the control by mouse *speed*. This class handles the drag itself:
// movement is distance-based, and holding Ctrl (Cmd on macOS) divides the speed by
// kFineControlFactor. The modifier can be pressed or released mid-drag without the control
// jumping, because each event only applies the movement since the previous one.
//
// Rotary styles follow JUCE's RotaryHorizontalVerticalDrag convention (right and up increase)
// over getMouseDragSensitivity() pixels for the full range. Linear styles jump to the click
// position on a plain click, like a stock fader, and then follow the mouse 1:1 along the
// track; a Ctrl-click starts a fine adjustment from the current value instead of jumping.
class FineControlSlider : public juce::Slider
{
   public:
    FineControlSlider();

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;

   private:
    static constexpr double kFineControlFactor = 10.0;

    double pixelsForFullRange() const;
    double proportionAtMouse(const juce::MouseEvent& e) const;

    bool dragging = false;
    juce::Point<float> lastDragPos;
    double dragProportion = 0.0;  // 0–1 position of the control, tracked across the drag
};
