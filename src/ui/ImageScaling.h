#pragma once

#include <juce_graphics/juce_graphics.h>

// JUCE's "high" quality resample is plain bilinear: it only ever touches a 2x2 source neighbourhood, so a large
// one-shot downscale (the 750 px meter face at ~155 px, the 1800 px background at 600 px) drops most of the source
// pixels, and 1 px lines and small text alias into broken, shimmering strokes. Each 2:1 bilinear step is an exact
// 2x2 box filter, so halve until within 2x of the target, then resample the rest of the way.
inline juce::Image downscaleSmoothly(juce::Image img, int w, int h)
{
    while (img.getWidth() >= 2 * w && img.getHeight() >= 2 * h)
        img = img.rescaled(img.getWidth() / 2, img.getHeight() / 2, juce::Graphics::highResamplingQuality);
    return img.rescaled(w, h, juce::Graphics::highResamplingQuality);
}
