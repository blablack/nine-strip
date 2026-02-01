#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <cstdint>
#include <functional>

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
    void timerCallback() override;

    [[nodiscard]] float getAspectRatio() const { return imageAspectRatio; }

   private:
    MeterType meterType;

    void drawNeedle(juce::Graphics& g, juce::Rectangle<float> bounds) const;

    std::function<float()> getLevelFunc;
    float currentLevel = -60.0f;

    juce::Image backgroundImage;
    juce::Image peakOnImage;
    juce::Image peakOffImage;

    bool isPeakLit = false;
    int peakHoldCounter = 0;
    const int peakHoldDuration = 5;  // 5 frames ≈ 83ms at 60Hz
    const float peakThreshold = 17.9f;

    float backgroundWidth = 750.0f;
    float backgroundHeight = 450.0f;
    float borderWidth = 20.0f;
    float bottomBorderHeight = 20.0f;

    float peakX = 280.0f;
    float peakY = 20.0f;
    float peakSize = 100.0f;

    float imageAspectRatio;
};
