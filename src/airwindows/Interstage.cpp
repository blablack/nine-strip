/* ========================================
 *  Interstage - Interstage.cpp
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Interstage.h"

#include <cstdlib>

Interstage::Interstage(double sampleRate) : sampleRate(sampleRate)
{
    // No parameters - zero UI controls!

    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;

    // Initialize left channel IIR state
    iirSampleAL = 0.0;
    iirSampleBL = 0.0;
    iirSampleCL = 0.0;
    iirSampleDL = 0.0;
    iirSampleEL = 0.0;
    iirSampleFL = 0.0;
    lastSampleL = 0.0;

    // Initialize right channel IIR state
    iirSampleAR = 0.0;
    iirSampleBR = 0.0;
    iirSampleCR = 0.0;
    iirSampleDR = 0.0;
    iirSampleER = 0.0;
    iirSampleFR = 0.0;
    lastSampleR = 0.0;

    flip = true;
    // this is reset: values being initialized only once. Startup values, whatever
    // they are.
}

Interstage::~Interstage() {}
