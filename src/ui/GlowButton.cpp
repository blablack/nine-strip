#include "GlowButton.h"

GlowButton::GlowButton(const juce::String& text) : juce::ToggleButton(text) {}

void GlowButton::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    bool isOn = getToggleState();

    // Base/housing (dark, recessed look)
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 5.0f);

    // Inner shadow for depth
    g.setGradientFill(juce::ColourGradient(juce::Colours::black.withAlpha(0.6f), bounds.getTopLeft(),
                                           juce::Colours::transparentBlack, bounds.getCentre(), true));
    g.fillRoundedRectangle(bounds.reduced(1.0f), 4.0f);

    if (isOn)
    {
        // Glowing light underneath the cap
        auto glowBounds = bounds.reduced(3.0f);
        g.setGradientFill(juce::ColourGradient(juce::Colour(0xffff6600).withAlpha(0.9f), glowBounds.getCentre(),
                                               juce::Colour(0xffff6600).withAlpha(0.0f), glowBounds.getTopLeft(), true));
        g.fillRoundedRectangle(glowBounds, 3.0f);
    }

    // Plastic cap
    auto capBounds = bounds.reduced(2.0f);
    g.setColour(isOn ? juce::Colour(0xff2a2a2a) : juce::Colour(0xff252525));
    g.fillRoundedRectangle(capBounds, 4.0f);

    // Highlight on top of cap (makes it look glossy/plastic)
    g.setGradientFill(juce::ColourGradient(juce::Colours::white.withAlpha(isOn ? 0.15f : 0.08f), capBounds.getTopLeft(),
                                           juce::Colours::transparentBlack, capBounds.getCentre(), false));
    g.fillRoundedRectangle(capBounds.removeFromTop(capBounds.getHeight() * 0.5f), 4.0f);

    // Subtle edge/bevel
    g.setColour(juce::Colours::white.withAlpha(0.1f));
    g.drawRoundedRectangle(capBounds, 4.0f, 0.5f);

    // Text
    g.setColour(isOn ? juce::Colour(0xffffaa66) : juce::Colours::grey);
    g.setFont(13.0f);
    g.setFont(g.getCurrentFont().boldened());
    g.drawText(getButtonText(), bounds, juce::Justification::centred);
}