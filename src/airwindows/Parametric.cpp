/* ========================================
 *  Parametric - Parametric.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Parametric.h"

#include <cstdlib>

Parametric::Parametric(double sampleRate) : sampleRate(sampleRate)
{
    A = 0.5;
    B = 0.5;
    C = 0.5;
    D = 0.5;
    E = 0.5;
    F = 0.5;
    G = 0.5;
    H = 0.5;
    I = 0.5;
    J = 1.0;

    for (int x = 0; x < biqs_total; x++)
    {
        high[x] = 0.0;
        hmid[x] = 0.0;
        lmid[x] = 0.0;
    }

    fpdL = 1.0;
    while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
    fpdR = 1.0;
    while (fpdR < 16386) fpdR = rand() * UINT32_MAX;
    // this is reset: values being initialized only once. Startup values, whatever
    // they are.
}

Parametric::~Parametric() {}

void Parametric::setParameter(int index, float value)
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
        case kParamE:
            E = value;
            break;
        case kParamF:
            F = value;
            break;
        case kParamG:
            G = value;
            break;
        case kParamH:
            H = value;
            break;
        case kParamI:
            I = value;
            break;
        case kParamJ:
            J = value;
            break;
        default:
            throw;  // unknown parameter, shouldn't happen!
    }
}

float Parametric::getParameter(int index)
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
        case kParamE:
            return E;
            break;
        case kParamF:
            return F;
            break;
        case kParamG:
            return G;
            break;
        case kParamH:
            return H;
            break;
        case kParamI:
            return I;
            break;
        case kParamJ:
            return J;
            break;
        default:
            break;  // unknown parameter, shouldn't happen!
    }
    return 0.0;  // we only need to update the relevant name, this is simple to
                 // manage
}
