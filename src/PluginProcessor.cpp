#include "PluginProcessor.h"

#include <array>

#include "PluginEditor.h"
#include "PurestGain.h"

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
      pressure4(44100.0),
      inputPurestGain(44100.0),
      outputPurestGain(44100.0)
{
    static const juce::String APP_FOLDER_NAME =
#if JUCE_LINUX
        ".config/NineStrip";
#else
        "NineStrip";
#endif

    // Then use it in both places:
    PropertiesFile::Options options;
    options.applicationName = "NineStrip";
    options.filenameSuffix = ".settings";
    options.folderName = APP_FOLDER_NAME;
    options.osxLibrarySubFolder = "Application Support";
    appProperties.setStorageParameters(options);

    ballisticsFilter.prepare({44100, static_cast<juce::uint32>(512), 2});
    ballisticsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::RMS);
    ballisticsFilter.setAttackTime(ballisticsFilterAttackTime);
    ballisticsFilter.setReleaseTime(ballisticsFilterReleaseTime);

    grBallisticsFilter.prepare({44100, static_cast<juce::uint32>(512), 1});
    grBallisticsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::peak);
    grBallisticsFilter.setAttackTime(grBallisticsFilterAttackTime);
    grBallisticsFilter.setReleaseTime(grBallisticsFilterReleaseTime);

    presetManager = std::make_unique<PresetManager>(apvts);

    apvts.state.addListener(this);
    setupParameterListeners();
}

NineStripProcessor::~NineStripProcessor()
{
    juce::Logger::setCurrentLogger(nullptr);

    apvts.state.removeListener(this);
    removeParameterListeners();
}

void NineStripProcessor::setupParameterListeners()
{
    for (const auto &id : parameterIDs) apvts.addParameterListener(id, this);
}

void NineStripProcessor::removeParameterListeners()
{
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

    // PurestGain
    else if (parameterID == "inputGain")
        inputPurestGain.setParameter(PurestGain::kParamA, newValue);
    else if (parameterID == "outputGain")
        outputPurestGain.setParameter(PurestGain::kParamA, newValue);
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

    // Input gain - store as 0-1, display as -40 to +40 dB
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "inputGain", "Input Gain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 80.0f) - 40.0f, 1) + " dB"; })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 40.0f) / 80.0f; })));

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

    // Input gain - store as 0-1, display as -40 to +40 dB
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "outputGain", "Output Gain", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String((value * 80.0f) - 40.0f, 1) + " dB"; })
            .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 40.0f) / 80.0f; })));

    // Bypass switches
    layout.add(std::make_unique<juce::AudioParameterBool>("masterBypass", "Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("saturationBypass", "Saturation Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("filterBypass", "Filter Bypass", false));
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
void NineStripProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    channel9.setSampleRate(sampleRate);
    highpass2.setSampleRate(sampleRate);
    lowpass2.setSampleRate(sampleRate);
    baxandall2.setSampleRate(sampleRate);
    parametric.setSampleRate(sampleRate);
    pressure4.setSampleRate(sampleRate);
    inputPurestGain.setSampleRate(sampleRate);
    outputPurestGain.setSampleRate(sampleRate);

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

    inputPurestGain.setParameter(PurestGain::kParamB, 1.0f);   // chasespeed
    outputPurestGain.setParameter(PurestGain::kParamB, 1.0f);  // chasespeed

    inputPurestGain.setParameter(PurestGain::kParamA, apvts.getRawParameterValue("inputGain")->load());

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

    outputPurestGain.setParameter(PurestGain::kParamA, apvts.getRawParameterValue("outputGain")->load());

    ballisticsFilter.prepare({sampleRate, static_cast<juce::uint32>(samplesPerBlock), 2});
    ballisticsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::RMS);
    ballisticsFilter.setAttackTime(ballisticsFilterAttackTime);
    ballisticsFilter.setReleaseTime(ballisticsFilterReleaseTime);
    meterBufferFloat.setSize(2, samplesPerBlock, false, false, true);
    meterBufferDouble.setSize(2, samplesPerBlock, false, false, true);
    emptyMeterBufferFloat.setSize(2, samplesPerBlock, false, false, true);
    emptyMeterBufferDouble.setSize(2, samplesPerBlock, false, false, true);

    grBallisticsFilter.prepare({sampleRate, static_cast<juce::uint32>(samplesPerBlock), 1});
    grBallisticsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::peak);
    grBallisticsFilter.setAttackTime(grBallisticsFilterAttackTime);
    grBallisticsFilter.setReleaseTime(grBallisticsFilterReleaseTime);
    grMeterBufferFloat.setSize(1, samplesPerBlock, false, false, true);
    grMeterBufferDouble.setSize(1, samplesPerBlock, false, false, true);
}

void NineStripProcessor::releaseResources()
{
    dcBlocker.reset();
    ballisticsFilter.reset();
    grBallisticsFilter.reset();
}

bool NineStripProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    // Only support stereo
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
}

template <typename SampleType>
void NineStripProcessor::updateMeters(const juce::AudioBuffer<SampleType> &buffer)
{
    // Make a COPY - don't modify the original audio!
    // Use pre-allocated buffer
    auto &meterBuffer = [&]() -> juce::AudioBuffer<SampleType> &
    {
        if constexpr (std::is_same_v<SampleType, float>)
            return meterBufferFloat;
        else
            return meterBufferDouble;
    }();

    meterBuffer.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
    meterBuffer.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());

    // Process the copy through the filter
    juce::dsp::AudioBlock<SampleType> block(meterBuffer);
    juce::dsp::ProcessContextReplacing<SampleType> context(block);

    ballisticsFilter.process(context);

    // Get the resulting envelope level from the last sample
    float levelL = meterBuffer.getSample(0, meterBuffer.getNumSamples() - 1);
    float levelR = meterBuffer.getSample(1, meterBuffer.getNumSamples() - 1);

    float dbfsL = juce::Decibels::gainToDecibels(levelL, -60.0f);
    float dbfsR = juce::Decibels::gainToDecibels(levelR, -60.0f);

    measuredLevelL.store(dbfsL);
    measuredLevelR.store(dbfsR);
}

template <typename SampleType>
void NineStripProcessor::updateGRMeter(float coefficientGain, int numSamples)
{
    // Get the appropriate buffer
    auto &meterBuffer = [&]() -> juce::AudioBuffer<SampleType> &
    {
        if constexpr (std::is_same_v<SampleType, float>)
            return grMeterBufferFloat;
        else
            return grMeterBufferDouble;
    }();

    // Clamp coefficient to valid range
    coefficientGain = std::clamp(coefficientGain, 0.0001f, 1.0f);

    // Fill buffer with coefficient (linear domain, not dB)
    for (int i = 0; i < numSamples; ++i) meterBuffer.setSample(0, i, coefficientGain);

    // Apply ballistics filter (same as VU meter approach)
    juce::dsp::AudioBlock<SampleType> block(meterBuffer);           // ← Changed to SampleType
    juce::dsp::ProcessContextReplacing<SampleType> context(block);  // ← Changed to SampleType
    grBallisticsFilter.process(context);

    // Get smoothed coefficient and convert to dB
    float smoothedCoeff = meterBuffer.getSample(0, numSamples - 1);
    float grDB = NAN;
    if (smoothedCoeff >= 0.999f)  // No compression
    {
        grDB = 0.0f;
    }
    else
    {
        grDB = juce::Decibels::gainToDecibels(smoothedCoeff);
    }

    // Clamp to 0 dB max
    gainReduction.store(std::min(grDB, 0.0f));
}

template <typename SampleType>
void NineStripProcessor::processBlockInternal(juce::AudioBuffer<SampleType> &buffer)
{
    juce::ScopedNoDenormals noDenormals;

    const bool masterBypass = apvts.getRawParameterValue("masterBypass")->load() > 0.5f;
    const bool meteringNeeded = editorOpen.load() && !isNonRealtime();

    // Master bypass - skip all processing
    if (masterBypass)
    {
        if (meteringNeeded)
        {
            // Get reference to appropriate buffer (already sized)
            auto &emptyMeterBuffer = [&]() -> juce::AudioBuffer<SampleType> &
            {
                if constexpr (std::is_same_v<SampleType, float>)
                    return emptyMeterBufferFloat;
                else
                    return emptyMeterBufferDouble;
            }();

            emptyMeterBuffer.clear();
            updateMeters(emptyMeterBuffer);
        }

        return;  // Early exit, pass audio through untouched
    }

    const bool saturationBypass = apvts.getRawParameterValue("saturationBypass")->load() > 0.5f;
    const bool filterBypass = apvts.getRawParameterValue("filterBypass")->load() > 0.5f;
    const bool eqBypass = apvts.getRawParameterValue("eqBypass")->load() > 0.5f;
    const bool compressorBypass = apvts.getRawParameterValue("compressorBypass")->load() > 0.5f;

    const bool inputMeasured = apvts.getRawParameterValue("inputMeasured")->load() > 0.5f;
    const bool inputMeteringNeeded = meteringNeeded && inputMeasured;
    const bool outputMeteringNeeded = meteringNeeded && !inputMeasured;

    // Create raw pointer arrays for Airwindows processing
    std::array<SampleType *, 2> channelData = {buffer.getWritePointer(0), buffer.getWritePointer(1)};

    if constexpr (std::is_same_v<SampleType, float>)
        inputPurestGain.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
    else
        inputPurestGain.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());

    if (inputMeteringNeeded)
    {
        updateMeters(buffer);
    }

    // Process through the plugin chain
    if (!saturationBypass)
    {
        if constexpr (std::is_same_v<SampleType, float>)
            channel9.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        else
            channel9.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
    }

    if (!filterBypass)
    {
        if constexpr (std::is_same_v<SampleType, float>)
        {
            highpass2.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
            lowpass2.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        }
        else
        {
            highpass2.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
            lowpass2.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        }
    }

    if (!eqBypass)
    {
        if constexpr (std::is_same_v<SampleType, float>)
        {
            baxandall2.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
            parametric.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        }
        else
        {
            baxandall2.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
            parametric.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        }
    }

    if (!filterBypass)
    {
        dcBlocker.processStereo(channelData.data(), buffer.getNumSamples());
    }

    if (!compressorBypass)
    {
        // Reset GR tracking for this block
        pressure4.resetGRTracking();

        if constexpr (std::is_same_v<SampleType, float>)
        {
            pressure4.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        }
        else
        {
            pressure4.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
        }

        pressure4.finalizeGR();

        if (meteringNeeded)
        {
            float coefficient = pressure4.getGainReduction();
            updateGRMeter<SampleType>(coefficient, buffer.getNumSamples());
        }
    }
    else if (meteringNeeded)
    {
        updateGRMeter<SampleType>(1.0f, buffer.getNumSamples());
    }

    if constexpr (std::is_same_v<SampleType, float>)
        outputPurestGain.processReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());
    else
        outputPurestGain.processDoubleReplacing(channelData.data(), channelData.data(), buffer.getNumSamples());

    if (outputMeteringNeeded)
    {
        updateMeters(buffer);
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
