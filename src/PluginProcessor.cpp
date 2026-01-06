#include "PluginProcessor.h"

#include "PluginEditor.h"

NineStripProcessor::NineStripProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      channel9(44100.0),
      highpass2(44100.0),
      lowpass2(44100.0),
      baxandall2(44100.0),
      parametric(44100.0),
      pressure4(44100.0)
{
    // Add parameter listeners for all AirWindows plugin parameters
    // Channel9
    apvts.addParameterListener("consoleType", this);
    apvts.addParameterListener("drive", this);

    // Highpass2
    apvts.addParameterListener("hipass", this);
    apvts.addParameterListener("ls_tite", this);
    apvts.addParameterListener("hp_poles", this);

    // Lowpass2
    apvts.addParameterListener("lowpass", this);
    apvts.addParameterListener("lp_sft_hrd", this);
    apvts.addParameterListener("lp_poles", this);

    // Baxandall2
    apvts.addParameterListener("treble", this);
    apvts.addParameterListener("bass", this);

    // Parametric
    // apvts.addParameterListener("tr_freq", this);
    // apvts.addParameterListener("treble_param", this);
    // apvts.addParameterListener("tr_reso", this);
    apvts.addParameterListener("hm_freq", this);
    apvts.addParameterListener("highmid", this);
    apvts.addParameterListener("hm_reso", this);
    // apvts.addParameterListener("lm_freq", this);
    // apvts.addParameterListener("lowmid", this);
    // apvts.addParameterListener("lm_reso", this);

    // Pressure4
    apvts.addParameterListener("pressure", this);
    apvts.addParameterListener("speed", this);
    apvts.addParameterListener("mewiness", this);
}

NineStripProcessor::~NineStripProcessor()
{
    // Remove all parameter listeners
    apvts.removeParameterListener("consoleType", this);
    apvts.removeParameterListener("drive", this);
    apvts.removeParameterListener("hipass", this);
    apvts.removeParameterListener("ls_tite", this);
    apvts.removeParameterListener("hp_poles", this);
    apvts.removeParameterListener("lowpass", this);
    apvts.removeParameterListener("lp_sft_hrd", this);
    apvts.removeParameterListener("lp_poles", this);
    apvts.removeParameterListener("treble", this);
    apvts.removeParameterListener("bass", this);
    // apvts.removeParameterListener("tr_freq", this);
    // apvts.removeParameterListener("treble_param", this);
    // apvts.removeParameterListener("tr_reso", this);
    apvts.removeParameterListener("hm_freq", this);
    apvts.removeParameterListener("highmid", this);
    apvts.removeParameterListener("hm_reso", this);
    // apvts.removeParameterListener("lm_freq", this);
    // apvts.removeParameterListener("lowmid", this);
    // apvts.removeParameterListener("lm_reso", this);
    apvts.removeParameterListener("pressure", this);
    apvts.removeParameterListener("speed", this);
    apvts.removeParameterListener("mewiness", this);
}

void NineStripProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    // Channel9
    if (parameterID == "consoleType")
        channel9.setParameter(Channel9::kParamA, newValue);
    else if (parameterID == "drive")
        channel9.setParameter(Channel9::kParamB, newValue);

    // Highpass2
    else if (parameterID == "hipass")
        highpass2.setParameter(Highpass2::kParamA, newValue);
    else if (parameterID == "ls_tite")
        highpass2.setParameter(Highpass2::kParamB, newValue);
    else if (parameterID == "hp_poles")
        highpass2.setParameter(Highpass2::kParamC, newValue);

    // Lowpass2
    else if (parameterID == "lowpass")
        lowpass2.setParameter(Lowpass2::kParamA, newValue);
    else if (parameterID == "lp_sft_hrd")
        lowpass2.setParameter(Lowpass2::kParamB, newValue);
    else if (parameterID == "lp_poles")
        lowpass2.setParameter(Lowpass2::kParamC, newValue);

    // Baxandall2
    else if (parameterID == "treble")
        baxandall2.setParameter(Baxandall2::kParamA, newValue);
    else if (parameterID == "bass")
        baxandall2.setParameter(Baxandall2::kParamB, newValue);

    // Parametric
    // else if (parameterID == "tr_freq")
    //    parametric.setParameter(Parametric::kParamA, newValue);
    // else if (parameterID == "treble_param")
    //    parametric.setParameter(Parametric::kParamB, newValue);
    // else if (parameterID == "tr_reso")
    //    parametric.setParameter(Parametric::kParamC, newValue);
    else if (parameterID == "hm_freq")
        parametric.setParameter(Parametric::kParamD, newValue);
    else if (parameterID == "highmid")
        parametric.setParameter(Parametric::kParamE, newValue);
    else if (parameterID == "hm_reso")
        parametric.setParameter(Parametric::kParamF, newValue);
    // else if (parameterID == "lm_freq")
    //     parametric.setParameter(Parametric::kParamG, newValue);
    // else if (parameterID == "lowmid")
    //     parametric.setParameter(Parametric::kParamH, newValue);
    // else if (parameterID == "lm_reso")
    //     parametric.setParameter(Parametric::kParamI, newValue);

    // Pressure4
    else if (parameterID == "pressure")
        pressure4.setParameter(Pressure4::kParamA, newValue);
    else if (parameterID == "speed")
        pressure4.setParameter(Pressure4::kParamB, newValue);
    else if (parameterID == "mewiness")
        pressure4.setParameter(Pressure4::kParamC, newValue);
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NineStripProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Input gain - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "inputGain", "Input Gain", 
        juce::NormalisableRange<float>(-24.0f, 24.0f), 
        0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value / 24.0f) * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() / 10.0f) * 24.0f; 
            })));

    // Channel9
    layout.add(std::make_unique<juce::AudioParameterChoice>("consoleType", "Console Type",
                                                            juce::StringArray{"Neve", "API", "SSL", "Teac", "Mackie"}, 0));

    // Drive - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // Lowpass2 - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lowpass", "Lowpass", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        1.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // LP Soft/Hard - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lp_sft_hrd", "LP Soft/Hard", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value * 20.0f) - 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() + 10.0f) / 20.0f; 
            })));

    // LP Poles - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lp_poles", "LP Slope", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.25f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // Highpass2 - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hipass", "Hipass", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // HP Ls/Tite - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "ls_tite", "HP Ls/Tite", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value * 20.0f) - 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() + 10.0f) / 20.0f; 
            })));

    // HP Poles - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hp_poles", "HP Slope", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.25f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // Baxandall2 - display as -10 to +10 (Pultec-style)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "treble", "High Shelf", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value * 20.0f) - 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() + 10.0f) / 20.0f; 
            })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "bass", "Low Shelf", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value * 20.0f) - 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() + 10.0f) / 20.0f; 
            })));

    // Parametric - display as 0-10
    // layout.add(std::make_unique<juce::AudioParameterFloat>("tr_freq", "Tr Freq", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("treble_param", "Treble", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("tr_reso", "Tr Reso", 0.0f, 1.0f, 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hm_freq", "High-Mid Freq", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "highmid", "High-Mid Gain", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
        .withStringFromValueFunction([](float value, int) { 
            return juce::String((value * 20.0f) - 10.0f, 1);  
        })
        .withValueFromStringFunction([](const juce::String &text) { 
            return (text.getFloatValue() + 10.0f) / 20.0f; 
        })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hm_reso", "High-Mid Reso", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // layout.add(std::make_unique<juce::AudioParameterFloat>("lm_freq", "LM Freq", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("lowmid", "LowMid", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("lm_reso", "LM Reso", 0.0f, 1.0f, 0.5f));

    // Pressure4 - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "pressure", "Pressure", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "speed", "Speed", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.2f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String(value * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return text.getFloatValue() / 10.0f; 
            })));

    // Mewiness - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mewiness", "Mewiness", 
        juce::NormalisableRange<float>(0.0f, 1.0f), 
        0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value * 20.0f) - 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() + 10.0f) / 20.0f; 
            })));

    // Output gain - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "outputGain", "Output Gain", 
        juce::NormalisableRange<float>(-24.0f, 24.0f), 
        0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { 
                return juce::String((value / 24.0f) * 10.0f, 1); 
            })
            .withValueFromStringFunction([](const juce::String &text) { 
                return (text.getFloatValue() / 10.0f) * 24.0f; 
            })));

    // Bypass switches
    layout.add(std::make_unique<juce::AudioParameterBool>("masterBypass", "Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("saturationBypass", "Saturation Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("eqBypass", "EQ Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("compressorBypass", "Compressor Bypass", false));

    return layout;
}

//==============================================================================
const juce::String NineStripProcessor::getName() const { return JucePlugin_Name; }

bool NineStripProcessor::acceptsMidi() const { return false; }
bool NineStripProcessor::producesMidi() const { return false; }
bool NineStripProcessor::isMidiEffect() const { return false; }
double NineStripProcessor::getTailLengthSeconds() const { return 0.0; }

int NineStripProcessor::getNumPrograms() { return 1; }
int NineStripProcessor::getCurrentProgram() { return 0; }
void NineStripProcessor::setCurrentProgram(int index) {}
const juce::String NineStripProcessor::getProgramName(int index) { return {}; }
void NineStripProcessor::changeProgramName(int index, const juce::String &newName) {}

//==============================================================================
void NineStripProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/)
{
    channel9.setSampleRate(sampleRate);
    highpass2.setSampleRate(sampleRate);
    lowpass2.setSampleRate(sampleRate);
    baxandall2.setSampleRate(sampleRate);
    parametric.setSampleRate(sampleRate);
    pressure4.setSampleRate(sampleRate);

    channel9.setParameter(Channel9::kParamC, 1.0f);    // output gain
    highpass2.setParameter(Highpass2::kParamD, 1.0f);  // wet/dry
    lowpass2.setParameter(Lowpass2::kParamD, 1.0f);    // wet/dry

    parametric.setParameter(Parametric::kParamA, 0.5f);  // high freq
    parametric.setParameter(Parametric::kParamB, 0.5f);  // high freq
    parametric.setParameter(Parametric::kParamC, 0.5f);  // high freq
    parametric.setParameter(Parametric::kParamG, 0.5f);  // low freq
    parametric.setParameter(Parametric::kParamH, 0.5f);  // low freq
    parametric.setParameter(Parametric::kParamI, 0.5f);  // low freq

    parametric.setParameter(Parametric::kParamJ, 1.0f);  // wet/dry

    pressure4.setParameter(Pressure4::kParamD, 1.0f);  // output gain

    channel9.setParameter(Channel9::kParamA, apvts.getRawParameterValue("consoleType")->load());
    channel9.setParameter(Channel9::kParamB, apvts.getRawParameterValue("drive")->load());

    highpass2.setParameter(Highpass2::kParamA, apvts.getRawParameterValue("hipass")->load());
    highpass2.setParameter(Highpass2::kParamB, apvts.getRawParameterValue("ls_tite")->load());
    highpass2.setParameter(Highpass2::kParamC, apvts.getRawParameterValue("hp_poles")->load());

    lowpass2.setParameter(Lowpass2::kParamA, apvts.getRawParameterValue("lowpass")->load());
    lowpass2.setParameter(Lowpass2::kParamB, apvts.getRawParameterValue("lp_sft_hrd")->load());
    lowpass2.setParameter(Lowpass2::kParamC, apvts.getRawParameterValue("lp_poles")->load());

    baxandall2.setParameter(Baxandall2::kParamA, apvts.getRawParameterValue("treble")->load());
    baxandall2.setParameter(Baxandall2::kParamB, apvts.getRawParameterValue("bass")->load());

    // parametric.setParameter(Parametric::kParamA, apvts.getRawParameterValue("tr_freq")->load());
    // parametric.setParameter(Parametric::kParamB, apvts.getRawParameterValue("treble_param")->load());
    // parametric.setParameter(Parametric::kParamC, apvts.getRawParameterValue("tr_reso")->load());
    parametric.setParameter(Parametric::kParamD, apvts.getRawParameterValue("hm_freq")->load());
    parametric.setParameter(Parametric::kParamE, apvts.getRawParameterValue("highmid")->load());
    parametric.setParameter(Parametric::kParamF, apvts.getRawParameterValue("hm_reso")->load());
    // parametric.setParameter(Parametric::kParamG, apvts.getRawParameterValue("lm_freq")->load());
    // parametric.setParameter(Parametric::kParamH, apvts.getRawParameterValue("lowmid")->load());
    // parametric.setParameter(Parametric::kParamI, apvts.getRawParameterValue("lm_reso")->load());

    pressure4.setParameter(Pressure4::kParamA, apvts.getRawParameterValue("pressure")->load());
    pressure4.setParameter(Pressure4::kParamB, apvts.getRawParameterValue("speed")->load());
    pressure4.setParameter(Pressure4::kParamC, apvts.getRawParameterValue("mewiness")->load());
}

void NineStripProcessor::releaseResources() {}

bool NineStripProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    // Only support stereo
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
}
void NineStripProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Master bypass - skip all processing
    bool masterBypassed = apvts.getRawParameterValue("masterBypass")->load() > 0.5f;
    if (masterBypassed)
    {
        if (getActiveEditor() != nullptr)
        {
            inputLevelL.store(-60.0f);
            inputLevelR.store(-60.0f);
            outputLevelL.store(-60.0f);
            outputLevelR.store(-60.0f);
        }

        return;  // Early exit, pass audio through untouched
    }

    // Get section bypass states
    bool saturationBypass = apvts.getRawParameterValue("saturationBypass")->load() > 0.5f;
    bool eqBypass = apvts.getRawParameterValue("eqBypass")->load() > 0.5f;
    bool compressorBypass = apvts.getRawParameterValue("compressorBypass")->load() > 0.5f;

    // Get gain values
    float inputGainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("inputGain")->load());
    float outputGainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("outputGain")->load());

    // Apply input gain
    buffer.applyGain(inputGainLinear);

    if (getActiveEditor() != nullptr)
    {
        float inL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        float inR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
        inL = std::max(inL, 0.000001f);  // Floor at -120 dB
        inR = std::max(inR, 0.000001f);
        inputLevelL.store(juce::Decibels::gainToDecibels(inL, -60.0f));
        inputLevelR.store(juce::Decibels::gainToDecibels(inR, -60.0f));
    }

    // Create raw pointer arrays for Airwindows processing
    float *channelData[2] = {
        buffer.getWritePointer(0),  // Left
        buffer.getWritePointer(1)   // Right
    };

    // Process through the plugin chain
    if (!saturationBypass)
    {
        channel9.processReplacing(channelData, channelData, buffer.getNumSamples());
    }

    if (!eqBypass)
    {
        highpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
        lowpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
        baxandall2.processReplacing(channelData, channelData, buffer.getNumSamples());
        parametric.processReplacing(channelData, channelData, buffer.getNumSamples());
    }

    if (!compressorBypass)
    {
        // Measure level before compression
        float preCompL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        float preCompR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
        float preCompLevel = std::max(preCompL, preCompR);
        
        pressure4.processReplacing(channelData, channelData, buffer.getNumSamples());
        
        // Measure level after compression
        float postCompL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        float postCompR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
        float postCompLevel = std::max(postCompL, postCompR);
        
        // Calculate gain reduction in dB
        if (getActiveEditor() != nullptr && preCompLevel > 0.000001f && postCompLevel > 0.000001f)
        {
            float grDb = juce::Decibels::gainToDecibels(postCompLevel / preCompLevel);
            gainReduction.store(std::min(grDb, 0.0f)); // Negative values for reduction
        }
    }
    else if (getActiveEditor() != nullptr)
    {
        gainReduction.store(0.0f);
    }

    // Apply output gain
    buffer.applyGain(outputGainLinear);

    if (getActiveEditor() != nullptr)
    {
        float outL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        float outR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
        outL = std::max(outL, 0.000001f);
        outR = std::max(outR, 0.000001f);
        outputLevelL.store(juce::Decibels::gainToDecibels(outL, -60.0f));
        outputLevelR.store(juce::Decibels::gainToDecibels(outR, -60.0f));
    }
}

void NineStripProcessor::processBlock(juce::AudioBuffer<double> &buffer, juce::MidiBuffer &midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    // Master bypass - skip all processing
    bool masterBypassed = apvts.getRawParameterValue("masterBypass")->load() > 0.5f;
    if (masterBypassed)
    {
        if (getActiveEditor() != nullptr)
        {
            inputLevelL.store(-60.0f);
            inputLevelR.store(-60.0f);
            outputLevelL.store(-60.0f);
            outputLevelR.store(-60.0f);
        }

        return;  // Early exit, pass audio through untouched
    }

    // Get section bypass states
    bool saturationBypass = apvts.getRawParameterValue("saturationBypass")->load() > 0.5f;
    bool eqBypass = apvts.getRawParameterValue("eqBypass")->load() > 0.5f;
    bool compressorBypass = apvts.getRawParameterValue("compressorBypass")->load() > 0.5f;

    // Get gain values
    float inputGainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("inputGain")->load());
    float outputGainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("outputGain")->load());

    // Apply input gain
    buffer.applyGain(inputGainLinear);

    if (getActiveEditor() != nullptr)
    {
        float inL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        float inR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
        inL = std::max(inL, 0.000001f);  // Floor at -120 dB
        inR = std::max(inR, 0.000001f);
        inputLevelL.store(juce::Decibels::gainToDecibels(inL, -60.0f));
        inputLevelR.store(juce::Decibels::gainToDecibels(inR, -60.0f));
    }

    // Create raw pointer arrays for Airwindows processing
    double *channelData[2] = {
        buffer.getWritePointer(0),  // Left
        buffer.getWritePointer(1)   // Right
    };

    // Process through the plugin chain
    if (!saturationBypass)
    {
        channel9.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
    }

    if (!eqBypass)
    {
        highpass2.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
        lowpass2.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
        baxandall2.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
        parametric.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
    }

    if (!compressorBypass)
    {
        pressure4.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
        
        // Get gain reduction from Pressure4
        if (getActiveEditor() != nullptr) 
        {
            float grCoeff = pressure4.getGainReduction();
            // Convert coefficient to dB (coefficient <= 1.0, so result will be <= 0 dB)
            float grDb = juce::Decibels::gainToDecibels(grCoeff);
            gainReduction.store(grDb);
        }
    }
    else if (getActiveEditor() != nullptr)
    {
        gainReduction.store(0.0f);
    }

    // Apply output gain
    buffer.applyGain(outputGainLinear);

    if (getActiveEditor() != nullptr)
    {
        float outL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
        float outR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
        outL = std::max(outL, 0.000001f);
        outR = std::max(outR, 0.000001f);
        outputLevelL.store(juce::Decibels::gainToDecibels(outL, -60.0f));
        outputLevelR.store(juce::Decibels::gainToDecibels(outR, -60.0f));
    }
}

//==============================================================================
bool NineStripProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *NineStripProcessor::createEditor() { return new NineStripProcessorEditor(*this); }

//==============================================================================
void NineStripProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NineStripProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new NineStripProcessor(); }
