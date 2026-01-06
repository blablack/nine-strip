#pragma once

#include <JuceHeader.h>

#include "airwindows/Baxandall2.h"
#include "airwindows/Channel9.h"
#include "airwindows/Highpass2.h"
#include "airwindows/Lowpass2.h"
#include "airwindows/Parametric.h"
#include "airwindows/Pressure4.h"

//==============================================================================
/**
 */
class NineStripProcessor : public juce::AudioProcessor, private juce::AudioProcessorValueTreeState::Listener
{
   public:
    //==============================================================================
    NineStripProcessor();
    ~NineStripProcessor() override;

    //==============================================================================
    juce::AudioProcessorValueTreeState apvts;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    float getInputLevelL() const { return inputLevelL.load(); }
    float getInputLevelR() const { return inputLevelR.load(); }
    float getOutputLevelL() const { return outputLevelL.load(); }
    float getOutputLevelR() const { return outputLevelR.load(); }
    float getGainReduction() const { return gainReduction.load(); }

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) override;
    void processBlock(juce::AudioBuffer<double> &buffer, juce::MidiBuffer &midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;

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
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void parameterChanged(const juce::String &parameterID, float newValue) override;

    Channel9 channel9;
    Highpass2 highpass2;
    Lowpass2 lowpass2;
    Baxandall2 baxandall2;
    Parametric parametric;
    Pressure4 pressure4;

    // Level meters
    std::atomic<float> inputLevelL{-60.0f};
    std::atomic<float> inputLevelR{-60.0f};
    std::atomic<float> outputLevelL{-60.0f};
    std::atomic<float> outputLevelR{-60.0f};
    std::atomic<float> gainReduction{0.0f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessor)
};
