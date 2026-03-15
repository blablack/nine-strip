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

    int width = baseWidth;
    int height = baseHeight;

    if (auto* props = audioProcessor.getAppProperties().getUserSettings())
    {
        width = props->getIntValue("editorWidth", baseWidth);
        height = props->getIntValue("editorHeight", baseHeight);
    }

    constrainer.setFixedAspectRatio(static_cast<float>(baseWidth) / static_cast<float>(baseHeight));
    constrainer.setSizeLimits(baseWidth, baseHeight, baseWidth * 3, baseHeight * 3);
    setConstrainer(&constrainer);
    setResizable(false, true);

    setSize(width, height);

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
    setupGroupComponent(consoleSatGroup, consoleSatLabel, "CONSOLE");

    // Setup console type rotary knob
    consoleSatGroup.addAndMakeVisible(consoleTypeSlider);
    consoleTypeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    consoleTypeSlider.setRange(0, 4, 1);  // 5 choices (0-4), step size of 1
    consoleTypeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

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
    consoleTypeSlider.getProperties().set("noTextAround", true);

    consoleTypeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "consoleType", consoleTypeSlider);

    // Add value label below slider
    consoleSatGroup.addAndMakeVisible(consoleTypeValueLabel);
    consoleTypeValueLabel.setJustificationType(juce::Justification::centred);
    consoleTypeValueLabel.setText("Neve", juce::dontSendNotification);  // Initial value

    // Update label when slider changes
    consoleTypeSlider.onValueChange = [this]()
    {
        consoleTypeValueLabel.setText(consoleTypeSlider.getTextFromValue(consoleTypeSlider.getValue()),
                                      juce::dontSendNotification);
    };

    addRotaryKnob(consoleSatGroup, driveSlider, driveLabel, "drive", "Drive", juce::Colours::white.darker(), driveAttachment);

    consoleSatGroup.addAndMakeVisible(saturationBypassButton);
    saturationBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "saturationBypass", saturationBypassButton);
}

void NineStripProcessorEditor::setupFiltersSection()
{
    // High Pass Filter
    setupGroupComponent(highPassGroup, highPassLabel, "HIGH PASS");

    addRotaryKnob(highPassGroup, hipassSlider, hipassLabel, "hipass", "Freq", juce::Colours::green.darker(), hipassAttachment);
    addRotaryKnob(highPassGroup, hpLsTiteSlider, hpLsTiteLabel, "ls_tite", "Loose/Tight", juce::Colours::green.darker(),
                  hpLsTiteAttachment);
    addRotaryKnob(highPassGroup, hpPolesSlider, hpPolesLabel, "hp_poles", "Slope", juce::Colours::green.darker(),
                  hpPolesAttachment);

    // Low Pass Filter
    setupGroupComponent(lowPassGroup, lowPassLabel, "LOW PASS");

    addRotaryKnob(lowPassGroup, lowpassSlider, lowpassLabel, "lowpass", "Freq", juce::Colours::blue.darker(),
                  lowpassAttachment);
    addRotaryKnob(lowPassGroup, lpSftHrdSlider, lpSftHrdLabel, "lp_sft_hrd", "Soft/Hard", juce::Colours::blue.darker(),
                  lpSftHrdAttachment);
    addRotaryKnob(lowPassGroup, lpPolesSlider, lpPolesLabel, "lp_poles", "Slope", juce::Colours::blue.darker(),
                  lpPolesAttachment);

    lowPassGroup.addAndMakeVisible(filterBypassButton);
    filterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "filterBypass", filterBypassButton);
}

void NineStripProcessorEditor::setupEQSection()
{
    // High Shelf
    setupGroupComponent(highShelfGroup, highShelfLabel, "HIGH SHELF");

    addRotaryKnob(highShelfGroup, trebleSlider, trebleLabel, "treble", "Gain", juce::Colours::black, trebleAttachment);

    // High-Mid EQ
    setupGroupComponent(highMidGroup, highMidLabel, "HI-MID EQ");

    addRotaryKnob(highMidGroup, hmFreqSlider, hmFreqLabel, "hm_freq", "Freq", juce::Colours::white.darker(), hmFreqAttachment);
    addRotaryKnob(highMidGroup, hmGainSlider, hmGainLabel, "highmid", "Gain", juce::Colours::white.darker(), hmGainAttachment);
    addRotaryKnob(highMidGroup, hmResoSlider, hmResoLabel, "hm_reso", "Q", juce::Colours::white.darker(), hmResoAttachment);

    // Low Shelf
    setupGroupComponent(lowShelfGroup, lowShelfLabel, "LOW SHELF");

    addRotaryKnob(lowShelfGroup, bassSlider, bassLabel, "bass", "Gain", juce::Colours::black, bassAttachment);

    // EQ Bypass
    lowShelfGroup.addAndMakeVisible(eqBypassButton);
    eqBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(),
                                                                                                "eqBypass", eqBypassButton);
}

void NineStripProcessorEditor::setupDynamicsSection()
{
    setupGroupComponent(compressorGroup, compressorLabel, "DYNAMICS");

    addRotaryKnob(compressorGroup, pressureSlider, pressureLabel, "pressure", "Pressure", juce::Colours::red.darker(),
                  pressureAttachment);
    addRotaryKnob(compressorGroup, speedSlider, speedLabel, "speed", "Speed", juce::Colours::red.darker(), speedAttachment);
    addRotaryKnob(compressorGroup, mewinessSlider, mewinessLabel, "mewiness", "Mewiness", juce::Colours::red.darker(),
                  mewinessAttachment);

    compressorGroup.addAndMakeVisible(grMeter);

    compressorGroup.addAndMakeVisible(compressorBypassButton);
    compressorBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "compressorBypass", compressorBypassButton);
}

void NineStripProcessorEditor::setupMeters()
{
    addAndMakeVisible(metersGroup);
    metersGroup.setColour(juce::GroupComponent::outlineColourId, juce::Colours::transparentBlack);
    metersGroup.setColour(juce::GroupComponent::textColourId, juce::Colours::white);

    metersGroup.addAndMakeVisible(needleVUMeterL);
    metersGroup.addAndMakeVisible(needleVUMeterR);

    metersGroup.addAndMakeVisible(vuMeterInputButton);
    vuMeterInputButton.setClickingTogglesState(false);  // Add this!
    vuMeterInputButton.onClick = [this]()
    {
        // If already on, do nothing
        if (vuMeterInputButton.getToggleState()) return;

        // Turn this button on and the other off
        vuMeterInputButton.setToggleState(true, juce::dontSendNotification);
        vuMeterOutputButton.setToggleState(false, juce::dontSendNotification);
        audioProcessor.getAPVTS().getParameter("inputMeasured")->setValueNotifyingHost(1.0f);
    };

    // Setup Output button
    metersGroup.addAndMakeVisible(vuMeterOutputButton);
    vuMeterOutputButton.setClickingTogglesState(false);  // Add this!
    vuMeterOutputButton.onClick = [this]()
    {
        // If already on, do nothing
        if (vuMeterOutputButton.getToggleState()) return;

        // Turn this button on and the other off
        vuMeterOutputButton.setToggleState(true, juce::dontSendNotification);
        vuMeterInputButton.setToggleState(false, juce::dontSendNotification);
        audioProcessor.getAPVTS().getParameter("inputMeasured")->setValueNotifyingHost(0.0f);
    };

    // Initialize button states based on current parameter value
    bool isInputMode = audioProcessor.getAPVTS().getRawParameterValue("inputMeasured")->load() > 0.5f;
    vuMeterInputButton.setToggleState(isInputMode, juce::dontSendNotification);
    vuMeterOutputButton.setToggleState(!isInputMode, juce::dontSendNotification);

    // Listen for parameter changes
    audioProcessor.getAPVTS().addParameterListener("inputMeasured", this);
}

void NineStripProcessorEditor::setupGain()
{
    setupGroupComponent(gainGroup, gainLabel, "GAIN");

    gainGroup.addAndMakeVisible(inputGainSlider);
    inputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    inputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    inputGainSlider.setLookAndFeel(&faderSkeuomorphicLook);
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(),
                                                                                                 "inputGain", inputGainSlider);
    gainGroup.addAndMakeVisible(inputGainLabel);
    inputGainLabel.setText("Input", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);

    gainGroup.addAndMakeVisible(outputGainSlider);
    outputGainSlider.setSliderStyle(juce::Slider::LinearVertical);
    outputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outputGainSlider.setLookAndFeel(&faderSkeuomorphicLook);
    outputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "outputGain", outputGainSlider);
    gainGroup.addAndMakeVisible(outputGainLabel);
    outputGainLabel.setText("Output", juce::dontSendNotification);
    outputGainLabel.setJustificationType(juce::Justification::centred);

    gainGroup.addAndMakeVisible(masterBypassButton);
    masterBypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "masterBypass", masterBypassButton);
}

void NineStripProcessorEditor::addRotaryKnob(juce::Component& parent, CircularKnob& slider, juce::Label& label,
                                             const juce::String& paramID, const juce::String& labelText, juce::Colour knobColor,
                                             std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment)
{
    parent.addAndMakeVisible(slider);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    slider.setLookAndFeel(&knobSkeuomorphicLook);

    slider.setColour(juce::Slider::rotarySliderFillColourId, knobColor);

    attachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), paramID, slider);

    parent.addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
}

NineStripProcessorEditor::~NineStripProcessorEditor()
{
    audioProcessor.editorStateChanged(false);
    audioProcessor.getAPVTS().removeParameterListener("inputMeasured", this);
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
        bool isInputMode = newValue > 0.5f;
        vuMeterInputButton.setToggleState(isInputMode, juce::dontSendNotification);
        vuMeterOutputButton.setToggleState(!isInputMode, juce::dontSendNotification);
    }
}

void NineStripProcessorEditor::resized()
{
    if (auto* props = audioProcessor.getAppProperties().getUserSettings())
    {
        props->setValue("editorWidth", getWidth());
        props->setValue("editorHeight", getHeight());
    }

    // Window size
    auto bounds = getLocalBounds().reduced(8);

    // Knobs size

    int columnWidth = bounds.getWidth() / 5;
    int bigKnobSize = (columnWidth - 10) / 2;  // Account for padding
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
        Track(Px(getHeight() * 30 / baseHeight)),  // Row 1: Preset
        Track(Fr(9)),                              // Row 2: Top sections
        Track(Fr(10)),                             // Row 3: Bottom sections
        Track(Fr(10))                              // Row 4: Bottom sections
    };

    mainGrid.templateColumns = {
        Track(Fr(4)),  // Column 1: Console/Filters
        Track(Fr(4)),  // Column 2: EQ
        Track(Fr(5)),  // Column 3: Meters (wider)
        Track(Fr(5))   // Column 4: Dynamics/Gain
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
    auto presetBounds = presetPanel.getLocalBounds().reduced(40, 0);
    previousPresetButton.setBounds(presetBounds.removeFromLeft(40).reduced(2));
    deletePresetButton.setBounds(presetBounds.removeFromRight(60).reduced(2));
    savePresetButton.setBounds(presetBounds.removeFromRight(60).reduced(2));
    nextPresetButton.setBounds(presetBounds.removeFromRight(40).reduced(2));
    presetComboBox.setBounds(presetBounds.reduced(2));
}

void NineStripProcessorEditor::layoutConsoleSection(int bigKnobSize)
{
    auto consoleBounds = consoleSatGroup.getLocalBounds().reduced(6);

    consoleSatLabel.setBounds(consoleBounds.removeFromTop(20));

    const int horizontalSpacing = 16;
    const int shadowPadding = 40;

    int totalWidth = bigKnobSize * 2 + horizontalSpacing;
    int startX = consoleBounds.getCentreX() - (totalWidth / 2);
    int knobY = consoleBounds.getY() + (consoleBounds.getHeight() - bigKnobSize - 40) / 2;

    // Console type knob - expand bounds for shadow
    consoleTypeSlider.setBounds(startX - shadowPadding / 2, knobY - shadowPadding / 2, bigKnobSize + shadowPadding,
                                bigKnobSize + shadowPadding);
    consoleTypeValueLabel.setBounds(startX, consoleTypeSlider.getBottom() - shadowPadding / 2, bigKnobSize, 18);

    // Drive knob - expand bounds for shadow
    driveSlider.setBounds(startX + bigKnobSize + horizontalSpacing - shadowPadding / 2, knobY - shadowPadding / 2,
                          bigKnobSize + shadowPadding, bigKnobSize + shadowPadding);
    driveLabel.setBounds(startX + bigKnobSize + horizontalSpacing, driveSlider.getBottom() - shadowPadding / 2, bigKnobSize,
                         18);

    // Bypass button at bottom-right
    saturationBypassButton.setBounds(consoleBounds.getRight() - 52, consoleBounds.getBottom() - 30, 50, 20);
}

void NineStripProcessorEditor::layoutFiltersSection(int bigKnobSize, int smallKnobSize)
{
    // Calculate proportional values based on group bounds
    auto groupBounds = highPassGroup.getLocalBounds();
    int margin = groupBounds.getWidth() * 0.02f;
    int headerHeight = groupBounds.getHeight() * 0.15f;

    // High Pass layout
    auto hpBounds = highPassGroup.getLocalBounds().reduced(margin);
    highPassLabel.setBounds(hpBounds.removeFromTop(headerHeight));
    layoutTriangleKnobs(hpBounds, hipassSlider, hipassLabel, hpPolesSlider, hpPolesLabel, hpLsTiteSlider, hpLsTiteLabel,
                        bigKnobSize, smallKnobSize);

    // Low Pass layout
    auto lpBounds = lowPassGroup.getLocalBounds().reduced(margin);
    lowPassLabel.setBounds(lpBounds.removeFromTop(headerHeight));
    layoutTriangleKnobs(lpBounds, lowpassSlider, lowpassLabel, lpPolesSlider, lpPolesLabel, lpSftHrdSlider, lpSftHrdLabel,
                        bigKnobSize, smallKnobSize);

    // Bypass button - proportional sizing
    int buttonWidth = lpBounds.getWidth() * 0.2f;
    int buttonHeight = lpBounds.getHeight() * 0.08f;
    int buttonMargin = margin;

    filterBypassButton.setBounds(lpBounds.getRight() - 52, lpBounds.getBottom() - 20, 50, 20);
}

void NineStripProcessorEditor::layoutEQSection(int bigKnobSize, int smallKnobSize)
{
    // Calculate proportional values
    auto groupBounds = highShelfGroup.getLocalBounds();
    int margin = groupBounds.getWidth() * 0.02f;
    int headerHeightLarge = groupBounds.getHeight() * 0.15f;
    int headerHeightSmall = groupBounds.getHeight() * 0.08f;
    int labelHeight = groupBounds.getHeight() * 0.08f;

    // High Shelf layout
    auto hsBounds = highShelfGroup.getLocalBounds().reduced(6);
    highShelfLabel.setBounds(hsBounds.removeFromTop(20));

    layoutCenteredKnob(hsBounds, trebleSlider, trebleLabel, bigKnobSize);

    // High-Mid layout
    auto hmBounds = highMidGroup.getLocalBounds().reduced(margin);
    highMidLabel.setBounds(hmBounds.removeFromTop(headerHeightLarge));
    layoutTriangleKnobs(hmBounds, hmFreqSlider, hmFreqLabel, hmGainSlider, hmGainLabel, hmResoSlider, hmResoLabel, bigKnobSize,
                        smallKnobSize);

    // Low Shelf layout
    auto lsBounds = lowShelfGroup.getLocalBounds().reduced(margin);
    lowShelfLabel.setBounds(lsBounds.removeFromTop(headerHeightLarge));
    layoutCenteredKnob(lsBounds, bassSlider, bassLabel, bigKnobSize);

    // Bypass button - proportional sizing
    int buttonWidth = lsBounds.getWidth() * 0.2f;
    int buttonHeight = lsBounds.getHeight() * 0.08f;
    int buttonMargin = margin;

    eqBypassButton.setBounds(lsBounds.getRight() - 52, lsBounds.getBottom() - 20, 50, 20);
}

void NineStripProcessorEditor::layoutDynamicsSection(int bigKnobSize, int smallKnobSize)
{
    // Calculate proportional values
    auto groupBounds = compressorGroup.getLocalBounds();
    int margin = jmax(4, static_cast<int>(groupBounds.getWidth() * 0.02f));
    int headerHeight = jmax(20, static_cast<int>(groupBounds.getHeight() * 0.15f));

    auto compBounds = compressorGroup.getLocalBounds().reduced(margin);
    compressorLabel.setBounds(compBounds.removeFromTop(headerHeight));

    auto triangleBounds = compBounds.withTrimmedTop(0);
    // Triangle knobs at top (not centered vertically)
    layoutTriangleKnobs(triangleBounds, pressureSlider, pressureLabel, speedSlider, speedLabel, mewinessSlider, mewinessLabel,
                        bigKnobSize, smallKnobSize, false);

    // Calculate proportional spacing for meter area
    int knobBottomSpacing = jmax(20, static_cast<int>(compBounds.getHeight() * 0.08f));  // 8% spacing after knobs
    int buttonAreaHeight = jmax(25, static_cast<int>(compBounds.getHeight() * 0.1f));    // 10% for button area

    int topY = mewinessSlider.getBottom() + knobBottomSpacing;
    int bottomY = compBounds.getBottom() - buttonAreaHeight;
    int availableHeight = bottomY - topY;

    // Use proportional width for meter (e.g., 80% of available width)
    int meterMaxWidth = jmax(100, static_cast<int>(compBounds.getWidth() * 0.8f));

    // Create area for the meter with proportional height
    juce::Rectangle<int> meterArea(compBounds.getX(), topY, compBounds.getWidth(),
                                   jmax(50, static_cast<int>(availableHeight * 0.75f))  // Use 75% of available height
    );

    // Constrain to aspect ratio and center
    auto grMeterBounds = constrainToAspectRatio(meterArea, grMeter.getAspectRatio());

    // Constrain width if needed
    if (grMeterBounds.getWidth() > meterMaxWidth)
    {
        int newHeight = static_cast<int>(meterMaxWidth / grMeter.getAspectRatio());
        grMeterBounds.setSize(meterMaxWidth, newHeight);
    }

    // Center both horizontally and vertically in available space
    grMeterBounds.setCentre(compBounds.getCentreX(), topY + availableHeight / 2);
    grMeter.setBounds(grMeterBounds);

    // Bypass button at bottom-right
    compressorBypassButton.setBounds(compBounds.getRight() - 55, compBounds.getBottom() - 20, 50, 20);
}

void NineStripProcessorEditor::layoutMeters()
{
    auto metersBounds = metersGroup.getLocalBounds().reduced(2);

    auto buttonArea = metersBounds.removeFromBottom(30).reduced(4, 2);

    // Calculate button dimensions - two buttons side-by-side with spacing
    const int spacing = 8;
    const int buttonWidth = (buttonArea.getWidth() - spacing) / 4;

    // Center the button group horizontally
    const int totalGroupWidth = buttonWidth * 2 + spacing;
    const int startX = buttonArea.getCentreX() - (totalGroupWidth / 2);
    const int buttonY = buttonArea.getY();
    const int buttonHeight = buttonArea.getHeight();

    vuMeterInputButton.setBounds(startX, buttonY, buttonWidth, buttonHeight);
    vuMeterOutputButton.setBounds(startX + buttonWidth + spacing, buttonY, buttonWidth, buttonHeight);

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
    auto gainSliders = gainBounds.reduced(0, 4);
    gainSliders.removeFromBottom(20);

    // Calculate fader width with shadow padding
    const int shadowPadding = 30;  // Extra space for fader shadow
    const int faderWidth = juce::jlimit(20, 50, gainSliders.getWidth() / 4);

    auto inputArea = gainSliders.removeFromLeft(gainSliders.getWidth() / 2).reduced(2);
    inputGainLabel.setBounds(inputArea.removeFromBottom(20));

    // Center the fader with shadow padding
    const int inputCenterX = inputArea.getX() + (inputArea.getWidth() - faderWidth - shadowPadding) / 2;
    inputGainSlider.setBounds(inputCenterX, inputArea.getY(), faderWidth + shadowPadding, inputArea.getHeight());

    auto outputArea = gainSliders.reduced(2);
    outputGainLabel.setBounds(outputArea.removeFromBottom(20));

    // Center the fader with shadow padding
    const int outputCenterX = outputArea.getX() + (outputArea.getWidth() - faderWidth - shadowPadding) / 2;
    outputGainSlider.setBounds(outputCenterX, outputArea.getY(), faderWidth + shadowPadding, outputArea.getHeight());
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

void NineStripProcessorEditor::setupGroupComponent(juce::Component& group, juce::Label& label, const juce::String& title)
{
    addAndMakeVisible(group);

    group.addAndMakeVisible(label);
    label.setText(title, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
}

void NineStripProcessorEditor::layoutTriangleKnobs(juce::Rectangle<int> bounds, CircularKnob& topLeft,
                                                   juce::Label& topLeftLabel, CircularKnob& topRight,
                                                   juce::Label& topRightLabel, CircularKnob& bottom, juce::Label& bottomLabel,
                                                   int bigKnobSize, int smallKnobSize, bool centerVertically)
{
    const int horizontalSpacing = 16;
    const int verticalSpacing = 20;
    const int labelPadding = 50;   // Extra width on each slider for min/max labels
    const int shadowPadding = 40;  // Extra space for shadow (30px radius + offset)

    int triangleWidth = bigKnobSize * 2 + horizontalSpacing;
    int triangleHeight = bigKnobSize + smallKnobSize + verticalSpacing;

    int triangleX = bounds.getX() + (bounds.getWidth() - triangleWidth) / 2;
    int triangleY = centerVertically ? bounds.getY() + (bounds.getHeight() - triangleHeight) / 2 - 20 : bounds.getY();

    // Top left - add shadow padding to all sides
    topLeft.setBounds(triangleX - labelPadding / 2 - shadowPadding / 2, triangleY - shadowPadding / 2,
                      bigKnobSize + labelPadding + shadowPadding, bigKnobSize + shadowPadding);
    topLeftLabel.setBounds(topLeft.getX() + labelPadding / 2 + shadowPadding / 2, topLeft.getBottom() - shadowPadding / 2,
                           bigKnobSize, 20);

    // Top right - add shadow padding to all sides
    topRight.setBounds(triangleX + bigKnobSize + horizontalSpacing - labelPadding / 2 - shadowPadding / 2,
                       triangleY - shadowPadding / 2, bigKnobSize + labelPadding + shadowPadding, bigKnobSize + shadowPadding);
    topRightLabel.setBounds(topRight.getX() + labelPadding / 2 + shadowPadding / 2, topRight.getBottom() - shadowPadding / 2,
                            bigKnobSize, 20);

    // Bottom - small knob with shadow padding
    int bottomY = triangleY + bigKnobSize + verticalSpacing;
    int bottomX = triangleX + (triangleWidth - smallKnobSize) / 2;

    bottom.setBounds(bottomX - labelPadding / 2 - shadowPadding / 2, bottomY - shadowPadding / 2,
                     smallKnobSize + labelPadding + shadowPadding, smallKnobSize + shadowPadding);
    bottomLabel.setBounds(bottom.getX() + labelPadding / 2 + shadowPadding / 2, bottom.getBottom() - shadowPadding / 2,
                          smallKnobSize, 20);
}

void NineStripProcessorEditor::layoutCenteredKnob(juce::Rectangle<int> bounds, CircularKnob& knob, juce::Label& label,
                                                  int knobSize)
{
    const int labelPadding = 50;
    const int shadowPadding = 40;  // Extra space for shadow (30px radius + offset)

    int centerX = bounds.getX() + (bounds.getWidth() - knobSize) / 2;
    int centerY = bounds.getY() + (bounds.getHeight() - knobSize - 40) / 2;

    knob.setBounds(centerX - labelPadding / 2 - shadowPadding / 2, centerY - shadowPadding / 2,
                   knobSize + labelPadding + shadowPadding, knobSize + shadowPadding);
    label.setBounds(knob.getX() + labelPadding / 2 + shadowPadding / 2, knob.getBottom() - shadowPadding / 2, knobSize, 20);
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