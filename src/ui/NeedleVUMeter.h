#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <functional>

class NeedleVUMeter : public juce::Component, private juce::Timer
{
   public:
    NeedleVUMeter(std::function<float()> levelGetter);
    ~NeedleVUMeter() override;

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    float getAspectRatio() const { return imageAspectRatio; }

   private:
    void drawNeedle(juce::Graphics& g, juce::Rectangle<float> bounds);

    std::function<float()> getLevelFunc;
    float currentLevel = -60.0f;

    juce::Image backgroundImage;
    juce::Image peakOnImage;
    juce::Image peakOffImage;

    float backgroundWidth = 750.0f;
    float backgroundHeight = 450.0f;
    float borderWidth = 20.0f;
    float bottomBorderHeight = 20.0f;

    float peakX = 280.0f;
    float peakY = 20.0f;
    float peakSize = 100.0f;

    float imageAspectRatio;
};
