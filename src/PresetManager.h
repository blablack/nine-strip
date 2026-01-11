#pragma once

#include <JuceHeader.h>

class PresetManager : public juce::AudioProcessorValueTreeState::Listener
{
   public:
    PresetManager(juce::AudioProcessorValueTreeState& apvts);
    ~PresetManager() override;

    void savePreset(const juce::String& presetName);
    void deletePreset(const juce::String& presetName);
    void loadPreset(const juce::String& presetName);
    void loadNextPreset();
    void loadPreviousPreset();

    juce::StringArray getAllPresets() const;
    juce::String getCurrentPreset() const;

    bool isPresetModified() const { return isModified; }
    void markAsModified() { isModified = true; }
    void markAsUnmodified() { isModified = false; }

    // AudioProcessorValueTreeState::Listener
    void parameterChanged(const juce::String& parameterID, float newValue) override;

   private:
    juce::File getDefaultDirectory() const;

    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::String currentPreset;

    bool isModified = false;
};
