#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <cstdint>
#include <functional>

#include "VUMeterBallistics.h"

class NeedleVUMeter : public juce::Component, private juce::Timer
{
   public:
    enum class MeterType : std::uint8_t
    {
        Level,         // Standard VU: -20dB to +3dB, left to right
        GainReduction  // GR meter: 0dB to -20dB, right to left
    };

    NeedleVUMeter(std::function<float()> levelGetter, MeterType type);
    ~NeedleVUMeter() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    [[nodiscard]] float getAspectRatio() const { return imageAspectRatio; }

   private:
    MeterType meterType;

    VUMeterBallistics ballistics;

    void drawNeedle(juce::Graphics& g, juce::Rectangle<float> bounds, float scale) const;

    std::function<float()> getLevelFunc;
    float currentLevel = -60.0f;

    juce::Image backgroundImage;
    juce::Image scaledBackground;
    juce::Image peakOnImage;
    juce::Image scaledPeakOnImage;
    juce::Image peakOffImage;
    juce::Image scaledPeakOffImage;

    bool isPeakLit = false;
    int peakHoldCounter = 0;

    // What the last repaint showed; the timer only repaints when the needle or LED would visibly change.
    float lastPaintedLevel = -60.0f;
    bool lastPaintedPeakLit = false;
    static constexpr float kRepaintThresholdDb = 0.05f;
    const int peakHoldDuration = 5;  // 5 frames ≈ 83ms at 60Hz
    const float peakThreshold = 17.9f;

    float backgroundWidth = 750.0f;
    float backgroundHeight = 450.0f;
    float borderWidth = 20.0f;
    float bottomBorderHeight = 10.0f;

    float peakSize = 100.0f;

    // Needle geometry in background-image pixels (750x450), scaled with the meter.
    float needlePivotBelowBottom = 99.0f;
    float needleWidth = 10.0f;

    float imageAspectRatio;
};
