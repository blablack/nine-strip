# Airwindows DSP Library
add_library(AirwindowsDSP STATIC
    Channel9.cpp
    Channel9Proc.cpp
    Capacitor2.cpp
    Capacitor2Proc.cpp
    Baxandall2.cpp  
    Baxandall2Proc.cpp
    Parametric.cpp
    ParametricProc.cpp
    Pressure4.cpp
    Pressure4Proc.cpp
    PurestGain.cpp
    PurestGainProc.cpp
    Interstage.cpp
    InterstageProc.cpp
)

target_include_directories(AirwindowsDSP PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(AirwindowsDSP PUBLIC cxx_std_17)

