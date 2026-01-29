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
    const auto trackWidth = static_cast<float>(width) * 0.2f;
    const auto thumbWidth = static_cast<float>(width) * 0.7f;
    const auto thumbHeight = thumbWidth * 1.6f;

    const auto centerX{static_cast<float>(x) + (static_cast<float>(width) / 2.0f)};
    const auto trackX{centerX - (trackWidth / 2.0f)};

    const auto trackColour{slider.findColour(juce::Slider::trackColourId)};
    const auto thumbColour{slider.findColour(juce::Slider::thumbColourId)};
    const auto bgColour{slider.findColour(juce::Slider::backgroundColourId)};

    // Constrain sliderPos to prevent thumb from going outside bounds
    const auto constrainedSliderPos = juce::jlimit(static_cast<float>(y) + thumbHeight / 2.0f,
                                                   static_cast<float>(y + height) - thumbHeight / 2.0f, sliderPos);

    // ============================================================================
    // TRACK DRAWING
    // ============================================================================

    // Draw track background
    juce::Rectangle<float> trackBg(trackX, static_cast<float>(y), trackWidth, static_cast<float>(height));
    g.setColour(bgColour);
    g.fillRoundedRectangle(trackBg, 4.0f);

    // Draw track outline
    g.setColour(trackColour.darker(0.5f));
    g.drawRoundedRectangle(trackBg, 4.0f, 1.0f);

    // Draw filled track (from bottom to current position)
    const auto fillHeight{static_cast<float>(height) - (constrainedSliderPos - static_cast<float>(y))};
    juce::Rectangle<float> trackFill(trackX + 1.0f, constrainedSliderPos, trackWidth - 2.0f, fillHeight);

    // Gradient fill for level indication
    g.setGradientFill(juce::ColourGradient(trackColour.brighter(0.3f), centerX, static_cast<float>(y + height), trackColour,
                                           centerX, static_cast<float>(y), false));
    g.fillRoundedRectangle(trackFill, 3.0f);

    // ============================================================================
    // THUMB FADER DRAWING
    // ============================================================================

    const auto thumbY{constrainedSliderPos - (thumbHeight / 2.0f)};
    const auto thumbX{centerX - (thumbWidth / 2.0f)};

    juce::Rectangle<float> thumbRect(thumbX, thumbY, thumbWidth, thumbHeight);

    // Thumb shadow
    const auto shadow =
        juce::DropShadow(juce::Colours::black, 20, {static_cast<int>(thumbWidth) / 4, static_cast<int>(thumbHeight) / 4});
    juce::Path thumbPath;
    thumbPath.addRectangle(thumbRect);
    shadow.drawForPath(g, thumbPath);

    // Base gradient: top quarter bright, middle gradient, bottom quarter dark
    juce::ColourGradient verticalGradient(thumbColour.darker(3.0f),  // Very dark top
                                          thumbX,                    // X doesn't matter for vertical gradient
                                          thumbY,
                                          thumbColour.brighter(3.0f),  // Very bright bottom
                                          thumbX,                      // X doesn't matter for vertical gradient
                                          thumbY + thumbHeight,
                                          false  // Linear vertical
    );
    g.setGradientFill(verticalGradient);
    g.fillRect(thumbRect);

    // Center indicator band (black bar in the middle)
    g.setColour(juce::Colours::black.withAlpha(0.8f));
    const auto centerBandY = thumbY + thumbHeight * 0.5f;
    g.fillRect(thumbX, centerBandY, thumbWidth, thumbHeight * 0.05f);

    const auto highlightHeight = thumbWidth / 6 * 1.5f;
    // Edge highlights for 3D effect
    // Top edge - bright (angled face catching light)
    g.setColour(thumbColour.brighter(2.0f));
    g.fillRect(thumbX, thumbY, thumbWidth, highlightHeight);

    // Bottom edge - dark (angled face in shadow)
    g.setColour(thumbColour.darker(3.0f));
    g.fillRect(thumbX, thumbY + thumbHeight - highlightHeight, thumbWidth, highlightHeight);

    // Subtle edge/bevel
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.drawRect(thumbRect, 0.5f);
}