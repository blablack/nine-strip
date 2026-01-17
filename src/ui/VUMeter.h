#pragma once

// #include <JuceHeader.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <functional>

class VUMeter : public juce::Component, private juce::Timer
{
   public:
    enum class MeterType
    {
        Level,
        GainReduction
    };

    VUMeter(std::function<float()> levelGetter, MeterType type = MeterType::Level);
    ~VUMeter() override;

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

   private:
    std::function<float()> getLevelFunc;
    float currentLevel = -60.0f;
    MeterType meterType;
};
