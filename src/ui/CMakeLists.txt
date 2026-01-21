add_library(NineStripUI STATIC
    KnobLookAndFeel.cpp
    FaderLookAndFeel.cpp
    NeedleVUMeter.cpp
)

target_include_directories(NineStripUI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(NineStripUI PUBLIC cxx_std_17)

target_link_libraries(NineStripUI
    PUBLIC
    NineStripAssets 
    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_dsp
)
