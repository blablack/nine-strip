/* ========================================
 *  PurestGain - PurestGain.cpp
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "PurestGain.h"

#include <cstdlib>

PurestGain::PurestGain(double sampleRate) : sampleRate(sampleRate)
{
    A = 0.5;
    B = 1.0;

    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;

    gainchase = -90.0;
    settingchase = -90.0;
    gainBchase = -90.0;
    chasespeed = 350.0;

    // this is reset: values being initialized only once. Startup values, whatever
    // they are.
}

PurestGain::~PurestGain() {}

void PurestGain::setParameter(int index, float value)
{
    switch (index)
    {
        case kParamA:
            A = value;
            break;
        case kParamB:
            B = value;
            break;
        default:
            throw;  // unknown parameter, shouldn't happen!
    }
}

float PurestGain::getParameter(int index)
{
    switch (index)
    {
        case kParamA:
            return A;
            break;
        case kParamB:
            return B;
            break;
        default:
            break;  // unknown parameter, shouldn't happen!
    }
    return 0.0;  // we only need to update the relevant name, this is simple to
                 // manage
}