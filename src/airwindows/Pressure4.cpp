/* ========================================
 *  Pressure4 - Pressure4.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Pressure4.h"

#include <cstdlib>

Pressure4::Pressure4(double sampleRate) : sampleRate(sampleRate)
{
    A = 0.0;
    B = 0.2;
    C = 1.0;
    D = 1.0;
    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;
    muSpeedA = 10000;
    muSpeedB = 10000;
    muCoefficientA = 1;
    muCoefficientB = 1;
    muVary = 1;
    flip = false;
    // this is reset: values being initialized only once. Startup values, whatever
    // they are.
}

Pressure4::~Pressure4() {}

void Pressure4::setParameter(int index, float value)
{
    switch (index)
    {
        case kParamA:
            A = value;
            break;
        case kParamB:
            B = value;
            break;  // percent. Using this value, it'll be 0-100 everywhere
        case kParamC:
            C = value;
            break;  // this is the popup, stored as a float
        case kParamD:
            D = value;
            break;  // this is the popup, stored as a float
        default:
            throw;  // unknown parameter, shouldn't happen!
    }
    // we can also set other defaults here, and do calculations that only have to
    // happen once when parameters actually change. Here is the 'popup' setting
    // its (global) values. variables can also be set in the processreplacing
    // loop, and there they'll be set every buffersize here they're set when a
    // parameter's actually changed, which should be less frequent, but you must
    // use global variables in the Pressure4.h file to do it.
}

float Pressure4::getParameter(int index)
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
