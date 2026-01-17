#include "NeedleVUMeter.h"

#include "BinaryData.h"

NeedleVUMeter::NeedleVUMeter(std::function<float()> levelGetter)
    : getLevelFunc(levelGetter), imageAspectRatio(backgroundWidth / backgroundHeight)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::needlevu_png, BinaryData::needlevu_pngSize);
    peakOnImage = juce::ImageCache::getFromMemory(BinaryData::peakon_png, BinaryData::peakon_pngSize);
    peakOffImage = juce::ImageCache::getFromMemory(BinaryData::peakoff_png, BinaryData::peakoff_pngSize);

    startTimerHz(60);
}

NeedleVUMeter::~NeedleVUMeter() { stopTimer(); }

void NeedleVUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // 1. Draw full background image maintaining aspect ratio
    g.drawImage(backgroundImage, bounds, juce::RectanglePlacement::centred);

    // 2. Calculate scaled border sizes based on image dimensions
    float scaleX = bounds.getWidth() / backgroundWidth;
    float scaleY = bounds.getHeight() / backgroundHeight;

    // Use the minimum scale to maintain aspect ratio
    float scale = juce::jmin(scaleX, scaleY);

    float scaledBorderWidth = borderWidth * scale;
    float scaledBottomBorder = bottomBorderHeight * scale;

    float peakScaled = peakSize * scale;
    float offsetX = (bounds.getWidth() - peakScaled) - (100 * scale);
    float offsetY = (bounds.getHeight() - peakScaled) - (100 * scale);

    // 5. Scale and position the peak indicator
    juce::Rectangle<float> peakBounds(offsetX, offsetY, peakScaled, peakScaled);

    g.drawImage(peakOffImage, peakBounds, juce::RectanglePlacement::stretchToFit);

    // 3. Set clipping region to exclude borders
    auto meterArea = bounds.reduced(scaledBorderWidth);
    meterArea.removeFromBottom(scaledBottomBorder);
    g.reduceClipRegion(meterArea.toNearestInt());

    // 4. Draw needle (won't appear over borders due to clipping)
    drawNeedle(g, bounds);
}

void NeedleVUMeter::drawNeedle(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Needle pivot point
    float pivotX = bounds.getCentreX();
    // float pivotY = bounds.getBottom() - bottomBorderHeight;
    float pivotY = bounds.getBottom() + 10.0f;

    float clampedLevel = juce::jlimit(-20.0f, 3.0f, currentLevel);

    // Map dB level to angle (-20 to +3 dB = -45° to +45°)
    float angle = juce::jmap(clampedLevel, -20.0f, 3.0f, -45.0f * juce::MathConstants<float>::pi / 180.0f,
                             45.0f * juce::MathConstants<float>::pi / 180.0f);

    // Needle length
    float needleLength = bounds.getHeight() * 0.8f;

    // Calculate needle endpoint
    float endX = pivotX + needleLength * std::sin(angle);
    float endY = pivotY - needleLength * std::cos(angle);

    // Draw needle
    g.setColour(juce::Colours::black);
    juce::Line<float> needle(pivotX, pivotY, endX, endY);
    g.drawLine(needle, 2.0f);
}

void NeedleVUMeter::timerCallback()
{
    currentLevel = getLevelFunc();

    repaint();
}
