#include "NeedleVUMeter.h"

#include <cmath>
#include <utility>

#include "BinaryData.h"
#include "ImageScaling.h"

NeedleVUMeter::NeedleVUMeter(std::function<float()> levelGetter, MeterType type, juce::Image glassOverlay)
    : meterType(type),
      ballistics(type == MeterType::Level ? -60.0f : 0.0f),
      getLevelFunc(std::move(levelGetter)),
      glassImage(std::move(glassOverlay)),
      imageAspectRatio(backgroundWidth / backgroundHeight)
{
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::needlevu_png, BinaryData::needlevu_pngSize);
    peakOnImage = juce::ImageCache::getFromMemory(BinaryData::peakon_png, BinaryData::peakon_pngSize);
    peakOffImage = juce::ImageCache::getFromMemory(BinaryData::peakoff_png, BinaryData::peakoff_pngSize);

    // The background image is fully opaque and stretched to the bounds, so the parent never needs to paint behind us.
    setOpaque(true);

    startTimerHz(60);
}

NeedleVUMeter::~NeedleVUMeter() { stopTimer(); }

void NeedleVUMeter::resized()
{
    if (backgroundImage.isNull() || getWidth() <= 0 || getHeight() <= 0) return;

    // Stretched to the exact bounds rather than centred: the bounds are aspect-constrained to within 1 px, and an
    // opaque component must cover every pixel, so absorb the rounding as sub-pixel stretch instead of a transparent sliver.
    scaledBackground = downscaleSmoothly(backgroundImage, getWidth(), getHeight());

    // Same 750x450 canvas as the face, so it goes through the same stretch to the bounds.
    if (glassImage.isValid()) scaledGlass = downscaleSmoothly(glassImage, getWidth(), getHeight());

    float scale =
        juce::jmin(static_cast<float>(getWidth()) / backgroundWidth, static_cast<float>(getHeight()) / backgroundHeight);

    int ps = juce::roundToInt(peakSize * scale);
    if (ps > 0)
    {
        scaledPeakOnImage = downscaleSmoothly(peakOnImage, ps, ps);
        scaledPeakOffImage = downscaleSmoothly(peakOffImage, ps, ps);
    }
}

void NeedleVUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // 1. Draw full background image maintaining aspect ratio
    if (scaledBackground.isValid())
    {
        g.drawImageAt(scaledBackground, 0, 0);
    }
    else
    {
        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);
        g.drawImage(backgroundImage, bounds, juce::RectanglePlacement::stretchToFit);
    }

    // 2. Calculate scaled border sizes based on image dimensions
    float scaleX = bounds.getWidth() / backgroundWidth;
    float scaleY = bounds.getHeight() / backgroundHeight;

    // Use the minimum scale to maintain aspect ratio
    float scale = juce::jmin(scaleX, scaleY);

    float scaledBorderWidth = borderWidth * scale;

    if (meterType == MeterType::Level)
    {
        float peakScaled = peakSize * scale;
        float offsetX = (bounds.getWidth() - peakScaled) - (100 * scale);
        float offsetY = (bounds.getHeight() - peakScaled) - (100 * scale);

        // 5. Scale and position the peak indicator
        juce::Rectangle<float> peakBounds(offsetX, offsetY, peakScaled, peakScaled);

        if (isPeakLit)
        {
            if (scaledPeakOnImage.isValid())
                g.drawImageAt(scaledPeakOnImage, static_cast<int>(offsetX), static_cast<int>(offsetY));
            else
                g.drawImage(peakOnImage, peakBounds, juce::RectanglePlacement::stretchToFit);
        }
        else
        {
            if (scaledPeakOffImage.isValid())
                g.drawImageAt(scaledPeakOffImage, static_cast<int>(offsetX), static_cast<int>(offsetY));
            else
                g.drawImage(peakOffImage, peakBounds, juce::RectanglePlacement::stretchToFit);
        }
    }

    // 3. Draw the needle clipped to the window so it never crosses the frame
    {
        juce::Graphics::ScopedSaveState clip(g);
        g.reduceClipRegion(bounds.reduced(scaledBorderWidth).toNearestInt());
        drawNeedle(g, bounds, scale);
    }

    // 4. Dirty glass in front of everything, frame included: the pane covers the whole meter
    if (scaledGlass.isValid()) g.drawImageAt(scaledGlass, 0, 0);
}

void NeedleVUMeter::drawNeedle(juce::Graphics& g, juce::Rectangle<float> bounds, float scale) const
{
    // Needle pivot point. The offset below the meter is in image pixels so the pivot moves with the printed
    // scale when the meter is resized; the angle table below was calibrated against the artwork at that geometry.
    float pivotX = bounds.getCentreX();
    float pivotY = bounds.getBottom() + needlePivotBelowBottom * scale;

    float vuLevel = NAN;
    if (meterType == MeterType::Level)
    {
        vuLevel = currentLevel + 21.0f;  // Convert dBFS RMS to VU HERE
    }
    else
    {
        vuLevel = currentLevel;  // Direct VU level
    }

    float clampedLevel = juce::jlimit(-20.0f, 3.0f, vuLevel);

    float angle = NAN;

    if (clampedLevel <= -10.0f)
        angle = juce::jmap(clampedLevel, -20.0f, -10.0f, -40.0f, -30.0f);
    else if (clampedLevel <= -7.0f)
        angle = juce::jmap(clampedLevel, -10.0f, -7.0f, -30.0f, -23.0f);
    else if (clampedLevel <= -5.0f)
        angle = juce::jmap(clampedLevel, -7.0f, -5.0f, -23.0f, -16.0f);
    else if (clampedLevel <= -3.0f)
        angle = juce::jmap(clampedLevel, -5.0f, -3.0f, -16.0f, -6.0f);
    else if (clampedLevel <= -2.0f)
        angle = juce::jmap(clampedLevel, -3.0f, -2.0f, -6.0f, 0.0f);
    else if (clampedLevel <= -1.0f)
        angle = juce::jmap(clampedLevel, -2.0f, -1.0f, 0.0f, 6.0f);
    else if (clampedLevel <= 0.0f)
        angle = juce::jmap(clampedLevel, -1.0f, 0.0f, 6.0f, 14.5f);
    else if (clampedLevel <= 1.0f)
        angle = juce::jmap(clampedLevel, 0.0f, 1.0f, 14.5f, 23.5f);
    else if (clampedLevel <= 2.0f)
        angle = juce::jmap(clampedLevel, 1.0f, 2.0f, 23.5f, 32.5f);
    else
        angle = juce::jmap(clampedLevel, 2.0f, 3.0f, 32.5f, 43.0f);

    angle *= juce::MathConstants<float>::pi / 180.0f;  // Convert to radians

    // Needle length
    float needleLength = bounds.getHeight() * 0.9f;

    // Calculate needle endpoint
    float endX = pivotX + needleLength * std::sin(angle);
    float endY = pivotY - needleLength * std::cos(angle);

    // Draw needle
    g.setColour(juce::Colour(40, 40, 40));
    juce::Line<float> needle(pivotX, pivotY, endX, endY);
    g.drawLine(needle, juce::jmax(1.0f, needleWidth * scale));
}

void NeedleVUMeter::timerCallback()
{
    const float rawDb = getLevelFunc();  // dBFS RMS (Level) or GR dB (GainReduction)

    currentLevel = ballistics.tick(rawDb);  // smooth through second-order ballistic

    if (meterType == MeterType::Level)
    {
        const float rawVU = rawDb + 21.0f;  // raw (pre-ballistic) VU for peak LED
        if (rawVU >= peakThreshold)
        {
            isPeakLit = true;
            peakHoldCounter = peakHoldDuration;
        }
        if (peakHoldCounter > 0)
        {
            --peakHoldCounter;
            isPeakLit = true;
        }
        else
        {
            isPeakLit = false;
        }
    }

    // Skip the repaint while the needle sits still (idle transport, settled ballistics). Compare against
    // the last *painted* level, not the previous tick, so a slow creep still repaints once it accumulates.
    if (std::abs(currentLevel - lastPaintedLevel) < kRepaintThresholdDb && isPeakLit == lastPaintedPeakLit) return;

    lastPaintedLevel = currentLevel;
    lastPaintedPeakLit = isPeakLit;
    repaint();
}
