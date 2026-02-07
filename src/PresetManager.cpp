#include "PresetManager.h"

static const juce::String presetExtension = ".ninestrip";

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts)
{
    for (auto* param : apvts.processor.getParameters())
    {
        if (auto* paramWithID = dynamic_cast<juce::RangedAudioParameter*>(param))
        {
            apvts.addParameterListener(paramWithID->paramID, this);
        }
    }
}

PresetManager::~PresetManager()
{
    // ✅ Clean up parameter listeners
    for (auto* param : valueTreeState.processor.getParameters())
    {
        if (auto* paramWithID = dynamic_cast<juce::RangedAudioParameter*>(param))
        {
            valueTreeState.removeParameterListener(paramWithID->paramID, this);
        }
    }
}

void PresetManager::parameterChanged(const juce::String& parameterID, float newValue)
{
    // ✅ Any parameter change marks preset as modified (if a preset is loaded)
    if (!currentPreset.isEmpty())
    {
        isModified = true;
    }
}

juce::File PresetManager::getDefaultDirectory() const  //
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

void PresetManager::savePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    currentPreset = presetName;

    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    juce::File presetFile = getDefaultDirectory().getChildFile(presetName + presetExtension);

    if (!xml->writeTo(presetFile))
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Save Failed",
                                               "Could not save preset: " + presetName);
        return;
    }

    isModified = false;  // Only mark unmodified if save succeeded
}

void PresetManager::deletePreset(const juce::String& presetName)
{
    if (presetName.isEmpty()) return;

    juce::File presetFile = getDefaultDirectory().getChildFile(presetName + presetExtension);

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

    juce::File presetFile = getDefaultDirectory().getChildFile(presetName + presetExtension);

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

juce::StringArray PresetManager::getAllPresets() const
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
