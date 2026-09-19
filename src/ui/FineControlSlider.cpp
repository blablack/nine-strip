#include "FineControlSlider.h"

FineControlSlider::FineControlSlider()
{
    // We handle the modifier ourselves in mouseDrag, so stop the Slider from swapping to
    // velocity mode. Snapping to the click position is also done here (see mouseDown).
    setVelocityModeParameters(1.0, 1, 0.0, false);
    setSliderSnapsToMousePosition(false);
}

void FineControlSlider::mouseDown(const juce::MouseEvent& e)
{
    // Let the Slider start the drag gesture (SliderAttachment -> beginChangeGesture) and
    // handle popup menus / double-click; we only take over the value updates.
    juce::Slider::mouseDown(e);

    dragging = isEnabled() && e.mods.isLeftButtonDown();
    lastDragPos = e.position;
    dragProportion = valueToProportionOfLength(getValue());

    // A plain click on a fader jumps to the mouse position like a stock JUCE slider does.
    if (dragging && !isRotary() && !e.mods.isCommandDown())
    {
        dragProportion = proportionAtMouse(e);
        setValue(proportionOfLengthToValue(dragProportion), juce::sendNotificationSync);
    }
}

void FineControlSlider::mouseDrag(const juce::MouseEvent& e)
{
    if (!dragging)
    {
        return;
    }

    const auto dx = static_cast<double>(e.position.x - lastDragPos.x);
    const auto dy = static_cast<double>(lastDragPos.y - e.position.y);  // up is positive
    lastDragPos = e.position;

    // Rotary: JUCE's RotaryHorizontalVerticalDrag convention, right and up both increase.
    double delta = dx;
    if (isRotary())
    {
        delta = dx + dy;
    }
    else if (isVertical())
    {
        delta = dy;
    }

    auto pixels = pixelsForFullRange();
    if (e.mods.isCommandDown())
    {
        pixels *= kFineControlFactor;
    }

    // Clamp the tracked position so that reversing direction after hitting an end stop
    // moves the control immediately instead of first "unwinding" the overshoot.
    dragProportion = juce::jlimit(0.0, 1.0, dragProportion + delta / pixels);
    setValue(proportionOfLengthToValue(dragProportion), juce::sendNotificationSync);
}

double FineControlSlider::pixelsForFullRange() const
{
    if (isHorizontal() || isVertical())
    {
        // The track length, so a plain drag keeps the fader thumb under the mouse.
        const auto length = std::abs(getPositionOfValue(getMaximum()) - getPositionOfValue(getMinimum()));
        if (length > 0.0f)
        {
            return length;
        }
    }

    return getMouseDragSensitivity();
}

double FineControlSlider::proportionAtMouse(const juce::MouseEvent& e) const
{
    const auto start = static_cast<double>(getPositionOfValue(getMinimum()));
    const auto end = static_cast<double>(getPositionOfValue(getMaximum()));
    const auto mouse = static_cast<double>(isVertical() ? e.position.y : e.position.x);

    return juce::approximatelyEqual(start, end) ? 0.0 : juce::jlimit(0.0, 1.0, (mouse - start) / (end - start));
}
