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
class NineStripProcessor : public juce::AudioProcessor {
   public:
    //==============================================================================
    NineStripProcessor();
    ~NineStripProcessor() override;

    juce::AudioProcessorValueTreeState apvts;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

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

    Channel9 channel9;
    Highpass2 highpass2;
    Lowpass2 lowpass2;
    Baxandall2 baxandall2;
    Parametric parametric;
    Pressure4 pressure4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessor)
};
