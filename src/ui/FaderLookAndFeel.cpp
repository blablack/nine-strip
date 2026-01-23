#include "FaderLookAndFeel.h"

FaderLookAndFeel::FaderLookAndFeel()
{
    setColour(juce::Slider::trackColourId, juce::Colours::darkslategrey.darker());
    setColour(juce::Slider::thumbColourId, juce::Colours::lightgrey);
    setColour(juce::Slider::backgroundColourId, juce::Colours::black.brighter(0.1f));
}

void FaderLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                        float minSliderPos, float maxSliderPos, juce::Slider::SliderStyle style,
                                        juce::Slider& slider)
{
    if (style != juce::Slider::LinearVertical) return;

    // Scale track and thumb based on width
    const auto trackWidth = static_cast<float>(width) * 0.3f;
    const auto thumbWidth = static_cast<float>(width) * 0.6f;
    const auto thumbHeight = thumbWidth * 1.5f;

    const auto centerX{static_cast<float>(x) + (static_cast<float>(width) / 2.0f)};
    const auto trackX{centerX - (trackWidth / 2.0f)};

    const auto trackColour{slider.findColour(juce::Slider::trackColourId)};
    const auto thumbColour{slider.findColour(juce::Slider::thumbColourId)};
    const auto bgColour{slider.findColour(juce::Slider::backgroundColourId)};

    // Draw track background
    juce::Rectangle<float> trackBg(trackX, static_cast<float>(y), trackWidth, static_cast<float>(height));
    g.setColour(bgColour);
    g.fillRoundedRectangle(trackBg, 4.0f);

    // Draw track outline
    g.setColour(trackColour.darker(0.5f));
    g.drawRoundedRectangle(trackBg, 4.0f, 1.0f);

    // Constrain sliderPos to prevent thumb from going outside bounds
    const auto constrainedSliderPos = juce::jlimit(static_cast<float>(y) + thumbHeight / 2.0f,
                                                   static_cast<float>(y + height) - thumbHeight / 2.0f, sliderPos);

    // Draw filled track (from bottom to current position)
    const auto fillHeight{static_cast<float>(height) - (constrainedSliderPos - static_cast<float>(y))};
    juce::Rectangle<float> trackFill(trackX + 1.0f, constrainedSliderPos, trackWidth - 2.0f, fillHeight);

    // Gradient fill for level indication
    g.setGradientFill(juce::ColourGradient(trackColour.brighter(0.3f), centerX, static_cast<float>(y + height), trackColour,
                                           centerX, static_cast<float>(y), false));
    g.fillRoundedRectangle(trackFill, 3.0f);

    // Draw fader thumb
    const auto thumbY{constrainedSliderPos - (thumbHeight / 2.0f)};
    const auto thumbX{centerX - (thumbWidth / 2.0f)};

    juce::Rectangle<float> thumbRect(thumbX, thumbY, thumbWidth, thumbHeight);

    // Thumb shadow
    const auto shadow =
        juce::DropShadow(juce::Colours::black, 20, {static_cast<int>(thumbWidth) / 4, static_cast<int>(thumbHeight) / 4});
    juce::Path thumbPath;
    thumbPath.addRectangle(thumbRect);
    shadow.drawForPath(g, thumbPath);

    juce::ColourGradient radialGradient(
        thumbColour.brighter(0.5f),   // Highlight center (simulates light hitting curved surface)
        thumbX + thumbWidth / 2.0f,   // Center X
        thumbY + thumbHeight * 0.3f,  // Light source from upper portion
        thumbColour.darker(0.3f),     // Darker edges
        thumbX + thumbWidth / 2.0f,   // Center X
        thumbY + thumbHeight,         // Extends to bottom
        true                          // ← RADIAL (was false for linear)
    );
    g.setGradientFill(radialGradient);
    g.fillRect(thumbRect);

    // Draw horizontal ridges/grooves
    constexpr int numRidges = 5;
    const auto ridgeHeight = thumbHeight / static_cast<float>(numRidges);

    for (int i = 0; i < numRidges; ++i)
    {
        const auto ridgeY = thumbY + (ridgeHeight * static_cast<float>(i));
        const auto verticalPos = static_cast<float>(i) / static_cast<float>(numRidges - 1);

        auto ridgeBaseColour = thumbColour.darker(0.4f).interpolatedWith(thumbColour.brighter(0.3f), verticalPos);

        juce::ColourGradient grooveGradient(ridgeBaseColour.darker(0.6f),  // Was 0.4f - darker shadow
                                            thumbX, ridgeY,
                                            ridgeBaseColour.brighter(0.6f),  // Was 0.4f - brighter highlight
                                            thumbX, ridgeY + ridgeHeight, false);

        g.setGradientFill(grooveGradient);
        g.fillRect(juce::Rectangle<float>(thumbX, ridgeY, thumbWidth, ridgeHeight));

        // Groove separator line
        if (i < numRidges - 1)
        {
            auto separatorBrightness = verticalPos;
            g.setColour(thumbColour.darker(0.7f).interpolatedWith(thumbColour.darker(0.3f), separatorBrightness));
            g.drawLine(thumbX + 1.0f, ridgeY + ridgeHeight, thumbX + thumbWidth - 1.0f, ridgeY + ridgeHeight, 1.0f);
        }
    }

    // Add subtle noise texture for realism
    juce::Random random(static_cast<int64_t>(thumbY * 1000));
    const int noisePoints = 300;
    for (int i = 0; i < noisePoints; ++i)
    {
        const auto noiseX = thumbX + random.nextFloat() * thumbWidth;
        const auto noiseY = thumbY + random.nextFloat() * thumbHeight;
        const auto noiseBrightness = random.nextFloat() * 0.25f - 0.125f;

        g.setColour(thumbColour.brighter(noiseBrightness).withAlpha(0.15f));
        g.fillRect(noiseX, noiseY, 1.0f, 1.0f);
    }

    auto highlightArea = thumbRect.removeFromTop(thumbHeight * 0.35f);
    g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(0.25f),  // Bright white at top
                                           highlightArea.getTopLeft(),
                                           juce::Colours::transparentBlack,  // Fades to transparent
                                           highlightArea.getBottomLeft(), false));
    g.fillRect(highlightArea);

    // Top edge - bright (catching light)
    g.setColour(thumbColour.brighter(0.6f));
    g.fillRect(thumbX, thumbY, thumbWidth, 1.5f);

    // Left edge - bright (catching light)
    g.setColour(thumbColour.brighter(0.5f));
    g.fillRect(thumbX, thumbY, 1.5f, thumbHeight);

    // Bottom edge - dark (in shadow)
    g.setColour(thumbColour.darker(0.5f));
    g.fillRect(thumbX, thumbY + thumbHeight - 1.5f, thumbWidth, 1.5f);

    // Right edge - dark (in shadow)
    g.setColour(thumbColour.darker(0.6f));
    g.fillRect(thumbX + thumbWidth - 1.5f, thumbY, 1.5f, thumbHeight);

    g.setColour(juce::Colours::white.withAlpha(0.15f));
    g.drawRect(thumbRect, 0.5f);

    // Center indicator band - smaller and thinner
    g.setColour(juce::Colours::black.withAlpha(0.6f));
    const auto centerBandY = thumbY + thumbHeight * 0.45f;
    g.fillRect(thumbX + 4.0f, centerBandY, thumbWidth - 8.0f, thumbHeight * 0.1f);

    // Outer outline
    g.setColour(thumbColour.darker(0.7f));
    g.drawRect(thumbRect, 1.0f);
}
