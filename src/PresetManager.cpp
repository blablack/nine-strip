#include "PresetManager.h"

static const juce::String presetExtension = ".ninestrip";

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts) {}

juce::File PresetManager::getDefaultDirectory()
{
    juce::File rootFolder = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory);

#ifdef JUCE_MAC
    rootFolder = rootFolder.getChildFile("Audio").getChildFile("Presets");
#endif

    rootFolder = rootFolder.getChildFile("NineStrip").getChildFile("Presets");

    // Create directory if it doesn't exist
    if (!rootFolder.exists())
    {
        auto result = rootFolder.createDirectory();
        if (!result.wasOk())
        {
            DBG("Failed to create preset directory: " + rootFolder.getFullPathName());
        }
    }

    return rootFolder;
}

juce::File PresetManager::getPresetFile(const juce::String& storedName)
{
    return getDefaultDirectory().getChildFile(storedName + presetExtension);
}

juce::String PresetManager::toStoredName(const juce::String& presetName)
{
    return juce::File::createLegalFileName(presetName).trim();
}

bool PresetManager::presetExists(const juce::String& storedName)
{
    return storedName.isNotEmpty() && getPresetFile(storedName).existsAsFile();
}

bool PresetManager::savePreset(const juce::String& presetName)
{
    const auto storedName = toStoredName(presetName);
    if (storedName.isEmpty()) return false;

    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    if (!xml->writeTo(getPresetFile(storedName))) return false;

    // Only adopt the name once the file exists, so a failed save cannot leave the UI naming a preset that isn't there.
    currentPreset = storedName;
    isModified = false;
    return true;
}

void PresetManager::deletePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    juce::File presetFile = getPresetFile(presetName);

    if (presetFile.existsAsFile())
    {
        presetFile.deleteFile();

        if (currentPreset == presetName)
        {
            currentPreset = "";
            isModified = false;
        }
    }
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    juce::File presetFile = getPresetFile(presetName);

    if (!presetFile.existsAsFile()) return;

    // Parse XML from file
    std::unique_ptr<juce::XmlElement> xml = juce::XmlDocument::parse(presetFile);

    if (xml == nullptr) return;

    // Convert XML to ValueTree
    auto valueTree = juce::ValueTree::fromXml(*xml);

    // Replace the plugin state (thread-safe)
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

juce::StringArray PresetManager::getAllPresets()
{
    juce::StringArray presets;

    juce::File defaultDirectory = getDefaultDirectory();

    // Find all preset files in the directory
    auto presetFiles = defaultDirectory.findChildFiles(juce::File::findFiles, false, "*" + presetExtension);

    for (const auto& file : presetFiles)
    {
        presets.add(file.getFileNameWithoutExtension());
    }

    presets.sort(true);

    return presets;
}

juce::String PresetManager::getCurrentPreset() const { return currentPreset; }
