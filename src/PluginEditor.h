#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "VUMeter.h"

class NineStripProcessorEditor : public juce::AudioProcessorEditor,
                                 public juce::ComboBox::Listener,
                                 public juce::Button::Listener,
                                 public juce::AudioProcessorValueTreeState::Listener
{
   public:
    NineStripProcessorEditor(NineStripProcessor&);
    ~NineStripProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void updatePresetDisplay();
    void updatePresetComboBox();

    void parameterChanged(const juce::String& parameterID, float newValue) override;

   private:
    NineStripProcessor& audioProcessor;

    juce::ComponentBoundsConstrainer constrainer;

    // Main layout grid
    juce::Grid mainGrid;

    // Preset Management Panel
    juce::Component presetPanel;
    juce::ComboBox presetComboBox;
    juce::TextButton savePresetButton;
    juce::TextButton deletePresetButton;
    juce::TextButton previousPresetButton;
    juce::TextButton nextPresetButton;

    // Console & Saturation
    juce::GroupComponent consoleSatGroup;
    juce::ComboBox consoleTypeCombo;
    juce::Slider driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> consoleTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    juce::ToggleButton saturationBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> saturationBypassAttachment;

    // High Pass Filter
    juce::GroupComponent highPassGroup;
    juce::Slider hipassSlider, hpLsTiteSlider, hpPolesSlider;
    juce::Label hipassLabel, hpLsTiteLabel, hpPolesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hipassAttachment, hpLsTiteAttachment,
        hpPolesAttachment;

    // Low Pass Filter
    juce::GroupComponent lowPassGroup;
    juce::Slider lowpassSlider, lpSftHrdSlider, lpPolesSlider;
    juce::Label lowpassLabel, lpSftHrdLabel, lpPolesLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowpassAttachment, lpSftHrdAttachment,
        lpPolesAttachment;

    // High Shelf
    juce::GroupComponent highShelfGroup;
    juce::Slider trebleSlider;
    juce::Label trebleLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;

    // High-Mid EQ
    juce::GroupComponent highMidGroup;
    juce::Slider hmFreqSlider, hmGainSlider, hmResoSlider;
    juce::Label hmFreqLabel, hmGainLabel, hmResoLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hmFreqAttachment, hmGainAttachment, hmResoAttachment;

    // Low Shelf
    juce::GroupComponent lowShelfGroup;
    juce::Slider bassSlider;
    juce::Label bassLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;

    // EQ Bypass
    juce::ToggleButton eqBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBypassAttachment;

    // Compressor
    juce::GroupComponent compressorGroup;
    juce::Slider pressureSlider, speedSlider, mewinessSlider;
    juce::Label pressureLabel, speedLabel, mewinessLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pressureAttachment, speedAttachment,
        mewinessAttachment;
    VUMeter grMeter;
    juce::ToggleButton compressorBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compressorBypassAttachment;

    // Meters
    juce::GroupComponent metersGroup;
    VUMeter measuredMeterL, measuredMeterR;
    juce::TextButton vuMeterModeButton{"Input"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> vuMeterModeAttachment;

    // Gain
    juce::GroupComponent gainGroup;
    juce::Slider inputGainSlider, outputGainSlider;
    juce::Label inputGainLabel, outputGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment, outputGainAttachment;
    juce::TextButton masterBypassButton{"Bypass"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> masterBypassAttachment;

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button* button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessorEditor)
};