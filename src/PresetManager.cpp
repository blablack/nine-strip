#include "PresetManager.h"

static const juce::String presetExtension = ".ninestrip";

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts) {}

juce::File PresetManager::getDefaultDirectory()
{
    juce::File rootFolder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);

#ifdef JUCE_MAC
    rootFolder = rootFolder.getChildFile("Audio").getChildFile("Presets");
#endif

    rootFolder = rootFolder.getChildFile("NineStrip");

    // Create directory if it doesn't exist
    if (!rootFolder.exists()) rootFolder.createDirectory();

    return rootFolder;
}

void PresetManager::savePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    currentPreset = presetName;
    isModified = false;

    // Get the current state from APVTS
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    // Create the preset file
    juce::File presetFile = getDefaultDirectory().getChildFile(presetName + presetExtension);

    // Write XML to file
    xml->writeTo(presetFile);
}

void PresetManager::deletePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    juce::File presetFile = getDefaultDirectory().getChildFile(presetName + presetExtension);

    if (presetFile.existsAsFile())
    {
        presetFile.deleteFile();

        if (currentPreset == presetName) currentPreset = "";
    }
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    juce::File presetFile = getDefaultDirectory().getChildFile(presetName + presetExtension);

    if (!presetFile.existsAsFile()) return;

    // Parse XML from file
    std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(presetFile);

    if (xml == nullptr) return;

    // Convert XML to ValueTree
    auto valueTree = juce::ValueTree::fromXml(*xml);

    // Replace the plugin state
    valueTreeState.replaceState(valueTree);

    currentPreset = presetName;
    isModified = false;
}

void PresetManager::loadNextPreset()
{
    juce::StringArray presets = getAllPresets();

    if (presets.isEmpty()) return;

    int currentIndex = presets.indexOf(currentPreset);

    // If current preset not found or at end, wrap to beginning
    int nextIndex = (currentIndex + 1) % presets.size();

    loadPreset(presets[nextIndex]);
}

void PresetManager::loadPreviousPreset()
{
    juce::StringArray presets = getAllPresets();

    if (presets.isEmpty()) return;

    int currentIndex = presets.indexOf(currentPreset);

    // If current preset not found or at beginning, wrap to end
    int previousIndex = (currentIndex - 1 < 0) ? presets.size() - 1 : currentIndex - 1;

    loadPreset(presets[previousIndex]);
}

juce::StringArray PresetManager::getAllPresets() const
{
    juce::StringArray presets;

    juce::File defaultDirectory = const_cast<PresetManager*>(this)->getDefaultDirectory();

    // Find all preset files in the directory
    auto presetFiles = defaultDirectory.findChildFiles(juce::File::findFiles, false, "*" + presetExtension);

    for (const auto& file : presetFiles)
    {
        presets.add(file.getFileNameWithoutExtension());
    }

    presets.sort(true);  // Sort alphabetically

    return presets;
}

juce::String PresetManager::getCurrentPreset() const { return currentPreset; }
