#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "ui/CircularKnob.h"
#include "ui/FaderLookAndFeel.h"
#include "ui/FineControlSlider.h"
#include "ui/GlowButton.h"
#include "ui/KnobLookAndFeel.h"
#include "ui/NeedleVUMeter.h"
#include "ui/ScaledLookAndFeel.h"

class NineStripProcessorEditor : public juce::AudioProcessorEditor,
                                 public juce::ComboBox::Listener,
                                 public juce::Button::Listener,
                                 public juce::AudioProcessorValueTreeState::Listener
{
   public:
    explicit NineStripProcessorEditor(NineStripProcessor& p);
    ~NineStripProcessorEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void updatePresetDisplay();
    void updatePresetComboBox();

    void parameterChanged(const juce::String& parameterID, float newValue) override;

   private:
    const int baseWidth = 600;
    const int baseHeight = 600;

    const int baseMargin = 3;

    // Layout constants are in pixels of the 600x600 base design and go through scaled(); knob and meter sizes
    // derive from the grid and scale by themselves. uiScale is recomputed at the top of resized().
    static constexpr float kBaseLabelFontHeight = 15.0f;  // juce::Label's default
    float uiScale = 1.0f;
    int scaled(float basePixels) const { return juce::roundToInt(basePixels * uiScale); }

    NineStripProcessor& audioProcessor;

    KnobLookAndFeel knobSkeuomorphicLook;
    FaderLookAndFeel faderSkeuomorphicLook;
    ScaledLookAndFeel presetLook;  // preset bar: lifts the 16 px font cap on the combo box and buttons

    juce::Image backgroundImage;
    juce::Image scaledBackground;

    juce::ComponentBoundsConstrainer constrainer;

    // Main layout grid
    juce::Grid mainGrid;

    // Preset Management Panel
    juce::Component presetPanel;
    juce::ComboBox presetComboBox;
    juce::TextButton savePresetButton;
    juce::TextButton deletePresetButton;
    juce::TextButton previousPresetButton;
    juce::TextButton nextPresetButton;

    // Console & Saturation
    juce::Component consoleSatGroup;
    juce::Label consoleSatLabel;
    CircularKnob consoleTypeSlider;
    juce::Label consoleTypeValueLabel;
    CircularKnob driveSlider;
    juce::Label driveLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> consoleTypeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    GlowButton saturationBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> saturationBypassAttachment;

    GlowButton saturationInputButton{"Pre"};

    // High Pass Filter
    juce::Component filterGroup;
    juce::Label filterLabel;
    CircularKnob lowpassSlider, hipassSlider, nonLinSlider;
    juce::Label lowpassLabel, hipassLabel, nonLinLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowpassAttachment, hipassAttachment, nonLinAttachment;

    // Filter Bypass
    GlowButton filterBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> filterBypassAttachment;

    // High Shelf
    juce::Component highShelfGroup;
    juce::Label highShelfLabel;
    CircularKnob trebleSlider;
    juce::Label trebleLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> trebleAttachment;

    // High-Mid EQ
    juce::Component highMidGroup;
    juce::Label highMidLabel;
    CircularKnob hmFreqSlider, hmGainSlider, hmResoSlider;
    juce::Label hmFreqLabel, hmGainLabel, hmResoLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hmFreqAttachment, hmGainAttachment, hmResoAttachment;

    // Low Shelf
    juce::Component lowShelfGroup;
    juce::Label lowShelfLabel;
    CircularKnob bassSlider;
    juce::Label bassLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bassAttachment;

    // EQ Bypass
    GlowButton eqBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> eqBypassAttachment;

    // Compressor
    juce::Component compressorGroup;
    juce::Label compressorLabel;
    CircularKnob pressureSlider, speedSlider, mewinessSlider;
    juce::Label pressureLabel, speedLabel, mewinessLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> pressureAttachment, speedAttachment,
        mewinessAttachment;
    NeedleVUMeter grMeter;
    GlowButton compressorBypassButton{"Byp"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compressorBypassAttachment;

    // Meters
    juce::Component metersGroup;
    NeedleVUMeter needleVUMeterL, needleVUMeterR;
    GlowButton vuMeterInputButton{"Input"};
    GlowButton vuMeterOutputButton{"Output"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> vuMeterModeAttachment;

    // Gain
    juce::Component gainGroup;
    juce::Label gainLabel;
    FineControlSlider inputGainSlider, outputGainSlider;
    juce::Label inputGainLabel, outputGainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment, outputGainAttachment;
    GlowButton masterBypassButton{"MASTER BYPASS"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> masterBypassAttachment;

    void setupPresetPanel();
    void setupConsoleSection();
    void setupFiltersSection();
    void setupEQSection();
    void setupDynamicsSection();
    void setupMeters();
    void setupGain();

    void setupMainGrid(juce::Rectangle<int> bounds);
    void layoutPresetPanel();
    void layoutConsoleSection(int bigKnobSize);
    void layoutFiltersSection(int bigKnobSize, int smallKnobSize);
    void layoutEQSection(int bigKnobSize, int smallKnobSize);
    void layoutDynamicsSection(int bigKnobSize, int smallKnobSize);
    void layoutMeters();
    void layoutGain();

    void setupGroupComponent(juce::Component& group, juce::Label& label, const juce::String& title);
    void setParameterFromClick(const juce::String& paramID, float newValue);
    void layoutBypassButton(GlowButton& button, juce::Rectangle<int> groupBounds);

    void addRotaryKnob(juce::Component& parent, CircularKnob& slider, juce::Label& label, const juce::String& paramID,
                       const juce::String& labelText, juce::Colour knobColor,
                       std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& attachment);

    void layoutTriangleKnobs(juce::Rectangle<int> bounds, CircularKnob& topLeft, juce::Label& topLeftLabel,
                             CircularKnob& topRight, juce::Label& topRightLabel, CircularKnob& bottom,
                             juce::Label& bottomLabel, int bigKnobSize, int smallKnobSize,
                             bool centerVertically = true);  // Default to centered

    void layoutCenteredKnob(juce::Rectangle<int> bounds, CircularKnob& knob, juce::Label& label, int knobSize);

    static juce::Rectangle<int> constrainToAspectRatio(juce::Rectangle<int> bounds, float aspectRatio);

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button* button) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NineStripProcessorEditor)
};