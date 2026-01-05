/* ========================================
 *  Baxandall2 - Baxandall2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Baxandall2.h"

#include <cstdlib>

Baxandall2::Baxandall2(double sampleRate) : sampleRate(sampleRate)
{
    A = 0.5;
    B = 0.5;
    for (int x = 0; x < 9; x++)
    {
        trebleAL[x] = 0.0;
        trebleBL[x] = 0.0;
        bassAL[x] = 0.0;
        bassBL[x] = 0.0;
        trebleAR[x] = 0.0;
        trebleBR[x] = 0.0;
        bassAR[x] = 0.0;
        bassBR[x] = 0.0;
    }
    flip = false;
    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;
    // this is reset: values being initialized only once. Startup values, whatever
    // they are.
}

Baxandall2::~Baxandall2() {}

void Baxandall2::setParameter(int index, float value)
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

float Baxandall2::getParameter(int index)
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
