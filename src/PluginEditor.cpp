#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "juce_graphics/juce_graphics.h"

NineStripProcessorEditor::NineStripProcessorEditor(NineStripProcessor& p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      measuredMeterL([&p]() { return p.getMeasuredLevelL(); }),
      measuredMeterR([&p]() { return p.getMeasuredLevelR(); }),
      grMeter([&p]() { return p.getGainReduction(); }, VUMeter::MeterType::GainReduction)
{
    setSize(580, 600);
    constrainer.setFixedAspectRatio(580.0f / 600.0f);
    constrainer.setSizeLimits(580, 600, 1740, 1800);
    setConstrainer(&constrainer);
    setResizable(false, true);

    // ========== PRESET PANEL ==========
    addAndMakeVisible(presetPanel);
    presetPanel.addAndMakeVisible(presetComboBox);
    presetComboBox.setTextWhenNothingSelected("No Preset Selected");
    presetComboBox.addListener(this);

    presetPanel.addAndMakeVisible(previousPresetButton);
    previousPresetButton.setButtonText("<");
    previousPresetButton.addListener(this);

    presetPanel.addAndMakeVisible(nextPresetButton);
    nextPresetButton.setButtonText(">");
    nextPresetButton.addListener(this);

    presetPanel.addAndMakeVisible(savePresetButton);
    savePresetButton.setButtonText("Save");
    savePresetButton.addListener(this);

    presetPanel.addAndMakeVisible(deletePresetButton);
    deletePresetButton.setButtonText("Delete");
    deletePresetButton.addListener(this);

    // ========== COLUMN 1: CONSOLE & FILTERS ==========
    // Console & Saturation
    addAndMakeVisible(consoleSatGroup);
    consoleSatGroup.setText("Console");
    consoleSatGroup.setTextLabelPosition(juce::Justification::centredTop);

    consoleSatGroup.addAndMakeVisible(consoleTypeCombo);
    consoleTypeCombo.addItemList(juce::StringArray{"Neve", "API", "SSL", "Teac", "Mackie"}, 1);
    consoleTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.apvts, "consoleType", consoleTypeCombo);

    consoleSatGroup.addAndMakeVisible(driveSlider);
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    driveAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "drive", driveSlider);
    consoleSatGroup.addAndMakeVisible(driveLabel);
    driveLabel.setText("Drive", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);

    consoleSatGroup.addAndMakeVisible(saturationBypassButton);
    saturationBypassButton.setClickingTogglesState(true);
    saturationBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "saturationBypass", saturationBypassButton);

    // High Pass Filter
    addAndMakeVisible(highPassGroup);
    highPassGroup.setText("High Pass");
    highPassGroup.setTextLabelPosition(juce::Justification::centredTop);

    highPassGroup.addAndMakeVisible(hipassSlider);
    hipassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hipassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    hipassAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hipass", hipassSlider);
    highPassGroup.addAndMakeVisible(hipassLabel);
    hipassLabel.setText("Freq", juce::dontSendNotification);
    hipassLabel.setJustificationType(juce::Justification::centred);

    highPassGroup.addAndMakeVisible(hpLsTiteSlider);
    hpLsTiteSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpLsTiteSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    hpLsTiteAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ls_tite", hpLsTiteSlider);
    highPassGroup.addAndMakeVisible(hpLsTiteLabel);
    hpLsTiteLabel.setText("Ls/Tite", juce::dontSendNotification);
    hpLsTiteLabel.setJustificationType(juce::Justification::centred);

    highPassGroup.addAndMakeVisible(hpPolesSlider);
    hpPolesSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hpPolesSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    hpPolesAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hp_poles", hpPolesSlider);
    highPassGroup.addAndMakeVisible(hpPolesLabel);
    hpPolesLabel.setText("Slope", juce::dontSendNotification);
    hpPolesLabel.setJustificationType(juce::Justification::centred);

    // Low Pass Filter
    addAndMakeVisible(lowPassGroup);
    lowPassGroup.setText("Low Pass");
    lowPassGroup.setTextLabelPosition(juce::Justification::centredTop);

    lowPassGroup.addAndMakeVisible(lowpassSlider);
    lowpassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lowpassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lowpassAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lowpass", lowpassSlider);
    lowPassGroup.addAndMakeVisible(lowpassLabel);
    lowpassLabel.setText("Freq", juce::dontSendNotification);
    lowpassLabel.setJustificationType(juce::Justification::centred);

    lowPassGroup.addAndMakeVisible(lpSftHrdSlider);
    lpSftHrdSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpSftHrdSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lpSftHrdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                "lp_sft_hrd", lpSftHrdSlider);
    lowPassGroup.addAndMakeVisible(lpSftHrdLabel);
    lpSftHrdLabel.setText("Soft/Hard", juce::dontSendNotification);
    lpSftHrdLabel.setJustificationType(juce::Justification::centred);

    lowPassGroup.addAndMakeVisible(lpPolesSlider);
    lpPolesSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    lpPolesSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    lpPolesAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "lp_poles", lpPolesSlider);
    lowPassGroup.addAndMakeVisible(lpPolesLabel);
    lpPolesLabel.setText("Slope", juce::dontSendNotification);
    lpPolesLabel.setJustificationType(juce::Justification::centred);

    // ========== COLUMN 2: EQ ==========
    // High Shelf
    addAndMakeVisible(highShelfGroup);
    highShelfGroup.setText("High Shelf");
    highShelfGroup.setTextLabelPosition(juce::Justification::centredTop);

    highShelfGroup.addAndMakeVisible(trebleSlider);
    trebleSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    trebleSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    trebleAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "treble", trebleSlider);
    highShelfGroup.addAndMakeVisible(trebleLabel);
    trebleLabel.setText("Gain", juce::dontSendNotification);
    trebleLabel.setJustificationType(juce::Justification::centred);

    // High-Mid EQ
    addAndMakeVisible(highMidGroup);
    highMidGroup.setText("Hi-Mid EQ");
    highMidGroup.setTextLabelPosition(juce::Justification::centredTop);

    highMidGroup.addAndMakeVisible(hmFreqSlider);
    hmFreqSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hmFreqSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    hmFreqAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hm_freq", hmFreqSlider);
    highMidGroup.addAndMakeVisible(hmFreqLabel);
    hmFreqLabel.setText("Freq", juce::dontSendNotification);
    hmFreqLabel.setJustificationType(juce::Justification::centred);

    highMidGroup.addAndMakeVisible(hmGainSlider);
    hmGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hmGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    hmGainAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "highmid", hmGainSlider);
    highMidGroup.addAndMakeVisible(hmGainLabel);
    hmGainLabel.setText("Gain", juce::dontSendNotification);
    hmGainLabel.setJustificationType(juce::Justification::centred);

    highMidGroup.addAndMakeVisible(hmResoSlider);
    hmResoSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    hmResoSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    hmResoAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "hm_reso", hmResoSlider);
    highMidGroup.addAndMakeVisible(hmResoLabel);
    hmResoLabel.setText("Q", juce::dontSendNotification);
    hmResoLabel.setJustificationType(juce::Justification::centred);

    // Low Shelf
    addAndMakeVisible(lowShelfGroup);
    lowShelfGroup.setText("Low Shelf");
    lowShelfGroup.setTextLabelPosition(juce::Justification::centredTop);

    lowShelfGroup.addAndMakeVisible(bassSlider);
    bassSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    bassSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    bassAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "bass", bassSlider);
    lowShelfGroup.addAndMakeVisible(bassLabel);
    bassLabel.setText("Gain", juce::dontSendNotification);
    bassLabel.setJustificationType(juce::Justification::centred);

    // EQ Bypass
    lowShelfGroup.addAndMakeVisible(eqBypassButton);
    eqBypassButton.setClickingTogglesState(true);
    eqBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,
                                                                                                "eqBypass", eqBypassButton);

    // ========== COLUMN 3: COMPRESSOR ==========
    addAndMakeVisible(compressorGroup);
    compressorGroup.setText("Dynamics");
    compressorGroup.setTextLabelPosition(juce::Justification::centredTop);

    compressorGroup.addAndMakeVisible(pressureSlider);
    pressureSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    pressureSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    pressureAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                "pressure", pressureSlider);
    compressorGroup.addAndMakeVisible(pressureLabel);
    pressureLabel.setText("Pressure", juce::dontSendNotification);
    pressureLabel.setJustificationType(juce::Justification::centred);

    compressorGroup.addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    speedSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    speedAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "speed", speedSlider);
    compressorGroup.addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.setJustificationType(juce::Justification::centred);

    compressorGroup.addAndMakeVisible(mewinessSlider);
    mewinessSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mewinessSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mewinessAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                "mewiness", mewinessSlider);
    compressorGroup.addAndMakeVisible(mewinessLabel);
    mewinessLabel.setText("Mewiness", juce::dontSendNotification);
    mewinessLabel.setJustificationType(juce::Justification::centred);

    compressorGroup.addAndMakeVisible(grMeter);

    compressorGroup.addAndMakeVisible(compressorBypassButton);
    compressorBypassButton.setClickingTogglesState(true);
    compressorBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "compressorBypass", compressorBypassButton);

    // ========== COLUMN 4: METERS & GAIN ==========
    addAndMakeVisible(metersGroup);
    metersGroup.setText("Meters");
    metersGroup.setTextLabelPosition(juce::Justification::centredTop);

    metersGroup.addAndMakeVisible(measuredMeterL);
    metersGroup.addAndMakeVisible(measuredMeterR);

    metersGroup.addAndMakeVisible(vuMeterModeButton);
    vuMeterModeButton.setClickingTogglesState(true);
    vuMeterModeButton.addListener(this);
    vuMeterModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "inputMeasured", vuMeterModeButton);
    audioProcessor.apvts.addParameterListener("inputMeasured", this);
    vuMeterModeButton.setButtonText((audioProcessor.apvts.getRawParameterValue("inputMeasured")->load() > 0.5f) ? "Input"
                                                                                                                : "Output");

    // GAIN
    addAndMakeVisible(gainGroup);

    gainGroup.setText("Gain");
    gainGroup.setTextLabelPosition(juce::Justification::centredTop);

    gainGroup.addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    inputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                 "inputGain", inputGainSlider);
    gainGroup.addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);

    gainGroup.addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    outputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "outputGain", outputGainSlider);
    gainGroup.addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::centred);

    gainGroup.addAndMakeVisible(masterBypassButton);
    masterBypassButton.setClickingTogglesState(true);
    masterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "masterBypass", masterBypassButton);

    updatePresetComboBox();
}

NineStripProcessorEditor::~NineStripProcessorEditor() { audioProcessor.apvts.removeParameterListener("inputMeasured", this); }

void NineStripProcessorEditor::paint(juce::Graphics& g)
{
    g.setGradientFill(
        juce::ColourGradient(juce::Colour(0xff3f3f46), 0, 0, juce::Colour(0xff18181b), 0, (float)getHeight(), false));
    g.fillAll();
}

void NineStripProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "inputMeasured")
    {
        vuMeterModeButton.setButtonText(newValue > 0.5f ? "Input" : "Output");
    }
}

void NineStripProcessorEditor::resized()
{
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;

    // Variables for position calculations
    int centerX = 0;
    int centerY = 0;

    int triangleWidth = 0;
    int triangleHeight = 0;

    int triangleX = 0;
    int triangleY = 0;

    int topY = 0;
    int bottomY = 0;
    int bottomX = 0;

    // Window size

    auto bounds = getLocalBounds().reduced(8);

    // Knobs size

    int columnWidth = bounds.getWidth() / 4;   // 4 columns
    int bigKnobSize = (columnWidth - 20) / 2;  // Account for padding
    int smallKnobSize = bigKnobSize * 0.8f;

    // ========== MAIN GRID: 4 columns + preset row ==========
    mainGrid.templateRows = {
        Track(Px(40)),  // Row 1: Preset
        Track(Fr(1)),   // Row 2: Top sections
        Track(Fr(1)),   // Row 3: Bottom sections
        Track(Fr(1))    // Row 4: Bottom sections
    };

    mainGrid.templateColumns = {
        Track(Fr(1)),  // Column 1: Console/Filters
        Track(Fr(1)),  // Column 2: EQ
        Track(Fr(1)),  // Column 3: Meters (wider)
        Track(Fr(1))   // Column 4: Dynamics/Gain
    };

    mainGrid.items.clear();

    // Preset panel - spans all 4 columns in row 1
    mainGrid.items.add(juce::GridItem(presetPanel).withArea(1, 1, 2, 5));

    // Console + Filters - spans rows 2-4, column 1
    mainGrid.items.add(juce::GridItem(consoleSatGroup).withArea(2, 1, 3, 2));
    mainGrid.items.add(juce::GridItem(highPassGroup).withArea(3, 1, 4, 2));
    mainGrid.items.add(juce::GridItem(lowPassGroup).withArea(4, 1, 5, 2));

    // EQ - spans rows 2-4, column 2
    mainGrid.items.add(juce::GridItem(highShelfGroup).withArea(2, 2, 3, 3));
    mainGrid.items.add(juce::GridItem(highMidGroup).withArea(3, 2, 4, 3));
    mainGrid.items.add(juce::GridItem(lowShelfGroup).withArea(4, 2, 5, 3));

    // Meters - spans rows 2, column 3-4
    mainGrid.items.add(juce::GridItem(metersGroup).withArea(2, 3, 3, 5));

    // Dynamics - spans rows 3-4, column 3
    mainGrid.items.add(juce::GridItem(compressorGroup).withArea(3, 3, 5, 4));

    // Gain - spans rows 3-4, column 4
    mainGrid.items.add(juce::GridItem(gainGroup).withArea(3, 4, 5, 5));

    mainGrid.setGap(juce::Grid::Px(8));
    mainGrid.performLayout(bounds);

    // ========== PRESET PANEL ==========
    auto presetBounds = presetPanel.getLocalBounds().reduced(4);
    previousPresetButton.setBounds(presetBounds.removeFromLeft(40).reduced(2));
    deletePresetButton.setBounds(presetBounds.removeFromRight(60).reduced(2));
    savePresetButton.setBounds(presetBounds.removeFromRight(60).reduced(2));
    nextPresetButton.setBounds(presetBounds.removeFromRight(40).reduced(2));
    presetComboBox.setBounds(presetBounds.reduced(2));

    // Console layout
    auto consoleBounds = consoleSatGroup.getLocalBounds().reduced(6);
    consoleTypeCombo.setBounds(consoleBounds.getX() + 2, consoleBounds.getY() + 20, consoleBounds.getWidth() - 4, 24);
    centerX = consoleBounds.getX() + (consoleBounds.getWidth() - bigKnobSize) / 2;
    centerY = consoleBounds.getY() + (consoleBounds.getHeight() - bigKnobSize) / 2;
    driveSlider.setBounds(centerX, centerY, bigKnobSize, bigKnobSize);
    driveLabel.setBounds(driveSlider.getX(), driveSlider.getBottom(), driveSlider.getWidth(), 12);
    saturationBypassButton.setBounds(consoleBounds.getRight() - 52, consoleBounds.getBottom() - 26, 50, 20);

    // High Pass layout
    auto hpBounds = highPassGroup.getLocalBounds().reduced(6);

    // Calculate total triangle dimensions
    triangleWidth = bigKnobSize * 2;                   // Two big knobs side by side
    triangleHeight = bigKnobSize + smallKnobSize + 8;  // Top row + gap + bottom row

    // Center the triangle in the available space
    triangleX = hpBounds.getX() + (hpBounds.getWidth() - triangleWidth) / 2;
    triangleY = hpBounds.getY() + (hpBounds.getHeight() - triangleHeight) / 2;

    // Top row - two big knobs side by side
    topY = triangleY;
    hipassSlider.setBounds(triangleX, topY, bigKnobSize, bigKnobSize);
    hipassLabel.setBounds(hipassSlider.getX(), hipassSlider.getBottom(), bigKnobSize, 12);

    hpPolesSlider.setBounds(triangleX + bigKnobSize, topY, bigKnobSize, bigKnobSize);
    hpPolesLabel.setBounds(hpPolesSlider.getX(), hpPolesSlider.getBottom(), bigKnobSize, 12);

    // Bottom - small knob centered below the gap between the two big knobs
    bottomY = triangleY + bigKnobSize + 8;                      // 8px gap
    bottomX = triangleX + (triangleWidth - smallKnobSize) / 2;  // Center it

    hpLsTiteSlider.setBounds(bottomX, bottomY, smallKnobSize, smallKnobSize);
    hpLsTiteLabel.setBounds(hpLsTiteSlider.getX(), hpLsTiteSlider.getBottom(), smallKnobSize, 12);

    // Low Pass layout
    auto lpBounds = lowPassGroup.getLocalBounds().reduced(6);

    // Calculate total triangle dimensions
    triangleWidth = bigKnobSize * 2;                   // Two big knobs side by side
    triangleHeight = bigKnobSize + smallKnobSize + 8;  // Top row + gap + bottom row

    // Center the triangle in the available space
    triangleX = lpBounds.getX() + (lpBounds.getWidth() - triangleWidth) / 2;
    triangleY = lpBounds.getY() + (lpBounds.getHeight() - triangleHeight) / 2;

    // Top row - two big knobs side by side
    topY = triangleY;
    lowpassSlider.setBounds(triangleX, topY, bigKnobSize, bigKnobSize);
    lowpassLabel.setBounds(lowpassSlider.getX(), lowpassSlider.getBottom(), bigKnobSize, 12);

    lpPolesSlider.setBounds(triangleX + bigKnobSize, topY, bigKnobSize, bigKnobSize);
    lpPolesLabel.setBounds(lpPolesSlider.getX(), lpPolesSlider.getBottom(), bigKnobSize, 12);

    // Bottom - small knob centered below
    bottomY = triangleY + bigKnobSize + 8;                      // 8px gap
    bottomX = triangleX + (triangleWidth - smallKnobSize) / 2;  // Center it

    lpSftHrdSlider.setBounds(bottomX, bottomY, smallKnobSize, smallKnobSize);
    lpSftHrdLabel.setBounds(lpSftHrdSlider.getX(), lpSftHrdSlider.getBottom(), smallKnobSize, 12);

    // High Shelf layout
    auto hsBounds = highShelfGroup.getLocalBounds().reduced(6);
    centerX = hsBounds.getX() + (hsBounds.getWidth() - bigKnobSize) / 2;
    centerY = hsBounds.getY() + (hsBounds.getHeight() - bigKnobSize) / 2;
    trebleSlider.setBounds(centerX, centerY, bigKnobSize, bigKnobSize);
    trebleLabel.setBounds(trebleSlider.getX(), trebleSlider.getBottom(), trebleSlider.getWidth(), 12);

    // High-Mid layout
    auto hmBounds = highMidGroup.getLocalBounds().reduced(6);

    // Calculate total triangle dimensions
    triangleWidth = bigKnobSize * 2;                   // Two big knobs side by side
    triangleHeight = bigKnobSize + smallKnobSize + 8;  // Top row + gap + bottom row

    // Center the triangle in the available space
    triangleX = hmBounds.getX() + (hmBounds.getWidth() - triangleWidth) / 2;
    triangleY = hmBounds.getY() + (hmBounds.getHeight() - triangleHeight) / 2;

    // Top row - two big knobs side by side
    topY = triangleY;
    hmFreqSlider.setBounds(triangleX, topY, bigKnobSize, bigKnobSize);
    hmFreqLabel.setBounds(hmFreqSlider.getX(), hmFreqSlider.getBottom(), bigKnobSize, 12);

    hmGainSlider.setBounds(triangleX + bigKnobSize, topY, bigKnobSize, bigKnobSize);
    hmGainLabel.setBounds(hmGainSlider.getX(), hmGainSlider.getBottom(), bigKnobSize, 12);

    // Bottom - small knob centered below
    bottomY = triangleY + bigKnobSize + 8;                      // 8px gap
    bottomX = triangleX + (triangleWidth - smallKnobSize) / 2;  // Center it

    hmResoSlider.setBounds(bottomX, bottomY, smallKnobSize, smallKnobSize);
    hmResoLabel.setBounds(hmResoSlider.getX(), hmResoSlider.getBottom(), smallKnobSize, 12);

    // Low Shelf layout
    auto lsBounds = lowShelfGroup.getLocalBounds().reduced(6);
    centerX = lsBounds.getX() + (lsBounds.getWidth() - bigKnobSize) / 2;
    centerY = lsBounds.getY() + (lsBounds.getHeight() - bigKnobSize) / 2;
    bassSlider.setBounds(centerX, centerY, bigKnobSize, bigKnobSize);
    bassLabel.setBounds(bassSlider.getX(), bassSlider.getBottom(), bassSlider.getWidth(), 12);
    eqBypassButton.setBounds(lsBounds.getRight() - 52, lsBounds.getBottom() - 26, 50, 20);

    // Now layout controls inside compressorGroup (your existing code)
    auto compBounds = compressorGroup.getLocalBounds().reduced(6);

    // Calculate total triangle dimensions for top section
    triangleWidth = bigKnobSize * 2;
    triangleHeight = bigKnobSize + smallKnobSize + 8;

    // Center the triangle
    triangleX = compBounds.getX() + (compBounds.getWidth() - triangleWidth) / 2;
    triangleY = compBounds.getY() + 10;  // Small offset from top

    // Top row - two big knobs
    topY = triangleY;
    pressureSlider.setBounds(triangleX, topY, bigKnobSize, bigKnobSize);
    pressureLabel.setBounds(pressureSlider.getX(), pressureSlider.getBottom(), bigKnobSize, 12);

    speedSlider.setBounds(triangleX + bigKnobSize, topY, bigKnobSize, bigKnobSize);
    speedLabel.setBounds(speedSlider.getX(), speedSlider.getBottom(), bigKnobSize, 12);

    // Bottom - small knob centered
    bottomY = triangleY + bigKnobSize + 8;
    bottomX = triangleX + (triangleWidth - smallKnobSize) / 2;

    mewinessSlider.setBounds(bottomX, bottomY, smallKnobSize, smallKnobSize);
    mewinessLabel.setBounds(mewinessSlider.getX(), mewinessSlider.getBottom(), smallKnobSize, 12);

    // GR Meter and button below
    centerX = compBounds.getX() + (compBounds.getWidth() - 20) / 2;
    grMeter.setBounds(centerX, mewinessSlider.getBottom() + 25, 20, compBounds.getBottom() - 60 - mewinessSlider.getBottom());

    compressorBypassButton.setBounds(compBounds.getRight() - 55, compBounds.getBottom() - 25, 50, 20);

    // Meters layout
    auto metersBounds = metersGroup.getLocalBounds().reduced(6);
    metersBounds.removeFromTop(18);
    auto meterArea = metersBounds.removeFromTop(100);

    // Split into two halves
    auto leftHalf = meterArea.removeFromLeft(meterArea.getWidth() / 2);
    auto rightHalf = meterArea;  // Remaining half

    // Center 18px meter in left half
    int leftCenterX = leftHalf.getX() + (leftHalf.getWidth() - 18) / 2;
    measuredMeterL.setBounds(leftCenterX, leftHalf.getY(), 18, leftHalf.getHeight());

    // Center 18px meter in right half
    int rightCenterX = rightHalf.getX() + (rightHalf.getWidth() - 18) / 2;
    measuredMeterR.setBounds(rightCenterX, rightHalf.getY(), 18, rightHalf.getHeight());

    vuMeterModeButton.setBounds(metersBounds.removeFromBottom(30).reduced(4, 2));

    // Gain layout
    auto gainBounds = gainGroup.getLocalBounds().reduced(6);
    gainBounds.removeFromTop(18);  // Space for group title

    // Reserve space for button at bottom
    masterBypassButton.setBounds(gainBounds.removeFromBottom(30).reduced(4, 2));

    // Use remaining space for sliders
    auto gainSliders = gainBounds.reduced(0, 4);  // Small vertical padding
    auto inputArea = gainSliders.removeFromLeft(gainSliders.getWidth() / 2).reduced(2);
    inputGainLabel.setBounds(inputArea.removeFromBottom(12));
    inputGainSlider.setBounds(inputArea);

    auto outputArea = gainSliders.reduced(2);
    outputGainLabel.setBounds(outputArea.removeFromBottom(12));
    outputGainSlider.setBounds(outputArea);
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

    auto currentPreset = presetManager.getCurrentPreset();
    if (!currentPreset.isEmpty())
    {
        int index = presets.indexOf(currentPreset);
        if (index >= 0)
        {
            presetComboBox.setSelectedId(index + 1, juce::dontSendNotification);
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