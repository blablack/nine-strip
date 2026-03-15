#include "CircularKnob.h"

bool CircularKnob::hitTest(int mx, int my)
{
    // Get the sliderBounds from the current layout
    auto layout = getLookAndFeel().getSliderLayout(*this);
    auto bounds = layout.sliderBounds.toFloat();

    // Use the same baseSizeRatio your LookAndFeel uses
    const auto minDim = std::min(bounds.getWidth(), bounds.getHeight());
    const auto radius = (minDim * 0.7f) / 2.0f;  // baseSizeRatio for non-small mode
    const auto centre = bounds.getCentre();

    const auto dx = static_cast<float>(mx) - centre.x;
    const auto dy = static_cast<float>(my) - centre.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}