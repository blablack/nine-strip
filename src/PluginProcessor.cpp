#include "PluginProcessor.h"

#include "PluginEditor.h"

NineStripProcessor::NineStripProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      channel9(44100.0),
      highpass2(44100.0),
      lowpass2(44100.0),
      baxandall2(44100.0),
      parametric(44100.0),
      pressure4(44100.0) {
    // Add parameters for Channel9
    addParameter(channel9_consoleType = new juce::AudioParameterChoice(
                     "consoleType", "Console Type", juce::StringArray{"Neve", "API", "SSL", "Teac", "Mackie"}, 0));

    addParameter(channel9_drive =
                     new juce::AudioParameterFloat("drive", "Drive", juce::NormalisableRange<float>(0.0f, 2.0f), 0.0f,
                                                   juce::AudioParameterFloatAttributes()
                                                       .withStringFromValueFunction([](float value, int) {
                                                           return juce::String(static_cast<int>(value * 100.0f)) + "%";
                                                       })
                                                       .withValueFromStringFunction([](const juce::String &text) {
                                                           return text.dropLastCharacters(1).getFloatValue() / 100.0f;
                                                       })));

    addParameter(channel9_output = new juce::AudioParameterFloat("output", "Output", 0.0f, 1.0f, 1.0f));

    // Lowpass2 parameters
    addParameter(lowpass2_lowpass = new juce::AudioParameterFloat("lowpass", "Lowpass", 0.01f, 1.0f, 1.0f));
    addParameter(
        lowpass2_sft_hrd = new juce::AudioParameterFloat(
            "lp_sft_hrd", "LP Soft/Hard", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction([](float value, int) { return juce::String((value * 2.0f) - 1.0f, 2); })
                .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 1.0f) / 2.0f; })));

    addParameter(lowpass2_poles = new juce::AudioParameterFloat(
                     "lp_poles", "LP Poles", juce::NormalisableRange<float>(0.0f, 1.0f), 0.25f,
                     juce::AudioParameterFloatAttributes()
                         .withStringFromValueFunction([](float value, int) { return juce::String(value * 4.0f, 2); })
                         .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 4.0f; })));

    addParameter(lowpass2_dry_wet = new juce::AudioParameterFloat("lp_dry_wet", "LP Dry/Wet", 0.0f, 1.0f, 1.0f));

    // Highpass2 parameters
    addParameter(highpass2_hipass = new juce::AudioParameterFloat("hipass", "Hipass", 0.0f, 1.0f, 0.0f));
    addParameter(
        highpass2_ls_tite = new juce::AudioParameterFloat(
            "ls_tite", "Ls/Tite", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction([](float value, int) { return juce::String((value * 2.0f) - 1.0f, 2); })
                .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 1.0f) / 2.0f; })));

    addParameter(highpass2_poles = new juce::AudioParameterFloat(
                     "hp_poles", "HP Poles", juce::NormalisableRange<float>(0.0f, 1.0f), 0.25f,
                     juce::AudioParameterFloatAttributes()
                         .withStringFromValueFunction([](float value, int) { return juce::String(value * 4.0f, 2); })
                         .withValueFromStringFunction([](const juce::String &text) { return text.getFloatValue() / 4.0f; })));

    addParameter(highpass2_dry_wet = new juce::AudioParameterFloat("hp_dry_wet", "HP Dry/Wet", 0.0f, 1.0f, 1.0f));

    // Baxandall2 parameters
    addParameter(
        baxandall2_treble = new juce::AudioParameterFloat(
            "treble", "Treble", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction([](float value, int) { return juce::String((value * 48.0f) - 24.0f, 2); })
                .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 24.0f) / 48.0f; })));

    addParameter(
        baxandall2_bass = new juce::AudioParameterFloat(
            "bass", "Bass", juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f,
            juce::AudioParameterFloatAttributes()
                .withStringFromValueFunction([](float value, int) { return juce::String((value * 48.0f) - 24.0f, 2); })
                .withValueFromStringFunction([](const juce::String &text) { return (text.getFloatValue() + 24.0f) / 48.0f; })));

    // Parametric parameters
    addParameter(parametric_tr_freq = new juce::AudioParameterFloat("tr_freq", "Tr Freq", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_treble = new juce::AudioParameterFloat("treble_param", "Treble", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_tr_reso = new juce::AudioParameterFloat("tr_reso", "Tr Reso", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_hm_freq = new juce::AudioParameterFloat("hm_freq", "HM Freq", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_highmid = new juce::AudioParameterFloat("highmid", "HighMid", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_hm_reso = new juce::AudioParameterFloat("hm_reso", "HM Reso", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_lm_freq = new juce::AudioParameterFloat("lm_freq", "LM Freq", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_lowmid = new juce::AudioParameterFloat("lowmid", "LowMid", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_lm_reso = new juce::AudioParameterFloat("lm_reso", "LM Reso", 0.0f, 1.0f, 0.5f));
    addParameter(parametric_dry_wet = new juce::AudioParameterFloat("param_dry_wet", "Param Dry/Wet", 0.0f, 1.0f, 1.0f));

    // Pressure4 parameters
    addParameter(pressure4_pressure = new juce::AudioParameterFloat("pressure", "Pressure", 0.0f, 1.0f, 0.0f));
    addParameter(pressure4_speed = new juce::AudioParameterFloat("speed", "Speed", 0.0f, 1.0f, 0.2f));
    addParameter(pressure4_mewiness = new juce::AudioParameterFloat("mewiness", "Mewiness", 0.0f, 1.0f, 0.5f));
    addParameter(pressure4_output_gain = new juce::AudioParameterFloat("output_gain", "Output Gain", 0.0f, 1.0f, 1.0f));
}

NineStripProcessor::~NineStripProcessor() {}

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

    // Update Channel9 parameters if changed
    int consoleIndex = channel9_consoleType->getIndex();
    channel9.setParameter(Channel9::kParamA, static_cast<float>(consoleIndex));
    channel9.setParameter(Channel9::kParamB,
                          channel9_drive->get() / 2.0f);  // Normalize to 0-1
    channel9.setParameter(Channel9::kParamC, channel9_output->get());

    // Set Lowpass2 parameters
    lowpass2.setParameter(Lowpass2::kParamA, lowpass2_lowpass->get());
    lowpass2.setParameter(Lowpass2::kParamB, lowpass2_sft_hrd->get());
    lowpass2.setParameter(Lowpass2::kParamC, lowpass2_poles->get());
    lowpass2.setParameter(Lowpass2::kParamD, lowpass2_dry_wet->get());

    // Set Highpass2 parameters
    highpass2.setParameter(Highpass2::kParamA, highpass2_hipass->get());
    highpass2.setParameter(Highpass2::kParamB, highpass2_ls_tite->get());
    highpass2.setParameter(Highpass2::kParamC, highpass2_poles->get());
    highpass2.setParameter(Highpass2::kParamD, highpass2_dry_wet->get());

    // Set Baxandall2 parameters
    baxandall2.setParameter(Baxandall2::kParamA, baxandall2_treble->get());
    baxandall2.setParameter(Baxandall2::kParamB, baxandall2_bass->get());

    // Set Parametric parameters
    parametric.setParameter(Parametric::kParamA, parametric_tr_freq->get());
    parametric.setParameter(Parametric::kParamB, parametric_treble->get());
    parametric.setParameter(Parametric::kParamC, parametric_tr_reso->get());
    parametric.setParameter(Parametric::kParamD, parametric_hm_freq->get());
    parametric.setParameter(Parametric::kParamE, parametric_highmid->get());
    parametric.setParameter(Parametric::kParamF, parametric_hm_reso->get());
    parametric.setParameter(Parametric::kParamG, parametric_lm_freq->get());
    parametric.setParameter(Parametric::kParamH, parametric_lowmid->get());
    parametric.setParameter(Parametric::kParamI, parametric_lm_reso->get());
    parametric.setParameter(Parametric::kParamJ, parametric_dry_wet->get());

    // Set Pressure4 parameters
    pressure4.setParameter(Pressure4::kParamA, pressure4_pressure->get());
    pressure4.setParameter(Pressure4::kParamB, pressure4_speed->get());
    pressure4.setParameter(Pressure4::kParamC, pressure4_mewiness->get());
    pressure4.setParameter(Pressure4::kParamD, pressure4_output_gain->get());

    // Create raw pointer arrays for Airwindows
    float *channelData[2] = {
        buffer.getWritePointer(0),  // Left
        buffer.getWritePointer(1)   // Right
    };

    channel9.processReplacing(channelData, channelData, buffer.getNumSamples());
    lowpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
    highpass2.processReplacing(channelData, channelData, buffer.getNumSamples());
    baxandall2.processReplacing(channelData, channelData, buffer.getNumSamples());
    parametric.processReplacing(channelData, channelData, buffer.getNumSamples());
    pressure4.processReplacing(channelData, channelData, buffer.getNumSamples());
}

//==============================================================================
bool NineStripProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor *NineStripProcessor::createEditor() { return new NineStripProcessorEditor(*this); }

//==============================================================================
void NineStripProcessor::getStateInformation(juce::MemoryBlock &destData) {
    // Store parameter values
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("NineStripSettings"));
    xml->setAttribute("consoleType", channel9_consoleType->getIndex());
    xml->setAttribute("drive", static_cast<double>(channel9_drive->get()));
    xml->setAttribute("output", static_cast<double>(channel9_output->get()));

    xml->setAttribute("lowpass", static_cast<double>(lowpass2_lowpass->get()));
    xml->setAttribute("lp_sft_hrd", static_cast<double>(lowpass2_sft_hrd->get()));
    xml->setAttribute("lp_poles", static_cast<double>(lowpass2_poles->get()));
    xml->setAttribute("lp_dry_wet", static_cast<double>(lowpass2_dry_wet->get()));

    xml->setAttribute("hipass", static_cast<double>(highpass2_hipass->get()));
    xml->setAttribute("ls_tite", static_cast<double>(highpass2_ls_tite->get()));
    xml->setAttribute("hp_poles", static_cast<double>(highpass2_poles->get()));
    xml->setAttribute("hp_dry_wet", static_cast<double>(highpass2_dry_wet->get()));

    xml->setAttribute("treble", static_cast<double>(baxandall2_treble->get()));
    xml->setAttribute("bass", static_cast<double>(baxandall2_bass->get()));

    xml->setAttribute("tr_freq", static_cast<double>(parametric_tr_freq->get()));
    xml->setAttribute("treble_param", static_cast<double>(parametric_treble->get()));
    xml->setAttribute("tr_reso", static_cast<double>(parametric_tr_reso->get()));
    xml->setAttribute("hm_freq", static_cast<double>(parametric_hm_freq->get()));
    xml->setAttribute("highmid", static_cast<double>(parametric_highmid->get()));
    xml->setAttribute("hm_reso", static_cast<double>(parametric_hm_reso->get()));
    xml->setAttribute("lm_freq", static_cast<double>(parametric_lm_freq->get()));
    xml->setAttribute("lowmid", static_cast<double>(parametric_lowmid->get()));
    xml->setAttribute("lm_reso", static_cast<double>(parametric_lm_reso->get()));
    xml->setAttribute("param_dry_wet", static_cast<double>(parametric_dry_wet->get()));

    xml->setAttribute("pressure", static_cast<double>(pressure4_pressure->get()));
    xml->setAttribute("speed", static_cast<double>(pressure4_speed->get()));
    xml->setAttribute("mewiness", static_cast<double>(pressure4_mewiness->get()));
    xml->setAttribute("output_gain", static_cast<double>(pressure4_output_gain->get()));

    copyXmlToBinary(*xml, destData);
}

void NineStripProcessor::setStateInformation(const void *data, int sizeInBytes) {
    // Restore parameter values
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName("NineStripSettings")) {
        *channel9_consoleType = xml->getIntAttribute("consoleType", 0);
        *channel9_drive = static_cast<float>(xml->getDoubleAttribute("drive", 0.0));
        *channel9_output = static_cast<float>(xml->getDoubleAttribute("output", 1.0));

        *lowpass2_lowpass = static_cast<float>(xml->getDoubleAttribute("lowpass", 1.0));
        *lowpass2_sft_hrd = static_cast<float>(xml->getDoubleAttribute("lp_sft_hrd", 0.5));
        *lowpass2_poles = static_cast<float>(xml->getDoubleAttribute("lp_poles", 0.25));
        *lowpass2_dry_wet = static_cast<float>(xml->getDoubleAttribute("lp_dry_wet", 1.0));

        *highpass2_hipass = static_cast<float>(xml->getDoubleAttribute("hipass", 0.0));
        *highpass2_ls_tite = static_cast<float>(xml->getDoubleAttribute("ls_tite", 0.5));
        *highpass2_poles = static_cast<float>(xml->getDoubleAttribute("hp_poles", 0.25));
        *highpass2_dry_wet = static_cast<float>(xml->getDoubleAttribute("hp_dry_wet", 1.0));

        *baxandall2_treble = static_cast<float>(xml->getDoubleAttribute("treble", 0.5));
        *baxandall2_bass = static_cast<float>(xml->getDoubleAttribute("bass", 0.5));

        *parametric_tr_freq = static_cast<float>(xml->getDoubleAttribute("tr_freq", 0.5));
        *parametric_treble = static_cast<float>(xml->getDoubleAttribute("treble_param", 0.5));
        *parametric_tr_reso = static_cast<float>(xml->getDoubleAttribute("tr_reso", 0.5));
        *parametric_hm_freq = static_cast<float>(xml->getDoubleAttribute("hm_freq", 0.5));
        *parametric_highmid = static_cast<float>(xml->getDoubleAttribute("highmid", 0.5));
        *parametric_hm_reso = static_cast<float>(xml->getDoubleAttribute("hm_reso", 0.5));
        *parametric_lm_freq = static_cast<float>(xml->getDoubleAttribute("lm_freq", 0.5));
        *parametric_lowmid = static_cast<float>(xml->getDoubleAttribute("lowmid", 0.5));
        *parametric_lm_reso = static_cast<float>(xml->getDoubleAttribute("lm_reso", 0.5));
        *parametric_dry_wet = static_cast<float>(xml->getDoubleAttribute("param_dry_wet", 1.0));

        *pressure4_pressure = static_cast<float>(xml->getDoubleAttribute("pressure", 0.0));
        *pressure4_speed = static_cast<float>(xml->getDoubleAttribute("speed", 0.2));
        *pressure4_mewiness = static_cast<float>(xml->getDoubleAttribute("mewiness", 0.5));
        *pressure4_output_gain = static_cast<float>(xml->getDoubleAttribute("output_gain", 1.0));
    }
}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() { return new NineStripProcessor(); }
