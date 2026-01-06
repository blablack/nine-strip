#include "VUMeter.h"

VUMeter::VUMeter(std::function<float()> levelGetter) : getLevelFunc(levelGetter) 
{ 
    startTimerHz(30); 
}

void VUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour(juce::Colours::darkgrey.darker());
    g.fillRect(bounds);

    // Calculate level bar height
    auto levelHeight = juce::jmap(currentLevel, -60.0f, 0.0f, 0.0f, bounds.getHeight());

    // Draw level bar WITHOUT modifying bounds
    auto levelBounds =
        juce::Rectangle<float>(bounds.getX(), bounds.getBottom() - levelHeight, bounds.getWidth(), levelHeight);

    // Color gradient based on level
    if (currentLevel > -3.0f)
        g.setColour(juce::Colours::red);
    else if (currentLevel > -12.0f)
        g.setColour(juce::Colours::yellow);
    else
        g.setColour(juce::Colours::green);

    g.fillRect(levelBounds);

    // Border
    g.setColour(juce::Colours::black);
    g.drawRect(bounds, 1.0f);

    // Draw dB markers (now using original bounds)
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);
    auto drawMarker = [&](float db, const juce::String& label)
    {
        float y = juce::jmap(db, -60.0f, 0.0f, bounds.getBottom(), bounds.getY());
        g.drawLine(bounds.getX(), y, bounds.getX() + 5, y, 1.0f);
        g.drawText(label, bounds.getX() + 6, y - 5, 20, 10, juce::Justification::centredLeft);
    };
    drawMarker(0.0f, "0");
    drawMarker(-12.0f, "-12");
    drawMarker(-24.0f, "-24");
}

void VUMeter::timerCallback()
{
    float newLevel = getLevelFunc();
    newLevel = juce::jlimit(-60.0f, 6.0f, newLevel);

    if (newLevel > currentLevel)
        currentLevel = newLevel;
    else
        currentLevel -= 0.5f;  // Fixed decay for dB values

    currentLevel = std::max(currentLevel, -60.0f);
    repaint();
}
