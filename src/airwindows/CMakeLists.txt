# Airwindows DSP Library
add_library(AirwindowsDSP STATIC
    Channel9.cpp
    Channel9Proc.cpp
    Highpass2.cpp
    Highpass2Proc.cpp
    Lowpass2.cpp
    Lowpass2Proc.cpp
    Baxandall2.cpp
    Baxandall2Proc.cpp
    Parametric.cpp
    ParametricProc.cpp
    Pressure4.cpp
    Pressure4Proc.cpp
)

target_include_directories(AirwindowsDSP PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(AirwindowsDSP PUBLIC cxx_std_17)

