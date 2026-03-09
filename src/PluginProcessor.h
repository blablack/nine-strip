#pragma once

#include <JuceHeader.h>

#include "DCBlocker.h"
#include "PresetManager.h"
#include "airwindows/Baxandall2.h"
#include "airwindows/Channel9.h"
#include "airwindows/Highpass2.h"
#include "airwindows/Interstage.h"
#include "airwindows/Lowpass2.h"
#include "airwindows/Parametric.h"
#include "airwindows/Pressure4.h"
#include "airwindows/PurestGain.h"

//==============================================================================
/**
 */
class NineStripProcessor : public juce::AudioProcessor,
                           private juce::AudioProcessorValueTreeState::Listener,
                           private juce::ValueTree::Listener
{
   public:
    //==============================================================================
    NineStripProcessor();
    ~NineStripProcessor() override;

    bool supportsDoublePrecisionProcessing() const override { return true; }

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    juce::AudioProcessorValueTreeState &getAPVTS() { return apvts; }

    float getMeasuredLevelL() const { return measuredLevelL.load(); }
    float getMeasuredLevelR() const { return measuredLevelR.load(); }
    float getGainReduction() const { return gainReduction.load(); }

    PresetManager &getPresetManager() { return *presetManager; }

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;
    void processBlock(juce::AudioBuffer<double> &buffer, juce::MidiBuffer &midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;
    void editorStateChanged(bool isOpen) { editorOpen.store(isOpen); }

    juce::ApplicationProperties &getAppProperties() { return appProperties; }

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;

   private:
    juce::ApplicationProperties appProperties;

    juce::AudioProcessorValueTreeState apvts;

    // Cached APVTS parameter pointers — set once in prepareToPlay, read on audio thread
    std::atomic<float> *paramMasterBypass = nullptr;
    std::atomic<float> *paramSatBypass = nullptr;
    std::atomic<float> *paramFilterBypass = nullptr;
    std::atomic<float> *paramEqBypass = nullptr;
    std::atomic<float> *paramCompBypass = nullptr;
    std::atomic<float> *paramInputMeasured = nullptr;

    const std::vector<juce::String> parameterIDs = {"inputGain", "consoleType", "drive",    "hipass", "ls_tite",  "hp_poles",
                                                    "lowpass",   "lp_sft_hrd",  "lp_poles", "treble", "bass",     "hm_freq",
                                                    "highmid",   "hm_reso",     "pressure", "speed",  "mewiness", "outputGain"};

    std::atomic<bool> editorOpen{false};

    void parameterChanged(const juce::String &parameterID, float newValue) override;
    void valueTreePropertyChanged(juce::ValueTree &, const juce::Identifier &) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void setupParameterListeners();
    void removeParameterListeners();

    template <typename SampleType>
    void processBlockInternal(juce::AudioBuffer<SampleType> &buffer);

    std::unique_ptr<PresetManager> presetManager;

    Channel9 channel9;
    Highpass2 highpass2;
    Lowpass2 lowpass2;
    Baxandall2 baxandall2;
    Parametric parametric;
    Pressure4 pressure4;
    PurestGain inputPurestGain;
    PurestGain outputPurestGain;
    DCBlocker dcBlocker;
    Interstage interstage;

    // Level meters
    template <typename SampleType>
    void updateMeters(const juce::AudioBuffer<SampleType> &buffer, int numSamples);

    juce::AudioBuffer<float> emptyMeterBufferFloat;
    juce::AudioBuffer<double> emptyMeterBufferDouble;

    void updateGRMeter(float gainReductionLinear);

    std::atomic<float> measuredLevelL{-60.0f};
    std::atomic<float> measuredLevelR{-60.0f};
    std::atomic<float> gainReduction{0.0f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessor)
};
