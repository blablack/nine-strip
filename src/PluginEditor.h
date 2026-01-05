#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"

class VUMeter : public juce::Component, private juce::Timer
{
   public:
    VUMeter(std::function<float()> levelGetter) : getLevelFunc(levelGetter) { startTimerHz(30); }

    void paint(juce::Graphics& g) override
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

    void timerCallback() override
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

   private:
    std::function<float()> getLevelFunc;
    float currentLevel = -60.0f;
};

class NineStripProcessorEditor : public juce::AudioProcessorEditor
{
   public:
    NineStripProcessorEditor(NineStripProcessor&);
    ~NineStripProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

   private:
    NineStripProcessor& audioProcessor;

    // VU Meters
    VUMeter inputMeterL, inputMeterR;
    VUMeter outputMeterL, outputMeterR;

    // Gain Section
    juce::Slider inputGainSlider;
    juce::Label inputGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;

    juce::Slider outputGainSlider;
    juce::Label outputGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputGainAttachment;

    // Bypass Buttons
    juce::TextButton masterBypassButton{"BYPASS"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> masterBypassAttachment;

    juce::TextButton saturationBypassButton{"SAT"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> saturationBypassAttachment;

    juce::TextButton eqBypassButton{"EQ"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBypassAttachment;

    juce::TextButton compressorBypassButton{"COMP"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compressorBypassAttachment;

    // Channel9 / Saturation
    juce::ComboBox consoleTypeCombo;
    juce::Label consoleTypeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> consoleTypeAttachment;

    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;

    // Filters
    juce::Slider hipassSlider, lowpassSlider;
    juce::Label hipassLabel, lowpassLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hipassAttachment, lowpassAttachment;

    juce::Slider hpPolesSlider, lpPolesSlider;
    juce::Label hpPolesLabel, lpPolesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpPolesAttachment, lpPolesAttachment;

    juce::Slider hpLsTiteSlider, lpSftHrdSlider;
    juce::Label hpLsTiteLabel, lpSftHrdLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpLsTiteAttachment, lpSftHrdAttachment;

    // Baxandall EQ
    juce::Slider trebleSlider, bassSlider;
    juce::Label trebleLabel, bassLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment, bassAttachment;

    // Parametric EQ
    // juce::Slider trFreqSlider, trGainSlider, trResoSlider;
    juce::Slider hmFreqSlider, hmGainSlider, hmResoSlider;
    // juce::Slider lmFreqSlider, lmGainSlider, lmResoSlider;

    // juce::Label trFreqLabel, trGainLabel, trResoLabel;
    juce::Label hmFreqLabel, hmGainLabel, hmResoLabel;
    // juce::Label lmFreqLabel, lmGainLabel, lmResoLabel;

    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trFreqAttachment, trGainAttachment,
    // trResoAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hmFreqAttachment, hmGainAttachment, hmResoAttachment;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lmFreqAttachment, lmGainAttachment,
    // lmResoAttachment;

    // Compressor
    juce::Slider pressureSlider, speedSlider, mewinessSlider;
    juce::Label pressureLabel, speedLabel, mewinessLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pressureAttachment, speedAttachment,
        mewinessAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessorEditor)
};
