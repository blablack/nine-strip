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
    presetManager = std::make_unique<PresetManager>(apvts);

    apvts.state.addListener(this);
    setupParameterListeners();

    meterUpdateCounter = 0;
}

NineStripProcessor::~NineStripProcessor()
{
    apvts.state.removeListener(this);
    removeParameterListeners();
}

void NineStripProcessor::setupParameterListeners()
{
    /*
        tr_freq
        treble_param
        tr_reso
        lm_freq
        lowmid
        lm_reso
    */

    const std::vector<juce::String> parameterIDs = {"consoleType", "drive",    "hipass", "ls_tite", "hp_poles", "lowpass",
                                                    "lp_sft_hrd",  "lp_poles", "treble", "bass",    "hm_freq",  "highmid",
                                                    "hm_reso",     "pressure", "speed",  "mewiness"};

    for (const auto &id : parameterIDs) apvts.addParameterListener(id, this);
}

void NineStripProcessor::removeParameterListeners()
{
    const std::vector<juce::String> parameterIDs = {"consoleType", "drive",    "hipass", "ls_tite", "hp_poles", "lowpass",
                                                    "lp_sft_hrd",  "lp_poles", "treble", "bass",    "hm_freq",  "highmid",
                                                    "hm_reso",     "pressure", "speed",  "mewiness"};

    for (const auto &id : parameterIDs) apvts.removeParameterListener(id, this);
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

void NineStripProcessor::valueTreePropertyChanged(juce::ValueTree &, const juce::Identifier &)
{
    if (presetManager)
    {
        presetManager->markAsModified();

        // Notify editor to update display
        if (auto *editor = dynamic_cast<NineStripProcessorEditor *>(getActiveEditor())) editor->updatePresetDisplay();
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NineStripProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Input gain - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "inputGain", "Input Gain", juce::NormalisableRange<float>(-24.0f, 24.0f), 0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value / 24.0f) * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() / 10.0f) * 24.0f; })));

    // Channel9
    layout.add(std::make_unique<juce::AudioParameterChoice>("consoleType", "Console Type",
                                                            juce::StringArray{"Neve", "API", "SSL", "Teac", "Mackie"}, 0));

    // Drive - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // Lowpass2 - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lowpass", "Lowpass", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // LP Soft/Hard - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lp_sft_hrd", "LP Soft/Hard", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 20.0f) - 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 10.0f) / 20.0f; })));

    // LP Poles - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lp_poles", "LP Slope", juce::NormalisableRange<float>(0.0f, 1.0f), 0.25f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // Highpass2 - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hipass", "Hipass", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // HP Ls/Tite - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "ls_tite", "HP Ls/Tite", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 20.0f) - 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 10.0f) / 20.0f; })));

    // HP Poles - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hp_poles", "HP Slope", juce::NormalisableRange<float>(0.0f, 1.0f), 0.25f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // Baxandall2 - display as -10 to +10 (Pultec-style)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "treble", "High Shelf", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 20.0f) - 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 10.0f) / 20.0f; })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "bass", "Low Shelf", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 20.0f) - 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 10.0f) / 20.0f; })));

    // Parametric - display as 0-10
    // layout.add(std::make_unique<juce::AudioParameterFloat>("tr_freq", "Tr Freq", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("treble_param", "Treble", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("tr_reso", "Tr Reso", 0.0f, 1.0f, 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hm_freq", "High-Mid Freq", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "highmid", "High-Mid Gain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 20.0f) - 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 10.0f) / 20.0f; })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hm_reso", "High-Mid Q", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // layout.add(std::make_unique<juce::AudioParameterFloat>("lm_freq", "LM Freq", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("lowmid", "LowMid", 0.0f, 1.0f, 0.5f));
    // layout.add(std::make_unique<juce::AudioParameterFloat>("lm_reso", "LM Reso", 0.0f, 1.0f, 0.5f));

    // Pressure4 - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "pressure", "Pressure", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "speed", "Speed", juce::NormalisableRange<float>(0.0f, 1.0f), 0.2f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // Mewiness - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "mewiness", "Mewiness", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 20.0f) - 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 10.0f) / 20.0f; })));

    // Output gain - display as -10 to +10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "outputGain", "Output Gain", juce::NormalisableRange<float>(-24.0f, 24.0f), 0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value / 24.0f) * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() / 10.0f) * 24.0f; })));

    // Bypass switches
    layout.add(std::make_unique<juce::AudioParameterBool>("masterBypass", "Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("saturationBypass", "Saturation Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("eqBypass", "EQ Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("compressorBypass", "Compressor Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("inputMeasured", "Input Measured", true));

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
const juce::String NineStripProcessor::getProgramName(int /*index*/) { return {}; }
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

    dcBlocker.prepare(sampleRate);

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

void NineStripProcessor::releaseResources() { dcBlocker.reset(); }

bool NineStripProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    // Only support stereo
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
}

template <typename SampleType>
void NineStripProcessor::processBlockInternal(juce::AudioBuffer<SampleType> &buffer)
{
    juce::ScopedNoDenormals noDenormals;

    // Master bypass - skip all processing
    if (apvts.getRawParameterValue("masterBypass")->load() > 0.5f)
    {
        if (getActiveEditor() != nullptr)
        {
            measuredLevelL.store(-60.0f);
            measuredLevelR.store(-60.0f);
        }

        return;  // Early exit, pass audio through untouched
    }

    // Get gain values
    float inputGainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("inputGain")->load());
    float outputGainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("outputGain")->load());

    // Apply input gain
    buffer.applyGain(inputGainLinear);

    if (getActiveEditor() != nullptr && !isNonRealtime() && apvts.getRawParameterValue("inputMeasured")->load() > 0.5f)
    {
        meterUpdateCounter++;

        if (meterUpdateCounter >= 4)
        {
            auto inL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
            auto inR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
            inL = std::max<SampleType>(inL, 0.000001);
            inR = std::max<SampleType>(inR, 0.000001);
            measuredLevelL.store(juce::Decibels::gainToDecibels(static_cast<float>(inL), -60.0f));
            measuredLevelR.store(juce::Decibels::gainToDecibels(static_cast<float>(inR), -60.0f));

            meterUpdateCounter = 0;
        }
    }

    // Create raw pointer arrays for Airwindows processing
    SampleType *channelData[2] = {buffer.getWritePointer(0), buffer.getWritePointer(1)};

    // Process through the plugin chain
    if (apvts.getRawParameterValue("saturationBypass")->load() < 0.5f)
    {
        if constexpr (std::is_same_v<SampleType, float>)
            channel9.processReplacing(channelData, channelData, buffer.getNumSamples());
        else
            channel9.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
    }

    if (apvts.getRawParameterValue("eqBypass")->load() < 0.5f)
    {
        if constexpr (std::is_same_v<SampleType, float>)
        {
            highpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
            lowpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
            baxandall2.processReplacing(channelData, channelData, buffer.getNumSamples());
            parametric.processReplacing(channelData, channelData, buffer.getNumSamples());
        }
        else
        {
            highpass2.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
            lowpass2.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
            baxandall2.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
            parametric.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
        }

        dcBlocker.processStereo(channelData, buffer.getNumSamples());
    }

    if (apvts.getRawParameterValue("compressorBypass")->load() < 0.5f)
    {
        if constexpr (std::is_same_v<SampleType, float>)
        {
            pressure4.processReplacing(channelData, channelData, buffer.getNumSamples());
        }
        else
        {
            pressure4.processDoubleReplacing(channelData, channelData, buffer.getNumSamples());
        }
        if (getActiveEditor() != nullptr)
        {
            float grDb = juce::Decibels::gainToDecibels(pressure4.getGainReduction());
            gainReduction.store(std::min(grDb, 0.0f));  // Negative values for reduction
        }
    }
    else if (getActiveEditor() != nullptr)
    {
        gainReduction.store(0.0f);
    }

    // Apply output gain
    buffer.applyGain(outputGainLinear);

    if (getActiveEditor() != nullptr && !isNonRealtime() && apvts.getRawParameterValue("inputMeasured")->load() <= 0.5f)
    {
        meterUpdateCounter++;

        if (meterUpdateCounter >= 4)
        {
            auto outL = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
            auto outR = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
            outL = std::max<SampleType>(outL, 0.000001);
            outR = std::max<SampleType>(outR, 0.000001);
            measuredLevelL.store(juce::Decibels::gainToDecibels(static_cast<float>(outL), -60.0f));
            measuredLevelR.store(juce::Decibels::gainToDecibels(static_cast<float>(outR), -60.0f));

            meterUpdateCounter = 0;
        }
    }
}

void NineStripProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &) { processBlockInternal(buffer); }
void NineStripProcessor::processBlock(juce::AudioBuffer<double> &buffer, juce::MidiBuffer &) { processBlockInternal(buffer); }

//==============================================================================
bool NineStripProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *NineStripProcessor::createEditor() { return new NineStripProcessorEditor(*this); }

//==============================================================================
void NineStripProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = apvts.copyState();

    // Add current preset name to the state
    auto presetName = presetManager->getCurrentPreset();
    if (!presetName.isEmpty()) state.setProperty("currentPreset", presetName, nullptr);

    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NineStripProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr)
    {
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            auto valueTree = juce::ValueTree::fromXml(*xmlState);
            apvts.replaceState(valueTree);

            // Restore preset name
            if (valueTree.hasProperty("currentPreset"))
            {
                auto presetName = valueTree.getProperty("currentPreset").toString();
                presetManager->loadPreset(presetName);

                // Notify editor to update UI
                if (auto *editor = dynamic_cast<NineStripProcessorEditor *>(getActiveEditor())) editor->updatePresetComboBox();
            }
        }
    }
}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new NineStripProcessor(); }
