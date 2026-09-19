#pragma once

#include <JuceHeader.h>

// Saves and loads the APVTS state as XML preset files and remembers which preset is loaded.
//
// The "modified" flag is driven by NineStripProcessor::valueTreePropertyChanged, which runs when the APVTS
// flushes parameter values to its tree (never on the audio thread). This class deliberately has no APVTS
// parameter listener of its own: those fire synchronously from setValueNotifyingHost, i.e. on the audio
// thread during host automation, where touching currentPreset would race the message thread.
class PresetManager
{
   public:
    explicit PresetManager(juce::AudioProcessorValueTreeState& apvts);

    // Preset names double as file names. This is the name a preset typed as `presetName` is stored under:
    // characters that are illegal in file names on any platform are dropped and the result is trimmed.
    [[nodiscard]] static juce::String toStoredName(const juce::String& presetName);
    [[nodiscard]] static bool presetExists(const juce::String& storedName);

    // Writes the current state under toStoredName(presetName) and makes it the current preset. Returns false,
    // leaving the current preset untouched, if the name is empty once legalised or the file cannot be written.
    bool savePreset(const juce::String& presetName);
    void deletePreset(const juce::String& presetName);
    void loadPreset(const juce::String& presetName);
    void loadNextPreset();
    void loadPreviousPreset();

    [[nodiscard]] static juce::StringArray getAllPresets();
    [[nodiscard]] juce::String getCurrentPreset() const;

    [[nodiscard]] bool isPresetModified() const { return isModified; }
    void markAsModified() { isModified = true; }
    void markAsUnmodified() { isModified = false; }

    // Restore the preset name/modified flag from saved host state without reading the preset file
    void setCurrentPreset(const juce::String& presetName, bool modified)
    {
        currentPreset = presetName;
        isModified = modified;
    }

   private:
    [[nodiscard]] static juce::File getDefaultDirectory();
    [[nodiscard]] static juce::File getPresetFile(const juce::String& storedName);

    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::String currentPreset;

    bool isModified = false;
};
