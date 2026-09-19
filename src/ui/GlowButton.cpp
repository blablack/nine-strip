#include "GlowButton.h"

GlowButton::GlowButton(const juce::String& buttonText) : juce::ToggleButton(buttonText) {}

void GlowButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    const bool on = getToggleState();
    const float s = uiScale;  // all sizes below are in pixels of the 600x600 base design

    // Base/housing (dark, recessed look)
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 5.0f * s);

    // Inner shadow for depth
    g.setGradientFill(juce::ColourGradient(juce::Colours::black.withAlpha(0.6f), bounds.getTopLeft(),
                                           juce::Colours::transparentBlack, bounds.getCentre(), true));
    g.fillRoundedRectangle(bounds.reduced(1.0f * s), 4.0f * s);

    if (on)
    {
        // Glowing light underneath the cap
        auto glowBounds = bounds.reduced(3.0f * s);
        g.setGradientFill(juce::ColourGradient(juce::Colour(0xffff6600).withAlpha(0.9f), glowBounds.getCentre(),
                                               juce::Colour(0xffff6600).withAlpha(0.0f), glowBounds.getTopLeft(), true));
        g.fillRoundedRectangle(glowBounds, 3.0f * s);
    }

    // Plastic cap
    auto capBounds = bounds.reduced(2.0f * s);
    g.setColour(on ? juce::Colour(0xff2a2a2a) : juce::Colour(0xff252525));
    g.fillRoundedRectangle(capBounds, 4.0f * s);

    // Highlight on top of cap (makes it look glossy/plastic)
    g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(on ? 0.15f : 0.08f), capBounds.getTopLeft(),
                                           juce::Colours::transparentBlack, capBounds.getCentre(), false));
    g.fillRoundedRectangle(capBounds.removeFromTop(capBounds.getHeight() * 0.5f), 4.0f * s);

    // Subtle edge/bevel
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.drawRoundedRectangle(capBounds, 4.0f * s, 0.5f * s);

    // Text
    g.setColour(on ? juce::Colour(0xffffaa66) : juce::Colours::grey);
    g.setFont(13.0f * s);
    g.setFont(g.getCurrentFont().boldened());
    g.drawText(getButtonText(), bounds, juce::Justification::centred);
}