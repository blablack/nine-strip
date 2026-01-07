#include "PluginEditor.h"

#include "PluginProcessor.h"

NineStripProcessorEditor::NineStripProcessorEditor(NineStripProcessor& p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      inputMeterL([&p]() { return p.getInputLevelL(); }),
      inputMeterR([&p]() { return p.getInputLevelR(); }),
      outputMeterL([&p]() { return p.getOutputLevelL(); }),
      outputMeterR([&p]() { return p.getOutputLevelR(); }),
      grMeter([&p]() { return p.getGainReduction(); }, VUMeter::MeterType::GainReduction)
{
    setSize(900, 700);

    // Configure and add VU meters
    addAndMakeVisible(inputMeterL);
    addAndMakeVisible(inputMeterR);
    addAndMakeVisible(outputMeterL);
    addAndMakeVisible(outputMeterR);
    addAndMakeVisible(grMeter);

    // Master Bypass
    addAndMakeVisible(masterBypassButton);
    masterBypassButton.setClickingTogglesState(true);
    masterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "masterBypass", masterBypassButton);

    // Input Gain
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                 "inputGain", inputGainSlider);
    addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input Gain", juce::dontSendNotification);
    inputGainLabel.attachToComponent(&inputGainSlider, false);
    inputGainLabel.setJustificationType(juce::Justification::centredTop);

    // Output Gain
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "outputGain", outputGainSlider);
    addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output Gain", juce::dontSendNotification);
    inputGainLabel.attachToComponent(&outputGainSlider, false);
    outputGainLabel.setJustificationType(juce::Justification::centredTop);

    // Section Bypasses
    addAndMakeVisible(saturationBypassButton);
    saturationBypassButton.setClickingTogglesState(true);
    saturationBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "saturationBypass", saturationBypassButton);

    addAndMakeVisible(eqBypassButton);
    eqBypassButton.setClickingTogglesState(true);
    eqBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,
                                                                                                "eqBypass", eqBypassButton);

    addAndMakeVisible(compressorBypassButton);
    compressorBypassButton.setClickingTogglesState(true);
    compressorBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "compressorBypass", compressorBypassButton);

    // Console Type
    addAndMakeVisible(consoleTypeCombo);
    consoleTypeCombo.addItemList(juce::StringArray{"Neve", "API", "SSL", "Teac", "Mackie"}, 1);
    consoleTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "consoleType", consoleTypeCombo);
    addAndMakeVisible(consoleTypeLabel);
    consoleTypeLabel.setText("Console Type", juce::dontSendNotification);
    consoleTypeLabel.attachToComponent(&consoleTypeCombo, false);

    // Drive
    addAndMakeVisible(driveSlider);
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    driveAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "drive", driveSlider);
    addAndMakeVisible(driveLabel);
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.attachToComponent(&driveSlider, false);

    // Highpass
    addAndMakeVisible(hipassSlider);
    hipassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hipassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    hipassAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hipass", hipassSlider);
    addAndMakeVisible(hipassLabel);
    hipassLabel.setText("Hipass", juce::dontSendNotification);
    hipassLabel.attachToComponent(&hipassSlider, false);

    addAndMakeVisible(hpPolesSlider);
    hpPolesSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpPolesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    hpPolesAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hp_poles", hpPolesSlider);
    addAndMakeVisible(hpPolesLabel);
    hpPolesLabel.setText("HP Slope", juce::dontSendNotification);
    hpPolesLabel.attachToComponent(&hpPolesSlider, false);

    addAndMakeVisible(hpLsTiteSlider);
    hpLsTiteSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpLsTiteSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    hpLsTiteAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ls_tite", hpLsTiteSlider);
    addAndMakeVisible(hpLsTiteLabel);
    hpLsTiteLabel.setText("HP Ls/Tite", juce::dontSendNotification);
    hpLsTiteLabel.attachToComponent(&hpLsTiteSlider, false);

    // Lowpass
    addAndMakeVisible(lowpassSlider);
    lowpassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowpassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    lowpassAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lowpass", lowpassSlider);
    addAndMakeVisible(lowpassLabel);
    lowpassLabel.setText("Lowpass", juce::dontSendNotification);
    lowpassLabel.attachToComponent(&lowpassSlider, false);

    addAndMakeVisible(lpPolesSlider);
    lpPolesSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpPolesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    lpPolesAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lp_poles", lpPolesSlider);
    addAndMakeVisible(lpPolesLabel);
    lpPolesLabel.setText("LP Slope", juce::dontSendNotification);
    lpPolesLabel.attachToComponent(&lpPolesSlider, false);

    addAndMakeVisible(lpSftHrdSlider);
    lpSftHrdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpSftHrdSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    lpSftHrdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                "lp_sft_hrd", lpSftHrdSlider);
    addAndMakeVisible(lpSftHrdLabel);
    lpSftHrdLabel.setText("LP Sft/Hrd", juce::dontSendNotification);
    lpSftHrdLabel.attachToComponent(&lpSftHrdSlider, false);

    // Baxandall EQ
    addAndMakeVisible(trebleSlider);
    trebleSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trebleSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    trebleAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "treble", trebleSlider);
    addAndMakeVisible(trebleLabel);
    trebleLabel.setText("High Shelf", juce::dontSendNotification);
    trebleLabel.attachToComponent(&trebleSlider, false);

    addAndMakeVisible(bassSlider);
    bassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bassSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    bassAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "bass", bassSlider);
    addAndMakeVisible(bassLabel);
    bassLabel.setText("Low Shelf", juce::dontSendNotification);
    bassLabel.attachToComponent(&bassSlider, false);

    // Parametric - Treble Band
    /*
    addAndMakeVisible(trFreqSlider);
    trFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    trFreqAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "tr_freq", trFreqSlider);
    addAndMakeVisible(trFreqLabel);
    trFreqLabel.setText("Tr Freq", juce::dontSendNotification);
    trFreqLabel.attachToComponent(&trFreqSlider, false);

    addAndMakeVisible(trGainSlider);
    trGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    trGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                              "treble_param", trGainSlider);
    addAndMakeVisible(trGainLabel);
    trGainLabel.setText("Tr Gain", juce::dontSendNotification);
    trGainLabel.attachToComponent(&trGainSlider, false);

    addAndMakeVisible(trResoSlider);
    trResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trResoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    trResoAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "tr_reso", trResoSlider);
    addAndMakeVisible(trResoLabel);
    trResoLabel.setText("Tr Q", juce::dontSendNotification);
    trResoLabel.attachToComponent(&trResoSlider, false);
    */

    // Parametric - High-Mid Band
    addAndMakeVisible(hmFreqSlider);
    hmFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hmFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    hmFreqAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hm_freq", hmFreqSlider);
    addAndMakeVisible(hmFreqLabel);
    hmFreqLabel.setText("High-Mid Freq", juce::dontSendNotification);
    hmFreqLabel.attachToComponent(&hmFreqSlider, false);

    addAndMakeVisible(hmGainSlider);
    hmGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hmGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    hmGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "highmid", hmGainSlider);
    addAndMakeVisible(hmGainLabel);
    hmGainLabel.setText("High-Mid Gain", juce::dontSendNotification);
    hmGainLabel.attachToComponent(&hmGainSlider, false);

    addAndMakeVisible(hmResoSlider);
    hmResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hmResoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    hmResoAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hm_reso", hmResoSlider);
    addAndMakeVisible(hmResoLabel);
    hmResoLabel.setText("High-Mid Q", juce::dontSendNotification);
    hmResoLabel.attachToComponent(&hmResoSlider, false);

    // Parametric - Low-Mid Band
    /*
    addAndMakeVisible(lmFreqSlider);
    lmFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lmFreqSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    lmFreqAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lm_freq", lmFreqSlider);
    addAndMakeVisible(lmFreqLabel);
    lmFreqLabel.setText("LM Freq", juce::dontSendNotification);
    lmFreqLabel.attachToComponent(&lmFreqSlider, false);

    addAndMakeVisible(lmGainSlider);
    lmGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lmGainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    lmGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lowmid", lmGainSlider);
    addAndMakeVisible(lmGainLabel);
    lmGainLabel.setText("LM Gain", juce::dontSendNotification);
    lmGainLabel.attachToComponent(&lmGainSlider, false);

    addAndMakeVisible(lmResoSlider);
    lmResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lmResoSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    lmResoAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lm_reso", lmResoSlider);
    addAndMakeVisible(lmResoLabel);
    lmResoLabel.setText("LM Q", juce::dontSendNotification);
    lmResoLabel.attachToComponent(&lmResoSlider, false);
    */

    // Compressor
    addAndMakeVisible(pressureSlider);
    pressureSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pressureSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    pressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                "pressure", pressureSlider);
    addAndMakeVisible(pressureLabel);
    pressureLabel.setText("Pressure", juce::dontSendNotification);
    pressureLabel.attachToComponent(&pressureSlider, false);

    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    speedAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "speed", speedSlider);
    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);

    addAndMakeVisible(mewinessSlider);
    mewinessSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mewinessSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mewinessAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                "mewiness", mewinessSlider);
    addAndMakeVisible(mewinessLabel);
    mewinessLabel.setText("Mewiness", juce::dontSendNotification);
    mewinessLabel.attachToComponent(&mewinessSlider, false);

    // Preset Manager
    addAndMakeVisible(presetComboBox);
    presetComboBox.setTextWhenNothingSelected("No Preset Selected");
    presetComboBox.addListener(this);

    addAndMakeVisible(savePresetButton);
    savePresetButton.setButtonText("Save");
    savePresetButton.addListener(this);

    addAndMakeVisible(deletePresetButton);
    deletePresetButton.setButtonText("Delete");
    deletePresetButton.addListener(this);

    addAndMakeVisible(previousPresetButton);
    previousPresetButton.setButtonText("<");
    previousPresetButton.addListener(this);

    addAndMakeVisible(nextPresetButton);
    nextPresetButton.setButtonText(">");
    nextPresetButton.addListener(this);

    // Populate the preset list
    updatePresetComboBox();
}

NineStripProcessorEditor::~NineStripProcessorEditor() {}

void NineStripProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    // Draw section backgrounds
    g.setColour(juce::Colours::grey);
    g.fillRect(50, 50, 800, 100);   // Saturation
    g.fillRect(50, 170, 800, 100);  // Filters
    g.fillRect(50, 290, 800, 150);  // EQ
    g.fillRect(50, 460, 800, 100);  // Compression

    // Section labels
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawText("SATURATION", 60, 30, 100, 20, juce::Justification::centredLeft);
    g.drawText("FILTERS", 60, 150, 100, 20, juce::Justification::centredLeft);
    g.drawText("EQ", 60, 270, 100, 20, juce::Justification::centredLeft);
    g.drawText("COMPRESSION", 60, 440, 100, 20, juce::Justification::centredLeft);
}

void NineStripProcessorEditor::updatePresetComboBox()
{
    presetComboBox.clear(juce::dontSendNotification);

    auto& presetManager = audioProcessor.getPresetManager();
    auto presets = presetManager.getAllPresets();

    int itemId = 1;
    for (const auto& preset : presets)
    {
        presetComboBox.addItem(preset, itemId++);
    }

    // Select current preset and show asterisk if modified
    auto currentPreset = presetManager.getCurrentPreset();
    if (!currentPreset.isEmpty())
    {
        int index = presets.indexOf(currentPreset);
        if (index >= 0)
        {
            presetComboBox.setSelectedId(index + 1, juce::dontSendNotification);

            // Add asterisk if modified
            if (presetManager.isPresetModified()) presetComboBox.setText(currentPreset + " *", juce::dontSendNotification);
        }
    }
}

void NineStripProcessorEditor::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &presetComboBox)
    {
        auto selectedText = presetComboBox.getText();
        if (!selectedText.isEmpty())
        {
            audioProcessor.getPresetManager().loadPreset(selectedText);
        }
    }
}

void NineStripProcessorEditor::updatePresetDisplay()
{
    auto& presetManager = audioProcessor.getPresetManager();
    auto currentPreset = presetManager.getCurrentPreset();

    if (!currentPreset.isEmpty())
    {
        auto displayText = presetManager.isPresetModified() ? currentPreset + " *" : currentPreset;

        // Only update if text actually changed
        if (presetComboBox.getText() != displayText) presetComboBox.setText(displayText, juce::dontSendNotification);
    }
}

void NineStripProcessorEditor::buttonClicked(juce::Button* button)
{
    auto& presetManager = audioProcessor.getPresetManager();

    if (button == &savePresetButton)
    {
        auto* alert = new juce::AlertWindow("Save Preset", "Enter preset name:", juce::AlertWindow::NoIcon);

        alert->addTextEditor("presetName", "", "Preset Name:");
        alert->addButton("OK", 1);
        alert->addButton("Cancel", 0);

        alert->enterModalState(true,
                               juce::ModalCallbackFunction::create(
                                   [this, alert](int result)
                                   {
                                       if (result == 1)
                                       {
                                           auto presetName = alert->getTextEditorContents("presetName");
                                           if (!presetName.isEmpty())
                                           {
                                               audioProcessor.getPresetManager().savePreset(presetName);
                                               updatePresetComboBox();
                                           }
                                       }
                                       delete alert;
                                   }),
                               true);
    }
    else if (button == &deletePresetButton)
    {
        auto currentPreset = presetManager.getCurrentPreset();
        if (!currentPreset.isEmpty())
        {
            presetManager.deletePreset(currentPreset);
            updatePresetComboBox();
        }
    }
    else if (button == &previousPresetButton)
    {
        presetManager.loadPreviousPreset();
        updatePresetComboBox();
    }
    else if (button == &nextPresetButton)
    {
        presetManager.loadNextPreset();
        updatePresetComboBox();
    }
}

void NineStripProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    // Preset Management
    previousPresetButton.setBounds(110, 10, 30, 30);
    presetComboBox.setBounds(145, 10, 200, 30);
    nextPresetButton.setBounds(350, 10, 30, 30);
    savePresetButton.setBounds(390, 10, 60, 30);
    deletePresetButton.setBounds(455, 10, 60, 30);

    // VU Meters - Left side
    inputMeterL.setBounds(10, 50, 20, 510);
    inputMeterR.setBounds(32, 50, 20, 510);

    // VU Meters - Right side
    outputMeterL.setBounds(855, 50, 20, 510);
    outputMeterR.setBounds(877, 50, 20, 510);

    // Gain Reduction Meter
    grMeter.setBounds(70, 590, 80, 100);

    // Master Bypass
    masterBypassButton.setBounds(10, 10, 80, 30);

    // Input/Output Gain
    inputGainSlider.setBounds(10, 590, 80, 100);
    outputGainSlider.setBounds(810, 590, 80, 100);

    // Saturation Section
    saturationBypassButton.setBounds(770, 55, 60, 25);
    consoleTypeCombo.setBounds(70, 75, 120, 25);
    driveSlider.setBounds(220, 65, 80, 80);

    // Filter Section
    eqBypassButton.setBounds(770, 175, 60, 25);
    hipassSlider.setBounds(70, 195, 80, 70);
    hpPolesSlider.setBounds(160, 195, 60, 70);
    hpLsTiteSlider.setBounds(230, 195, 60, 70);

    lowpassSlider.setBounds(320, 195, 80, 70);
    lpPolesSlider.setBounds(410, 195, 60, 70);
    lpSftHrdSlider.setBounds(480, 195, 60, 70);

    // EQ Section - Baxandall Shelves
    bassSlider.setBounds(70, 315, 80, 80);
    trebleSlider.setBounds(160, 315, 80, 80);

    // Parametric - 3 bands
    // lmFreqSlider.setBounds(270, 315, 70, 80);
    // lmGainSlider.setBounds(345, 315, 70, 80);
    // lmResoSlider.setBounds(420, 315, 70, 80);

    hmFreqSlider.setBounds(505, 315, 70, 80);
    hmGainSlider.setBounds(580, 315, 70, 80);
    hmResoSlider.setBounds(655, 315, 70, 80);

    // trFreqSlider.setBounds(270, 355, 70, 80);
    // trGainSlider.setBounds(345, 355, 70, 80);
    // trResoSlider.setBounds(420, 355, 70, 80);

    // Compression Section
    compressorBypassButton.setBounds(770, 465, 60, 25);
    grMeter.setBounds(345, 485, 30, 70);  // Position near compressor controls
    pressureSlider.setBounds(70, 485, 80, 70);
    speedSlider.setBounds(160, 485, 80, 70);
    mewinessSlider.setBounds(250, 485, 80, 70);
}
