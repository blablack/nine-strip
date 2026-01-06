#pragma once

#include <JuceHeader.h>
#include <functional>

class VUMeter : public juce::Component, private juce::Timer
{
   public:
    VUMeter(std::function<float()> levelGetter);

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

   private:
    std::function<float()> getLevelFunc;
    float currentLevel = -60.0f;
};
