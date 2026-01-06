#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "VUMeter.h"

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
    VUMeter grMeter;

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
