#include "VUMeter.h"

VUMeter::VUMeter(std::function<float()> levelGetter, MeterType type) : getLevelFunc(levelGetter), meterType(type)
{
    startTimerHz(30);
    if (meterType == MeterType::GainReduction) currentLevel = 0.0f;
}

VUMeter::~VUMeter() { stopTimer(); }

void VUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour(juce::Colours::darkgrey.darker());
    g.fillRect(bounds);

    if (meterType == MeterType::GainReduction)
    {
        // Gain reduction meter (0 to -20 dB)
        auto levelHeight = juce::jmap(currentLevel, -20.0f, 0.0f, bounds.getHeight(), 0.0f);
        auto levelBounds = juce::Rectangle<float>(bounds.getX(), bounds.getY(), bounds.getWidth(), levelHeight);

        // Color based on reduction amount
        if (currentLevel < -12.0f)
            g.setColour(juce::Colours::red);
        else if (currentLevel < -6.0f)
            g.setColour(juce::Colours::orange);
        else
            g.setColour(juce::Colours::green);

        g.fillRect(levelBounds);
    }
    else
    {
        // Standard level meter
        auto levelHeight = juce::jmap(currentLevel, -60.0f, 0.0f, 0.0f, bounds.getHeight());
        auto levelBounds =
            juce::Rectangle<float>(bounds.getX(), bounds.getBottom() - levelHeight, bounds.getWidth(), levelHeight);

        if (currentLevel > -3.0f)
            g.setColour(juce::Colours::red);
        else if (currentLevel > -12.0f)
            g.setColour(juce::Colours::yellow);
        else
            g.setColour(juce::Colours::green);

        g.fillRect(levelBounds);
    }

    // Border
    g.setColour(juce::Colours::black);
    g.drawRect(bounds, 1.0f);

    // Draw dB markers
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);

    if (meterType == MeterType::GainReduction)
    {
        auto drawMarker = [&](float db, const juce::String& label)
        {
            float y = juce::jmap(db, -20.0f, 0.0f, bounds.getBottom(), bounds.getY());
            g.drawLine(bounds.getX(), y, bounds.getX() + 5, y, 1.0f);
            g.drawText(label, bounds.getX() + 6, y - 5, 20, 10, juce::Justification::centredLeft);
        };
        drawMarker(0.0f, "0");
        drawMarker(-10.0f, "-10");
        drawMarker(-20.0f, "-20");
    }
    else
    {
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
}

void VUMeter::timerCallback()
{
    float newLevel = getLevelFunc();

    if (meterType == MeterType::GainReduction)
    {
        newLevel = juce::jlimit(-20.0f, 0.0f, newLevel);
        // Faster attack, slower release for GR
        if (newLevel < currentLevel)
            currentLevel = newLevel;
        else
            currentLevel += (newLevel - currentLevel) * 0.1f;
    }
    else
    {
        newLevel = juce::jlimit(-60.0f, 6.0f, newLevel);
        if (newLevel > currentLevel)
            currentLevel = newLevel;
        else
            currentLevel -= 0.5f;
        currentLevel = std::max(currentLevel, -60.0f);
    }

    repaint();
}
