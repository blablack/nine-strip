/* ========================================
 *  Highpass2 - Highpass2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Highpass2.h"

#include <cstdlib>

Highpass2::Highpass2(double sampleRate) : sampleRate(sampleRate)
{
    A = 0.0;
    B = 0.5;   //-1.0 to 1.0
    C = 0.25;  // 0.0 to 4.0
    D = 1.0;
    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;

    iirSampleAL = 0.0;
    iirSampleBL = 0.0;
    iirSampleCL = 0.0;
    iirSampleDL = 0.0;
    iirSampleEL = 0.0;
    iirSampleFL = 0.0;
    iirSampleGL = 0.0;
    iirSampleHL = 0.0;

    iirSampleAR = 0.0;
    iirSampleBR = 0.0;
    iirSampleCR = 0.0;
    iirSampleDR = 0.0;
    iirSampleER = 0.0;
    iirSampleFR = 0.0;
    iirSampleGR = 0.0;
    iirSampleHR = 0.0;

    fpFlip = true;
    // this is reset: values being initialized only once. Startup values, whatever
    // they are.
}

Highpass2::~Highpass2() {}

void Highpass2::setParameter(int index, float value)
{
    switch (index)
    {
        case kParamA:
            A = value;
            break;
        case kParamB:
            B = value;
            break;
        case kParamC:
            C = value;
            break;
        case kParamD:
            D = value;
            break;
        default:
            throw;  // unknown parameter, shouldn't happen!
    }
}

float Highpass2::getParameter(int index)
{
    switch (index)
    {
        case kParamA:
            return A;
            break;
        case kParamB:
            return B;
            break;
        case kParamC:
            return C;
            break;
        case kParamD:
            return D;
            break;
        default:
            break;  // unknown parameter, shouldn't happen!
    }
    return 0.0;  // we only need to update the relevant name, this is simple to
                 // manage
}
