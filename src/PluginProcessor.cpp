#include "PluginProcessor.h"

#include "Capacitor2.h"
#include "PluginEditor.h"
#include "PurestGain.h"

NineStripProcessor::NineStripProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()),
      channel9Pre(44100.0),
      channel9Post(44100.0),
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
    {
        channel9Pre.setParameter(Channel9::kParamA, newValue);
        channel9Post.setParameter(Channel9::kParamA, newValue);
    }
    else if (parameterID == "drive")
    {
        channel9Pre.setParameter(Channel9::kParamB, newValue);
        channel9Post.setParameter(Channel9::kParamB, newValue);
    }

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
    channel9Pre.setSampleRate(sampleRate);
    channel9Post.setSampleRate(sampleRate);
    capacitor2.setSampleRate(sampleRate);
    baxandall2.setSampleRate(sampleRate);
    parametric.setSampleRate(sampleRate);
    pressure4.setSampleRate(sampleRate);
    interstage.setSampleRate(sampleRate);
    inputPurestGain.setSampleRate(sampleRate);
    outputPurestGain.setSampleRate(sampleRate);

    dcBlocker.prepare(sampleRate);

    channel9Pre.setParameter(Channel9::kParamC, 1.0f);   // output gain
    channel9Post.setParameter(Channel9::kParamC, 1.0f);  // output gain
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

    channel9Pre.setParameter(Channel9::kParamA, apvts.getRawParameterValue("consoleType")->load());
    channel9Pre.setParameter(Channel9::kParamB, apvts.getRawParameterValue("drive")->load());
    channel9Post.setParameter(Channel9::kParamA, apvts.getRawParameterValue("consoleType")->load());
    channel9Post.setParameter(Channel9::kParamB, apvts.getRawParameterValue("drive")->load());

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

    stageScratchFloat.setSize(2, samplesPerBlock, false, false, true);
    stageScratchDouble.setSize(2, samplesPerBlock, false, false, true);
    masterDryFloat.setSize(2, samplesPerBlock, false, false, true);
    masterDryDouble.setSize(2, samplesPerBlock, false, false, true);

    // Bypass crossfades: start settled at the current switch positions so there is no fade-in on transport start.
    const bool masterBypass = paramMasterBypass->load() > 0.5f;
    const bool saturationInput = paramSaturationInput->load() > 0.5f;
    const bool saturationBypass = paramSatBypass->load() > 0.5f;
    const bool filterBypass = paramFilterBypass->load() > 0.5f;
    const bool eqBypass = paramEqBypass->load() > 0.5f;
    const bool compressorBypass = paramCompBypass->load() > 0.5f;

    for (auto *mix : {&masterMix, &satPreMix, &satPostMix, &filterMix, &dcMix, &eqMix, &compMix})
        mix->reset(sampleRate, kBypassRampSeconds);

    masterMix.setCurrentAndTargetValue(masterBypass ? 0.0f : 1.0f);
    satPreMix.setCurrentAndTargetValue(!saturationBypass && saturationInput ? 1.0f : 0.0f);
    satPostMix.setCurrentAndTargetValue(!saturationBypass && !saturationInput ? 1.0f : 0.0f);
    filterMix.setCurrentAndTargetValue(filterBypass ? 0.0f : 1.0f);
    dcMix.setCurrentAndTargetValue(filterBypass ? 0.0f : 1.0f);
    eqMix.setCurrentAndTargetValue(eqBypass ? 0.0f : 1.0f);
    compMix.setCurrentAndTargetValue(compressorBypass ? 0.0f : 1.0f);
}

void NineStripProcessor::releaseResources() { dcBlocker.reset(); }

void NineStripProcessor::resetCapacitor2State()
{
    // Reset IIR state in-place without touching Airwindows source.
    // Capacitor2's destructor is a no-op and all members are POD/value types,
    // so placement-new is safe here. This runs only when NaN is already present
    // in the output (i.e. audio was already broken), never during normal playback.
    const double sr = getSampleRate() > 0.0 ? getSampleRate() : 44100.0;
    capacitor2.~Capacitor2();
    new (&capacitor2) Capacitor2(sr);
    capacitor2.setParameter(Capacitor2::kParamD, 1.0f);
    capacitor2.setParameter(Capacitor2::kParamA, apvts.getRawParameterValue("lowpass")->load());
    capacitor2.setParameter(Capacitor2::kParamB, apvts.getRawParameterValue("hipass")->load());
    capacitor2.setParameter(Capacitor2::kParamC, apvts.getRawParameterValue("non_lin")->load());
}

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

namespace
{
template <typename SampleType>
void copyStereo(SampleType **dst, SampleType **src, int numSamples)
{
    juce::FloatVectorOperations::copy(dst[0], src[0], numSamples);
    juce::FloatVectorOperations::copy(dst[1], src[1], numSamples);
}

template <typename SampleType>
void crossfadeStereo(SampleType **live, SampleType **dry, juce::LinearSmoothedValue<float> &mix, int numSamples)
{
    // live holds the wet signal on entry; blend it against dry with the per-sample ramp
    for (int i = 0; i < numSamples; ++i)
    {
        const auto m = static_cast<SampleType>(mix.getNextValue());
        live[0][i] = dry[0][i] + (live[0][i] - dry[0][i]) * m;
        live[1][i] = dry[1][i] + (live[1][i] - dry[1][i]) * m;
    }
}

// Runs one switchable stage with a click-free bypass.
//   settled active   : process live in place (same cost as a hard switch)
//   settled bypassed : process a copy and discard it, so the stage's state stays warm and re-enabling is transient-free
//   ramping          : process live in place, then crossfade against the dry copy
// If scratch is null (host delivered a block larger than prepared) the stage falls back to a hard switch.
template <typename SampleType, typename Process>
void processCrossfadedStage(juce::LinearSmoothedValue<float> &mix, bool active, SampleType **live, SampleType **scratch,
                            int numSamples, Process &&process)
{
    mix.setTargetValue(active ? 1.0f : 0.0f);

    if (scratch == nullptr)
    {
        mix.setCurrentAndTargetValue(mix.getTargetValue());
        if (active) process(live);
        return;
    }

    if (!mix.isSmoothing())
    {
        if (active)
        {
            process(live);
        }
        else
        {
            copyStereo(scratch, live, numSamples);
            process(scratch);
        }
        return;
    }

    copyStereo(scratch, live, numSamples);
    process(live);
    crossfadeStereo(live, scratch, mix, numSamples);
}
}  // namespace

template <typename SampleType>
void NineStripProcessor::processBlockInternal(juce::AudioBuffer<SampleType> &buffer)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const bool masterBypass = paramMasterBypass->load(std::memory_order_relaxed) > 0.5f;
    const bool meteringNeeded = editorOpen.load(std::memory_order_relaxed) && !isNonRealtime();

    auto &stageScratchBuffer = [&]() -> juce::AudioBuffer<SampleType> &
    {
        if constexpr (std::is_same_v<SampleType, float>)
            return stageScratchFloat;
        else
            return stageScratchDouble;
    }();
    auto &masterDryBuffer = [&]() -> juce::AudioBuffer<SampleType> &
    {
        if constexpr (std::is_same_v<SampleType, float>)
            return masterDryFloat;
        else
            return masterDryDouble;
    }();

    // Scratch buffers are sized in prepareToPlay; a larger block than promised degrades to hard switching.
    const bool scratchAvailable = numSamples <= stageScratchBuffer.getNumSamples();
    SampleType *stageScratchPtrs[2] = {stageScratchBuffer.getWritePointer(0), stageScratchBuffer.getWritePointer(1)};
    SampleType *masterDry[2] = {masterDryBuffer.getWritePointer(0), masterDryBuffer.getWritePointer(1)};
    SampleType **scratch = scratchAvailable ? stageScratchPtrs : nullptr;

    masterMix.setTargetValue(masterBypass ? 0.0f : 1.0f);
    if (!scratchAvailable) masterMix.setCurrentAndTargetValue(masterMix.getTargetValue());
    const bool masterRamping = masterMix.isSmoothing();

    // Master bypass, once the crossfade has settled - skip all processing
    if (masterBypass && !masterRamping)
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

    if (masterRamping) copyStereo(masterDry, channels, numSamples);

    if constexpr (std::is_same_v<SampleType, float>)
        inputPurestGain.processReplacing(channels, channels, numSamples);
    else
        inputPurestGain.processDoubleReplacing(channels, channels, numSamples);

    if (inputMeteringNeeded) updateMeters(buffer, numSamples);

    if constexpr (std::is_same_v<SampleType, float>)
        interstage.processReplacing(channels, channels, numSamples);
    else
        interstage.processDoubleReplacing(channels, channels, numSamples);

    // Process through the plugin chain. Each switchable stage crossfades in/out over kBypassRampSeconds;
    // Pre/Post is two independent stages fading in opposite directions.
    processCrossfadedStage(satPreMix, !saturationBypass && saturationInput, channels, scratch, numSamples,
                           [&](SampleType **ch)
                           {
                               if constexpr (std::is_same_v<SampleType, float>)
                                   channel9Pre.processReplacing(ch, ch, numSamples);
                               else
                                   channel9Pre.processDoubleReplacing(ch, ch, numSamples);
                           });

    processCrossfadedStage(filterMix, !filterBypass, channels, scratch, numSamples,
                           [&](SampleType **ch)
                           {
                               if constexpr (std::is_same_v<SampleType, float>)
                                   capacitor2.processReplacing(ch, ch, numSamples);
                               else
                                   capacitor2.processDoubleReplacing(ch, ch, numSamples);

                               // Capacitor2's dielectric nonlinearity can make the IIR feedback coefficient
                               // go negative (non_lin≈1 + lowpass≈1 + negative-peak audio), causing permanent
                               // NaN in the filter state. Detect it, clear this block, and reset the state.
                               bool nanDetected = false;
                               for (int i = 0; i < numSamples && !nanDetected; ++i)
                                   nanDetected = !std::isfinite(ch[0][i]) || !std::isfinite(ch[1][i]);
                               if (nanDetected)
                               {
                                   resetCapacitor2State();
                                   for (int i = 0; i < numSamples; ++i) ch[0][i] = ch[1][i] = SampleType(0);
                               }
                           });

    processCrossfadedStage(eqMix, !eqBypass, channels, scratch, numSamples,
                           [&](SampleType **ch)
                           {
                               if constexpr (std::is_same_v<SampleType, float>)
                               {
                                   baxandall2.processReplacing(ch, ch, numSamples);
                                   parametric.processReplacing(ch, ch, numSamples);
                               }
                               else
                               {
                                   baxandall2.processDoubleReplacing(ch, ch, numSamples);
                                   parametric.processDoubleReplacing(ch, ch, numSamples);
                               }
                           });

    processCrossfadedStage(dcMix, !filterBypass, channels, scratch, numSamples,
                           [&](SampleType **ch) { dcBlocker.processStereo(ch, numSamples); });

    processCrossfadedStage(compMix, !compressorBypass, channels, scratch, numSamples,
                           [&](SampleType **ch)
                           {
                               if constexpr (std::is_same_v<SampleType, float>)
                                   pressure4.processReplacing(ch, ch, numSamples);
                               else
                                   pressure4.processDoubleReplacing(ch, ch, numSamples);
                           });

    if (meteringNeeded)
    {
        // The compressor keeps running while bypassed (to stay warm), but the meter should show no reduction then.
        const bool compAudible = !compressorBypass || compMix.isSmoothing();
        updateGRMeter(compAudible ? pressure4.getGainReductionLinear() : 1.0f);
    }

    processCrossfadedStage(satPostMix, !saturationBypass && !saturationInput, channels, scratch, numSamples,
                           [&](SampleType **ch)
                           {
                               if constexpr (std::is_same_v<SampleType, float>)
                                   channel9Post.processReplacing(ch, ch, numSamples);
                               else
                                   channel9Post.processDoubleReplacing(ch, ch, numSamples);
                           });

    if constexpr (std::is_same_v<SampleType, float>)
        outputPurestGain.processReplacing(channels, channels, numSamples);
    else
        outputPurestGain.processDoubleReplacing(channels, channels, numSamples);

    if (masterRamping) crossfadeStereo(channels, masterDry, masterMix, numSamples);

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

    // Add current preset name (and whether it has been tweaked) to the state
    auto presetName = presetManager->getCurrentPreset();
    if (!presetName.isEmpty())
    {
        state.setProperty("currentPreset", presetName, nullptr);
        state.setProperty("presetModified", presetManager->isPresetModified(), nullptr);
    }

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

            // Restore the preset name for display only. The parameter values just restored
            // from the host are authoritative; re-loading the preset file here would
            // overwrite any tweaks made after the preset was loaded.
            if (valueTree.hasProperty("currentPreset"))
            {
                auto presetName = valueTree.getProperty("currentPreset").toString();
                const bool modified = static_cast<bool>(valueTree.getProperty("presetModified", false));
                presetManager->setCurrentPreset(presetName, modified);

                // Notify editor to update UI
                if (auto *editor = dynamic_cast<NineStripProcessorEditor *>(getActiveEditor())) editor->updatePresetComboBox();
            }
        }
    }
}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new NineStripProcessor(); }
