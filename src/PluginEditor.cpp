#include "PluginEditor.h"

#include "PluginProcessor.h"
#include "juce_graphics/juce_graphics.h"

NineStripProcessorEditor::NineStripProcessorEditor(NineStripProcessor& p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      needleVUMeterL([&p]() { return p.getMeasuredLevelL(); }, NeedleVUMeter::MeterType::Level),
      needleVUMeterR([&p]() { return p.getMeasuredLevelR(); }, NeedleVUMeter::MeterType::Level),
      grMeter([&p]() { return p.getGainReduction(); }, NeedleVUMeter::MeterType::GainReduction)
{
    audioProcessor.editorStateChanged(true);

    const int baseWidth = 800;
    const int baseHeight = 600;
    setSize(baseWidth, baseHeight);
    constrainer.setFixedAspectRatio(static_cast<float>(baseWidth) / static_cast<float>(baseHeight));
    constrainer.setSizeLimits(baseWidth, baseHeight, baseWidth * 3, baseHeight * 3);
    setConstrainer(&constrainer);
    setResizable(false, true);

    setupPresetPanel();
    setupConsoleSection();
    setupFiltersSection();
    setupEQSection();
    setupDynamicsSection();
    setupMeters();
    setupGain();

    updatePresetComboBox();
}

void NineStripProcessorEditor::setupPresetPanel()
{
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
}

void NineStripProcessorEditor::setupConsoleSection()
{
    setupGroupComponent(consoleSatGroup, "CONSOLE");

    // Setup console type rotary knob
    consoleSatGroup.addAndMakeVisible(consoleTypeSlider);
    consoleTypeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    consoleTypeSlider.setRange(0, 4, 1);  // 5 choices (0-4), step size of 1
    consoleTypeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);

    // Map numeric values to text labels
    consoleTypeSlider.textFromValueFunction = [](double value)
    {
        static const juce::StringArray types{"Neve", "API", "SSL", "Teac", "Mackie"};
        return types[static_cast<int>(value)];
    };

    consoleTypeSlider.valueFromTextFunction = [](const juce::String& text)
    {
        static const juce::StringArray types{"Neve", "API", "SSL", "Teac", "Mackie"};
        return static_cast<double>(types.indexOf(text));
    };

    consoleTypeSlider.setLookAndFeel(&knobSkeuomorphicLook);
    consoleTypeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);

    consoleTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "consoleType", consoleTypeSlider);

    addRotaryKnob(consoleSatGroup, driveSlider, driveLabel, "drive", "Drive", juce::Colours::white.darker(), driveAttachment);

    consoleSatGroup.addAndMakeVisible(saturationBypassButton);
    saturationBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "saturationBypass", saturationBypassButton);
}

void NineStripProcessorEditor::setupFiltersSection()
{
    // High Pass Filter
    setupGroupComponent(highPassGroup, "HIGH PASS");

    addRotaryKnob(highPassGroup, hipassSlider, hipassLabel, "hipass", "Freq", juce::Colours::green.darker(), hipassAttachment);
    addRotaryKnob(highPassGroup, hpLsTiteSlider, hpLsTiteLabel, "ls_tite", "Ls/Tite", juce::Colours::green.darker(),
                  hpLsTiteAttachment);
    addRotaryKnob(highPassGroup, hpPolesSlider, hpPolesLabel, "hp_poles", "Slope", juce::Colours::green.darker(),
                  hpPolesAttachment);

    // Low Pass Filter
    setupGroupComponent(lowPassGroup, "LOW PASS");

    addRotaryKnob(lowPassGroup, lowpassSlider, lowpassLabel, "lowpass", "Freq", juce::Colours::blue.darker(),
                  lowpassAttachment);
    addRotaryKnob(lowPassGroup, lpSftHrdSlider, lpSftHrdLabel, "lp_sft_hrd", "Soft/Hard", juce::Colours::blue.darker(),
                  lpSftHrdAttachment);
    addRotaryKnob(lowPassGroup, lpPolesSlider, lpPolesLabel, "lp_poles", "Slope", juce::Colours::blue.darker(),
                  lpPolesAttachment);
}

void NineStripProcessorEditor::setupEQSection()
{
    // High Shelf
    setupGroupComponent(highShelfGroup, "HIGH SHELF");

    addRotaryKnob(highShelfGroup, trebleSlider, trebleLabel, "treble", "Gain", juce::Colours::black, trebleAttachment);

    // High-Mid EQ
    setupGroupComponent(highMidGroup, "HI-MID EQ");

    addRotaryKnob(highMidGroup, hmFreqSlider, hmFreqLabel, "hm_freq", "Freq", juce::Colours::white.darker(), hmFreqAttachment);
    addRotaryKnob(highMidGroup, hmGainSlider, hmGainLabel, "highmid", "Gain", juce::Colours::white.darker(), hmGainAttachment);
    addRotaryKnob(highMidGroup, hmResoSlider, hmResoLabel, "hm_reso", "Q", juce::Colours::white.darker(), hmResoAttachment);

    // Low Shelf
    setupGroupComponent(lowShelfGroup, "LOW SHELF");

    addRotaryKnob(lowShelfGroup, bassSlider, bassLabel, "bass", "Gain", juce::Colours::black, bassAttachment);

    // EQ Bypass
    lowShelfGroup.addAndMakeVisible(eqBypassButton);
    eqBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts,
                                                                                                "eqBypass", eqBypassButton);
}

void NineStripProcessorEditor::setupDynamicsSection()
{
    setupGroupComponent(compressorGroup, "DYNAMICS");

    addRotaryKnob(compressorGroup, pressureSlider, pressureLabel, "pressure", "Pressure", juce::Colours::white.darker(),
                  pressureAttachment);
    addRotaryKnob(compressorGroup, speedSlider, speedLabel, "speed", "Speed", juce::Colours::white.darker(), speedAttachment);
    addRotaryKnob(compressorGroup, mewinessSlider, mewinessLabel, "mewiness", "Mewiness", juce::Colours::white.darker(),
                  mewinessAttachment);

    compressorGroup.addAndMakeVisible(grMeter);

    compressorGroup.addAndMakeVisible(compressorBypassButton);
    compressorBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "compressorBypass", compressorBypassButton);
}

void NineStripProcessorEditor::setupMeters()
{
    addAndMakeVisible(metersGroup);
    metersGroup.setColour(juce::GroupComponent::outlineColourId, juce::Colours::transparentBlack);
    metersGroup.setColour(juce::GroupComponent::textColourId, juce::Colours::white);

    metersGroup.addAndMakeVisible(needleVUMeterL);
    metersGroup.addAndMakeVisible(needleVUMeterR);

    metersGroup.addAndMakeVisible(vuMeterModeButton);
    vuMeterModeButton.setClickingTogglesState(true);
    vuMeterModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "inputMeasured", vuMeterModeButton);
    audioProcessor.apvts.addParameterListener("inputMeasured", this);
    vuMeterModeButton.setButtonText((audioProcessor.apvts.getRawParameterValue("inputMeasured")->load() > 0.5f) ? "Input"
                                                                                                                : "Output");
}

void NineStripProcessorEditor::setupGain()
{
    setupGroupComponent(gainGroup, "GAIN");

    gainGroup.addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    inputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inputGainSlider.setLookAndFeel(&faderSkeuomorphicLook);
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts,
                                                                                                 "inputGain", inputGainSlider);
    gainGroup.addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);

    gainGroup.addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    outputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputGainSlider.setLookAndFeel(&faderSkeuomorphicLook);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.apvts, "outputGain", outputGainSlider);
    gainGroup.addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::centred);

    gainGroup.addAndMakeVisible(masterBypassButton);
    masterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.apvts, "masterBypass", masterBypassButton);
}

void NineStripProcessorEditor::addRotaryKnob(juce::Component& parent, juce::Slider& slider, juce::Label& label,
                                             const juce::String& paramID, const juce::String& labelText, juce::Colour knobColor,
                                             std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
{
    parent.addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    slider.setLookAndFeel(&knobSkeuomorphicLook);

    slider.setColour(juce::Slider::rotarySliderFillColourId, knobColor);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, paramID, slider);

    parent.addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
}

NineStripProcessorEditor::~NineStripProcessorEditor()
{
    audioProcessor.editorStateChanged(false);
    audioProcessor.apvts.removeParameterListener("inputMeasured", this);
}

void NineStripProcessorEditor::paint(juce::Graphics& g)
{
    auto backgroundImage = juce::ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImage(backgroundImage, getLocalBounds().toFloat());
}

void NineStripProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "inputMeasured")
    {
        juce::MessageManager::callAsync([this, newValue]()
                                        { vuMeterModeButton.setButtonText(newValue > 0.5f ? "Input" : "Output"); });
    }
}

void NineStripProcessorEditor::resized()
{
    // Window size
    auto bounds = getLocalBounds().reduced(8);

    // Knobs size

    int columnWidth = bounds.getWidth() / 6;   // 4 columns
    int bigKnobSize = (columnWidth - 20) / 2;  // Account for padding
    int smallKnobSize = bigKnobSize * 0.8f;

    setupMainGrid(bounds);
    layoutPresetPanel();
    layoutConsoleSection(bigKnobSize);
    layoutFiltersSection(bigKnobSize, smallKnobSize);
    layoutEQSection(bigKnobSize, smallKnobSize);
    layoutDynamicsSection(bigKnobSize, smallKnobSize);
    layoutMeters();
    layoutGain();
}

void NineStripProcessorEditor::setupMainGrid(juce::Rectangle<int> bounds)
{
    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Px = juce::Grid::Px;

    // ========== MAIN GRID: 4 columns + preset row ==========
    mainGrid.templateRows = {
        Track(Px(getHeight() * 40 / 600)),  // Row 1: Preset
        Track(Fr(1)),                       // Row 2: Top sections
        Track(Fr(1)),                       // Row 3: Bottom sections
        Track(Fr(1))                        // Row 4: Bottom sections
    };

    mainGrid.templateColumns = {
        Track(Fr(5)),  // Column 1: Console/Filters
        Track(Fr(5)),  // Column 2: EQ
        Track(Fr(6)),  // Column 3: Meters (wider)
        Track(Fr(4))   // Column 4: Dynamics/Gain
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
}

void NineStripProcessorEditor::layoutPresetPanel()
{
    auto presetBounds = presetPanel.getLocalBounds().reduced(4);
    previousPresetButton.setBounds(presetBounds.removeFromLeft(40).reduced(2));
    deletePresetButton.setBounds(presetBounds.removeFromRight(60).reduced(2));
    savePresetButton.setBounds(presetBounds.removeFromRight(60).reduced(2));
    nextPresetButton.setBounds(presetBounds.removeFromRight(40).reduced(2));
    presetComboBox.setBounds(presetBounds.reduced(2));
}

void NineStripProcessorEditor::layoutConsoleSection(int bigKnobSize)
{
    auto consoleBounds = consoleSatGroup.getLocalBounds().reduced(6);

    const int horizontalSpacing = 16;
    int totalWidth = bigKnobSize * 2 + horizontalSpacing;
    int startX = consoleBounds.getCentreX() - (totalWidth / 2);
    int knobY = consoleBounds.getY() + (consoleBounds.getHeight() - bigKnobSize - 40) / 2;  // Center vertically

    // Console type knob on left
    consoleTypeSlider.setBounds(startX, knobY, bigKnobSize, bigKnobSize + 20);

    // Drive knob on right
    driveSlider.setBounds(startX + bigKnobSize + horizontalSpacing, knobY, bigKnobSize, bigKnobSize);
    driveLabel.setBounds(driveSlider.getX(), driveSlider.getBottom(), bigKnobSize, 18);

    // Bypass button at bottom-right
    saturationBypassButton.setBounds(consoleBounds.getRight() - 52, consoleBounds.getBottom() - 26, 50, 20);
}

void NineStripProcessorEditor::layoutFiltersSection(int bigKnobSize, int smallKnobSize)
{
    // High Pass layout
    auto hpBounds = highPassGroup.getLocalBounds().reduced(6);
    layoutTriangleKnobs(hpBounds, hipassSlider, hipassLabel, hpPolesSlider, hpPolesLabel, hpLsTiteSlider, hpLsTiteLabel,
                        bigKnobSize, smallKnobSize);

    // Low Pass layout
    auto lpBounds = lowPassGroup.getLocalBounds().reduced(6);
    layoutTriangleKnobs(lpBounds, lowpassSlider, lowpassLabel, lpPolesSlider, lpPolesLabel, lpSftHrdSlider, lpSftHrdLabel,
                        bigKnobSize, smallKnobSize);
}

void NineStripProcessorEditor::layoutEQSection(int bigKnobSize, int smallKnobSize)
{
    // High Shelf layout
    auto hsBounds = highShelfGroup.getLocalBounds().reduced(6);
    layoutCenteredKnob(hsBounds, trebleSlider, trebleLabel, bigKnobSize);

    // High-Mid layout
    auto hmBounds = highMidGroup.getLocalBounds().reduced(6);
    layoutTriangleKnobs(hmBounds, hmFreqSlider, hmFreqLabel, hmGainSlider, hmGainLabel, hmResoSlider, hmResoLabel, bigKnobSize,
                        smallKnobSize);

    // Low Shelf layout
    auto lsBounds = lowShelfGroup.getLocalBounds().reduced(6);
    layoutCenteredKnob(lsBounds, bassSlider, bassLabel, bigKnobSize);

    // Bypass button at bottom-right
    eqBypassButton.setBounds(lsBounds.getRight() - 52, lsBounds.getBottom() - 26, 50, 20);
}

void NineStripProcessorEditor::layoutDynamicsSection(int bigKnobSize, int smallKnobSize)
{
    auto compBounds = compressorGroup.getLocalBounds().reduced(6);

    auto triangleBounds = compBounds.withTrimmedTop(30);
    // Triangle knobs at top (not centered vertically)
    layoutTriangleKnobs(triangleBounds, pressureSlider, pressureLabel, speedSlider, speedLabel, mewinessSlider, mewinessLabel,
                        bigKnobSize, smallKnobSize, false);

    // Define available area for GR meter
    int topY = mewinessSlider.getBottom() + 25;
    int bottomY = compBounds.getBottom() - 30;  // Leave room for bypass button
    int availableHeight = bottomY - topY;

    // Create centered rectangle for the meter
    juce::Rectangle<int> meterArea(compBounds.getX(), topY, compBounds.getWidth(), static_cast<int>(availableHeight * 0.7));

    // Constrain to aspect ratio and center horizontally
    auto grMeterBounds = constrainToAspectRatio(meterArea, grMeter.getAspectRatio());
    grMeterBounds.setCentre(compBounds.getCentreX(), grMeterBounds.getCentreY());
    grMeter.setBounds(grMeterBounds);

    // Bypass button at bottom-right
    compressorBypassButton.setBounds(compBounds.getRight() - 55, compBounds.getBottom() - 25, 50, 20);
}

void NineStripProcessorEditor::layoutMeters()
{
    auto metersBounds = metersGroup.getLocalBounds().reduced(6);
    metersBounds.removeFromTop(2);

    vuMeterModeButton.setBounds(metersBounds.removeFromBottom(30).reduced(4, 2));

    auto meterArea = metersBounds;

    // Split into left and right channels
    auto leftChannel = meterArea.removeFromLeft(meterArea.getWidth() / 2).reduced(2);
    auto rightChannel = meterArea.reduced(2);

    // Left channel: needle on left, bar on right
    auto leftNeedle = leftChannel.removeFromLeft(leftChannel.getWidth() * 1.0f);
    // Constrain to aspect ratio
    leftNeedle = constrainToAspectRatio(leftNeedle, needleVUMeterL.getAspectRatio());
    needleVUMeterL.setBounds(leftNeedle);

    // Right channel: needle on left, bar on right
    auto rightNeedle = rightChannel.removeFromLeft(rightChannel.getWidth() * 1.0f);
    // Constrain to aspect ratio
    rightNeedle = constrainToAspectRatio(rightNeedle, needleVUMeterR.getAspectRatio());
    needleVUMeterR.setBounds(rightNeedle);
}

juce::Rectangle<int> NineStripProcessorEditor::constrainToAspectRatio(juce::Rectangle<int> bounds, float aspectRatio)
{
    float currentAspectRatio = static_cast<float>(bounds.getWidth()) / static_cast<float>(bounds.getHeight());

    if (currentAspectRatio > aspectRatio)
    {
        // Too wide - reduce width, center horizontally
        int newWidth = static_cast<int>(bounds.getHeight() * aspectRatio);
        int xOffset = (bounds.getWidth() - newWidth) / 2;
        return bounds.withWidth(newWidth).withX(bounds.getX() + xOffset);
    }
    else
    {
        // Too tall - reduce height, center vertically
        int newHeight = static_cast<int>(bounds.getWidth() / aspectRatio);
        int yOffset = (bounds.getHeight() - newHeight) / 2;
        return bounds.withHeight(newHeight).withY(bounds.getY() + yOffset);
    }
}

void NineStripProcessorEditor::layoutGain()
{
    auto gainBounds = gainGroup.getLocalBounds().reduced(6);
    gainBounds.removeFromTop(18);  // Space for group title

    // Reserve space for button at bottom
    masterBypassButton.setBounds(gainBounds.removeFromBottom(30).reduced(4, 2));

    // Use remaining space for sliders
    auto gainSliders = gainBounds.reduced(0, 4);  // Small vertical padding
    gainSliders.removeFromBottom(20);

    // Calculate fader width based on available space - scale between 20-50px
    const int faderWidth = juce::jlimit(20, 50, gainSliders.getWidth() / 4);

    auto inputArea = gainSliders.removeFromLeft(gainSliders.getWidth() / 2).reduced(2);
    inputGainLabel.setBounds(inputArea.removeFromBottom(20));

    // Center the fader horizontally in its area
    const int inputCenterX = inputArea.getX() + (inputArea.getWidth() - faderWidth) / 2;
    inputGainSlider.setBounds(inputCenterX, inputArea.getY(), faderWidth, inputArea.getHeight());

    auto outputArea = gainSliders.reduced(2);
    outputGainLabel.setBounds(outputArea.removeFromBottom(20));

    // Center the fader horizontally in its area
    const int outputCenterX = outputArea.getX() + (outputArea.getWidth() - faderWidth) / 2;
    outputGainSlider.setBounds(outputCenterX, outputArea.getY(), faderWidth, outputArea.getHeight());
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

void NineStripProcessorEditor::setupGroupComponent(juce::GroupComponent& group, const juce::String& title)
{
    addAndMakeVisible(group);
    group.setText(title);
    group.setTextLabelPosition(juce::Justification::centredTop);
    group.setColour(juce::GroupComponent::outlineColourId, juce::Colours::transparentBlack);
    group.setColour(juce::GroupComponent::textColourId, juce::Colours::white);
}

void NineStripProcessorEditor::layoutTriangleKnobs(juce::Rectangle<int> bounds, juce::Slider& topLeft,
                                                   juce::Label& topLeftLabel, juce::Slider& topRight,
                                                   juce::Label& topRightLabel, juce::Slider& bottom, juce::Label& bottomLabel,
                                                   int bigKnobSize, int smallKnobSize, bool centerVertically)
{
    const int horizontalSpacing = 16;
    const int verticalSpacing = 32;
    const int labelPadding = 50;  // Extra width on each slider for min/max labels

    int triangleWidth = bigKnobSize * 2 + horizontalSpacing;
    int triangleHeight = bigKnobSize + smallKnobSize + verticalSpacing;

    int triangleX = bounds.getX() + (bounds.getWidth() - triangleWidth) / 2;
    int triangleY = centerVertically ? bounds.getY() + (bounds.getHeight() - triangleHeight) / 2 : bounds.getY() + 10;

    // Top row - add labelPadding to width, offset x by half
    topLeft.setBounds(triangleX - labelPadding / 2, triangleY, bigKnobSize + labelPadding, bigKnobSize);
    topLeftLabel.setBounds(topLeft.getX() + labelPadding / 2, topLeft.getBottom(), bigKnobSize, 20);

    topRight.setBounds(triangleX + bigKnobSize + horizontalSpacing - labelPadding / 2, triangleY, bigKnobSize + labelPadding,
                       bigKnobSize);
    topRightLabel.setBounds(topRight.getX() + labelPadding / 2, topRight.getBottom(), bigKnobSize, 20);

    // Bottom - small knob with padding
    int bottomY = triangleY + bigKnobSize + verticalSpacing;
    int bottomX = triangleX + (triangleWidth - smallKnobSize) / 2;

    bottom.setBounds(bottomX - labelPadding / 2, bottomY, smallKnobSize + labelPadding, smallKnobSize);
    bottomLabel.setBounds(bottom.getX() + labelPadding / 2, bottom.getBottom(), smallKnobSize, 20);
}

void NineStripProcessorEditor::layoutCenteredKnob(juce::Rectangle<int> bounds, juce::Slider& knob, juce::Label& label,
                                                  int knobSize)
{
    const int labelPadding = 50;

    int centerX = bounds.getX() + (bounds.getWidth() - knobSize) / 2;
    int centerY = bounds.getY() + (bounds.getHeight() - knobSize) / 2;

    knob.setBounds(centerX - labelPadding / 2, centerY, knobSize + labelPadding, knobSize);
    label.setBounds(knob.getX() + labelPadding / 2, knob.getBottom(), knobSize, 20);
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

        alert->enterModalState(true,  // take keyboard focus
                               juce::ModalCallbackFunction::create(
                                   [this, alert](int result)  // Capture alert ptr for getText
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