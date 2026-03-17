#pragma once

#include <JuceHeader.h>

class PresetManager : public juce::AudioProcessorValueTreeState::Listener
{
   public:
    explicit PresetManager(juce::AudioProcessorValueTreeState& apvts);
    ~PresetManager() override;

    void savePreset(const juce::String& presetName);
    void deletePreset(const juce::String& presetName);
    void loadPreset(const juce::String& presetName);
    void loadNextPreset();
    void loadPreviousPreset();

    [[nodiscard]] static juce::StringArray getAllPresets();
    [[nodiscard]] juce::String getCurrentPreset() const;

    [[nodiscard]] bool isPresetModified() const { return isModified; }
    void markAsModified() { isModified = true; }
    void markAsUnmodified() { isModified = false; }

    // AudioProcessorValueTreeState::Listener
    void parameterChanged(const juce::String& parameterID, float newValue) override;

   private:
    [[nodiscard]] static juce::File getDefaultDirectory();

    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::String currentPreset;

    bool isModified = false;
};
