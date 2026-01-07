#pragma once

#include <JuceHeader.h>

class PresetManager
{
   public:
    PresetManager(juce::AudioProcessorValueTreeState& apvts);

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

   private:
    juce::File getDefaultDirectory();

    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::String currentPreset;

    bool isModified = false;
};
