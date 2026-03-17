#include "PluginProcessor.h"

#include "Capacitor2.h"
#include "PluginEditor.h"
#include "PurestGain.h"

NineStripProcessor::NineStripProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      channel9(44100.0),
      capacitor2(44100.0),
      baxandall2(44100.0),
      parametric(44100.0),
      pressure4(44100.0),
      interstage(44100.0),
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

    presetManager = std::make_unique<PresetManager>(apvts);

    apvts.state.addListener(this);
    setupParameterListeners();
}

NineStripProcessor::~NineStripProcessor()
{
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

    // Capacitor2
    else if (parameterID == "lowpass")
        capacitor2.setParameter(Capacitor2::kParamA, newValue);
    else if (parameterID == "hipass")
        capacitor2.setParameter(Capacitor2::kParamB, newValue);
    else if (parameterID == "non_lin")
        capacitor2.setParameter(Capacitor2::kParamC, newValue);

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

    // Lowpass - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "lowpass", "Lowpass", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // Highpass - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "hipass", "Hipass", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction([](float value, int) { return juce::String(value * 10.0f, 1); })
            .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 10.0f; })));

    // Non Linear - display as 0-10
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "non_lin", "Non Linear", juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f,
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
        "mewiness", "Mewiness", juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f,
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
    layout.add(std::make_unique<juce::AudioParameterBool>("masterBypass", "Master Bypass", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("saturationInput", "Saturation Input", true));
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
const juce::String NineStripProcessor::getProgramName(int /*index*/) { return "Default"; }
void NineStripProcessor::changeProgramName(int index, const juce::String &newName) {}

//==============================================================================
void NineStripProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    channel9.setSampleRate(sampleRate);
    capacitor2.setSampleRate(sampleRate);
    baxandall2.setSampleRate(sampleRate);
    parametric.setSampleRate(sampleRate);
    pressure4.setSampleRate(sampleRate);
    interstage.setSampleRate(sampleRate);
    inputPurestGain.setSampleRate(sampleRate);
    outputPurestGain.setSampleRate(sampleRate);

    dcBlocker.prepare(sampleRate);

    channel9.setParameter(Channel9::kParamC, 1.0f);      // output gain
    capacitor2.setParameter(Capacitor2::kParamD, 1.0f);  // wet/dry

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

    capacitor2.setParameter(Capacitor2::kParamA, apvts.getRawParameterValue("lowpass")->load());
    capacitor2.setParameter(Capacitor2::kParamB, apvts.getRawParameterValue("hipass")->load());
    capacitor2.setParameter(Capacitor2::kParamC, apvts.getRawParameterValue("non_lin")->load());

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

    paramMasterBypass = apvts.getRawParameterValue("masterBypass");
    paramSaturationInput = apvts.getRawParameterValue("saturationInput");
    paramSatBypass = apvts.getRawParameterValue("saturationBypass");
    paramFilterBypass = apvts.getRawParameterValue("filterBypass");
    paramEqBypass = apvts.getRawParameterValue("eqBypass");
    paramCompBypass = apvts.getRawParameterValue("compressorBypass");
    paramInputMeasured = apvts.getRawParameterValue("inputMeasured");

    emptyMeterBufferFloat.setSize(2, samplesPerBlock, false, false, true);
    emptyMeterBufferDouble.setSize(2, samplesPerBlock, false, false, true);
}

void NineStripProcessor::releaseResources() { dcBlocker.reset(); }

bool NineStripProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
    // Only support stereo
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
}

template <typename SampleType>
void NineStripProcessor::updateMeters(const juce::AudioBuffer<SampleType> &buffer, int numSamples)
{
    // Use JUCE's SIMD-optimized RMS calculation (FloatVectorOperations)
    const float rmsL = buffer.getRMSLevel(0, 0, numSamples);
    const float rmsR = buffer.getRMSLevel(buffer.getNumChannels() > 1 ? 1 : 0, 0, numSamples);

    constexpr float kMinRms = 1e-6f;
    constexpr float kFloor = -60.0f;
    measuredLevelL.store(rmsL > kMinRms ? 20.0f * std::log10(rmsL) : kFloor, std::memory_order_relaxed);
    measuredLevelR.store(rmsR > kMinRms ? 20.0f * std::log10(rmsR) : kFloor, std::memory_order_relaxed);
}

template void NineStripProcessor::updateMeters<float>(const juce::AudioBuffer<float> &, int);
template void NineStripProcessor::updateMeters<double>(const juce::AudioBuffer<double> &, int);

void NineStripProcessor::updateGRMeter(const float gainReductionLinear)
{
    const float grDb = (gainReductionLinear > 1e-6f) ? 20.0f * std::log10(gainReductionLinear) : -60.0f;
    gainReduction.store(grDb, std::memory_order_relaxed);
}

template <typename SampleType>
void NineStripProcessor::processBlockInternal(juce::AudioBuffer<SampleType> &buffer)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const bool masterBypass = paramMasterBypass->load(std::memory_order_relaxed) > 0.5f;
    const bool meteringNeeded = editorOpen.load(std::memory_order_relaxed) && !isNonRealtime();

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
            updateMeters(emptyMeterBuffer, numSamples);

            updateGRMeter(1.0f);  // No gain reduction when bypassed
        }

        return;  // Early exit, pass audio through untouched
    }

    const bool saturationInput = paramSaturationInput->load(std::memory_order_relaxed) > 0.5f;
    const bool saturationBypass = paramSatBypass->load(std::memory_order_relaxed) > 0.5f;
    const bool filterBypass = paramFilterBypass->load(std::memory_order_relaxed) > 0.5f;
    const bool eqBypass = paramEqBypass->load(std::memory_order_relaxed) > 0.5f;
    const bool compressorBypass = paramCompBypass->load(std::memory_order_relaxed) > 0.5f;

    const bool inputMeasured = paramInputMeasured->load(std::memory_order_relaxed) > 0.5f;
    const bool inputMeteringNeeded = meteringNeeded && inputMeasured;
    const bool outputMeteringNeeded = meteringNeeded && !inputMeasured;

    // Create raw pointer arrays for Airwindows processing
    SampleType *channels[2] = {buffer.getWritePointer(0), buffer.getWritePointer(1)};

    if constexpr (std::is_same_v<SampleType, float>)
        inputPurestGain.processReplacing(channels, channels, numSamples);
    else
        inputPurestGain.processDoubleReplacing(channels, channels, numSamples);

    if (inputMeteringNeeded) updateMeters(buffer, numSamples);

    if constexpr (std::is_same_v<SampleType, float>)
        interstage.processReplacing(channels, channels, numSamples);
    else
        interstage.processDoubleReplacing(channels, channels, numSamples);

    // Process through the plugin chain
    if (!saturationBypass && saturationInput)
    {
        if constexpr (std::is_same_v<SampleType, float>)
            channel9.processReplacing(channels, channels, numSamples);
        else
            channel9.processDoubleReplacing(channels, channels, numSamples);
    }

    if (!filterBypass)
    {
        if constexpr (std::is_same_v<SampleType, float>)
        {
            capacitor2.processReplacing(channels, channels, numSamples);
        }
        else
        {
            capacitor2.processDoubleReplacing(channels, channels, numSamples);
        }
    }

    if (!eqBypass)
    {
        if constexpr (std::is_same_v<SampleType, float>)
        {
            baxandall2.processReplacing(channels, channels, numSamples);
            parametric.processReplacing(channels, channels, numSamples);
        }
        else
        {
            baxandall2.processDoubleReplacing(channels, channels, numSamples);
            parametric.processDoubleReplacing(channels, channels, numSamples);
        }
    }

    if (!filterBypass) dcBlocker.processStereo(channels, numSamples);

    if (!compressorBypass)
    {
        if constexpr (std::is_same_v<SampleType, float>)
            pressure4.processReplacing(channels, channels, numSamples);
        else
            pressure4.processDoubleReplacing(channels, channels, numSamples);

        if (meteringNeeded)
        {
            updateGRMeter(pressure4.getGainReductionLinear());
        }
    }
    else if (meteringNeeded)
    {
        updateGRMeter(1.0f);
    }

    if (!saturationBypass && !saturationInput)
    {
        if constexpr (std::is_same_v<SampleType, float>)
            channel9.processReplacing(channels, channels, numSamples);
        else
            channel9.processDoubleReplacing(channels, channels, numSamples);
    }

    if constexpr (std::is_same_v<SampleType, float>)
        outputPurestGain.processReplacing(channels, channels, numSamples);
    else
        outputPurestGain.processDoubleReplacing(channels, channels, numSamples);

    if (outputMeteringNeeded) updateMeters(buffer, numSamples);
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
