#pragma once

#include <JuceHeader.h>

#include "Lowpass2.h"
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

    juce::AudioParameterFloat *inputGain;

    Channel9 channel9;
    juce::AudioParameterChoice *channel9_consoleType;
    juce::AudioParameterFloat *channel9_drive;

    Highpass2 highpass2;
    juce::AudioParameterFloat *highpass2_hipass;
    juce::AudioParameterFloat *highpass2_ls_tite;
    juce::AudioParameterFloat *highpass2_poles;

    Lowpass2 lowpass2;
    juce::AudioParameterFloat *lowpass2_lowpass;
    juce::AudioParameterFloat *lowpass2_sft_hrd;
    juce::AudioParameterFloat *lowpass2_poles;

    Baxandall2 baxandall2;
    juce::AudioParameterFloat *baxandall2_treble;
    juce::AudioParameterFloat *baxandall2_bass;

    Parametric parametric;
    juce::AudioParameterFloat *parametric_tr_freq;
    juce::AudioParameterFloat *parametric_treble;
    juce::AudioParameterFloat *parametric_tr_reso;
    juce::AudioParameterFloat *parametric_hm_freq;
    juce::AudioParameterFloat *parametric_highmid;
    juce::AudioParameterFloat *parametric_hm_reso;
    juce::AudioParameterFloat *parametric_lm_freq;
    juce::AudioParameterFloat *parametric_lowmid;
    juce::AudioParameterFloat *parametric_lm_reso;

    Pressure4 pressure4;
    juce::AudioParameterFloat *pressure4_pressure;
    juce::AudioParameterFloat *pressure4_speed;
    juce::AudioParameterFloat *pressure4_mewiness;
    juce::AudioParameterFloat *pressure4_output_gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessor)
};
