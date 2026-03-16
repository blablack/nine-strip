/* ========================================
 *  Capacitor2 - Capacitor2.cpp
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Capacitor2.h"

#include <cstdlib>

Capacitor2::Capacitor2(double sampleRate) : sampleRate(sampleRate)
{
    A = 1.0;
    B = 0.0;
    C = 0.0;
    D = 1.0;

    iirHighpassAL = 0.0;
    iirHighpassBL = 0.0;
    iirHighpassCL = 0.0;
    iirHighpassDL = 0.0;
    iirHighpassEL = 0.0;
    iirHighpassFL = 0.0;
    iirLowpassAL = 0.0;
    iirLowpassBL = 0.0;
    iirLowpassCL = 0.0;
    iirLowpassDL = 0.0;
    iirLowpassEL = 0.0;
    iirLowpassFL = 0.0;

    iirHighpassAR = 0.0;
    iirHighpassBR = 0.0;
    iirHighpassCR = 0.0;
    iirHighpassDR = 0.0;
    iirHighpassER = 0.0;
    iirHighpassFR = 0.0;
    iirLowpassAR = 0.0;
    iirLowpassBR = 0.0;
    iirLowpassCR = 0.0;
    iirLowpassDR = 0.0;
    iirLowpassER = 0.0;
    iirLowpassFR = 0.0;

    count = 0;

    lowpassChase = 0.0;
    highpassChase = 0.0;
    wetChase = 0.0;

    lowpassBaseAmount = 1.0;
    highpassBaseAmount = 0.0;
    wet = 1.0;

    lastLowpass = 1000.0;
    lastHighpass = 1000.0;
    lastWet = 1000.0;

    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;
    // this is reset: values being initialized only once. Startup values, whatever they are.
}

Capacitor2::~Capacitor2() {}

void Capacitor2::setParameter(int index, float value)
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

float Capacitor2::getParameter(int index)
{
    switch (index)
    {
        case kParamA:
            return A;
        case kParamB:
            return B;
        case kParamC:
            return C;
        case kParamD:
            return D;
        default:
            break;  // unknown parameter, shouldn't happen!
    }
    return 0.0;
}
