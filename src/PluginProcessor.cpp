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
      pressure4(44100.0) {}

NineStripProcessor::~NineStripProcessor() {}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NineStripProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Input gain
    layout.add(std::make_unique<juce::AudioParameterFloat>("inputGain", "Input Gain", -12.0f, 12.0f, 0.0f));

    // Channel9
    layout.add(std::make_unique<juce::AudioParameterChoice>("consoleType", "Console Type",
                                                            juce::StringArray{"Neve", "API", "SSL", "Teac", "Mackie"}, 0));

    layout.add(std::make_unique<juce::AudioParameterFloat>("drive", "Drive", 0.0f, 2.0f, 0.0f));

    // Lowpass2
    layout.add(std::make_unique<juce::AudioParameterFloat>("lowpass", "Lowpass", 0.01f, 1.0f, 1.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lp_sft_hrd", "LP Soft/Hard", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lp_poles", "LP Poles", 0.0f, 1.0f, 0.25f));

    // Highpass2
    layout.add(std::make_unique<juce::AudioParameterFloat>("hipass", "Hipass", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("ls_tite", "Ls/Tite", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("hp_poles", "HP Poles", 0.0f, 1.0f, 0.25f));

    // Baxandall2
    layout.add(std::make_unique<juce::AudioParameterFloat>("treble", "Treble", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("bass", "Bass", 0.0f, 1.0f, 0.5f));

    // Parametric (all 9 parameters)
    layout.add(std::make_unique<juce::AudioParameterFloat>("tr_freq", "Tr Freq", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("treble_param", "Treble", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("tr_reso", "Tr Reso", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("hm_freq", "HM Freq", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("highmid", "HighMid", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("hm_reso", "HM Reso", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lm_freq", "LM Freq", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lowmid", "LowMid", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("lm_reso", "LM Reso", 0.0f, 1.0f, 0.5f));

    // Pressure4
    layout.add(std::make_unique<juce::AudioParameterFloat>("pressure", "Pressure", 0.0f, 1.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("speed", "Speed", 0.0f, 1.0f, 0.2f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mewiness", "Mewiness", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("output_gain", "Output Gain", 0.0f, 1.0f, 1.0f));

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
void NineStripProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    channel9.setSampleRate(sampleRate);
    highpass2.setSampleRate(sampleRate);
    lowpass2.setSampleRate(sampleRate);
    baxandall2.setSampleRate(sampleRate);
    parametric.setSampleRate(sampleRate);
    pressure4.setSampleRate(sampleRate);
}

void NineStripProcessor::releaseResources() {}

bool NineStripProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const {
    // Only support stereo
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo() &&
           layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo();
}
void NineStripProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages) {
    juce::ScopedNoDenormals noDenormals;

    // Apply input gain
    float gainLinear = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("inputGain")->load());

    // Set Channel9 parameters
    channel9.setParameter(Channel9::kParamA, apvts.getRawParameterValue("consoleType")->load());
    channel9.setParameter(Channel9::kParamB, apvts.getRawParameterValue("drive")->load() / 2.0f);
    channel9.setParameter(Channel9::kParamC, 1.0f);

    // Set Highpass2 parameters
    highpass2.setParameter(Highpass2::kParamA, apvts.getRawParameterValue("hipass")->load());
    highpass2.setParameter(Highpass2::kParamB, apvts.getRawParameterValue("ls_tite")->load());
    highpass2.setParameter(Highpass2::kParamC, apvts.getRawParameterValue("hp_poles")->load());
    highpass2.setParameter(Highpass2::kParamD, 1.0f);

    // Set Lowpass2 parameters
    lowpass2.setParameter(Lowpass2::kParamA, apvts.getRawParameterValue("lowpass")->load());
    lowpass2.setParameter(Lowpass2::kParamB, apvts.getRawParameterValue("lp_sft_hrd")->load());
    lowpass2.setParameter(Lowpass2::kParamC, apvts.getRawParameterValue("lp_poles")->load());
    lowpass2.setParameter(Lowpass2::kParamD, 1.0f);

    // Set Baxandall2 parameters
    baxandall2.setParameter(Baxandall2::kParamA, apvts.getRawParameterValue("treble")->load());
    baxandall2.setParameter(Baxandall2::kParamB, apvts.getRawParameterValue("bass")->load());

    // Set Parametric parameters
    parametric.setParameter(Parametric::kParamA, apvts.getRawParameterValue("tr_freq")->load());
    parametric.setParameter(Parametric::kParamB, apvts.getRawParameterValue("treble_param")->load());
    parametric.setParameter(Parametric::kParamC, apvts.getRawParameterValue("tr_reso")->load());
    parametric.setParameter(Parametric::kParamD, apvts.getRawParameterValue("hm_freq")->load());
    parametric.setParameter(Parametric::kParamE, apvts.getRawParameterValue("highmid")->load());
    parametric.setParameter(Parametric::kParamF, apvts.getRawParameterValue("hm_reso")->load());
    parametric.setParameter(Parametric::kParamG, apvts.getRawParameterValue("lm_freq")->load());
    parametric.setParameter(Parametric::kParamH, apvts.getRawParameterValue("lowmid")->load());
    parametric.setParameter(Parametric::kParamI, apvts.getRawParameterValue("lm_reso")->load());
    parametric.setParameter(Parametric::kParamJ, 1.0f);

    // Set Pressure4 parameters
    pressure4.setParameter(Pressure4::kParamA, apvts.getRawParameterValue("pressure")->load());
    pressure4.setParameter(Pressure4::kParamB, apvts.getRawParameterValue("speed")->load());
    pressure4.setParameter(Pressure4::kParamC, apvts.getRawParameterValue("mewiness")->load());
    pressure4.setParameter(Pressure4::kParamD, apvts.getRawParameterValue("output_gain")->load());

    buffer.applyGain(gainLinear);

    // Create raw pointer arrays for Airwindows processing
    float *channelData[2] = {
        buffer.getWritePointer(0),  // Left
        buffer.getWritePointer(1)   // Right
    };

    // Process through the plugin chain
    channel9.processReplacing(channelData, channelData, buffer.getNumSamples());
    highpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
    lowpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
    baxandall2.processReplacing(channelData, channelData, buffer.getNumSamples());
    parametric.processReplacing(channelData, channelData, buffer.getNumSamples());
    pressure4.processReplacing(channelData, channelData, buffer.getNumSamples());
}

//==============================================================================
bool NineStripProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *NineStripProcessor::createEditor() { return new NineStripProcessorEditor(*this); }

//==============================================================================
void NineStripProcessor::getStateInformation(juce::MemoryBlock &destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NineStripProcessor::setStateInformation(const void *data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new NineStripProcessor(); }
