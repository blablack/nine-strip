#include "ScaledLookAndFeel.h"

juce::Font ScaledLookAndFeel::getComboBoxFont(juce::ComboBox& box)
{
    return withDefaultMetrics(juce::FontOptions{juce::jmin(16.0f * uiScale, static_cast<float>(box.getHeight()) * 0.85f)});
}

juce::Font ScaledLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return withDefaultMetrics(juce::FontOptions{juce::jmin(16.0f * uiScale, static_cast<float>(buttonHeight) * 0.6f)});
}
