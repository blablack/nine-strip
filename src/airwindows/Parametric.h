/* ========================================
 *  Parametric - Parametric.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Parametric_H
#define __Parametric_H

#include <cstdint>

class Parametric
{
   public:
    enum
    {
        kParamA = 0,
        kParamB = 1,
        kParamC = 2,
        kParamD = 3,
        kParamE = 4,
        kParamF = 5,
        kParamG = 6,
        kParamH = 7,
        kParamI = 8,
        kParamJ = 9,
        kNumParameters = 10
    };
    Parametric(double sampleRate);
    ~Parametric();

    void processReplacing(float** inputs, float** outputs, int sampleFrames);
    void processDoubleReplacing(double** inputs, double** outputs, int sampleFrames);

    void setParameter(int index, float value);
    float getParameter(int index);
    void setSampleRate(double sr) { sampleRate = sr; }

   private:
    double sampleRate;

    float A;
    float B;
    float C;
    float D;
    float E;
    float F;
    float G;
    float H;
    float I;
    float J;

    enum
    {
        biqs_freq,
        biqs_reso,
        biqs_level,
        biqs_nonlin,
        biqs_temp,
        biqs_dis,
        biqs_a0,
        biqs_a1,
        biqs_b1,
        biqs_b2,
        biqs_c0,
        biqs_c1,
        biqs_d1,
        biqs_d2,
        biqs_e0,
        biqs_e1,
        biqs_f1,
        biqs_f2,
        biqs_aL1,
        biqs_aL2,
        biqs_aR1,
        biqs_aR2,
        biqs_cL1,
        biqs_cL2,
        biqs_cR1,
        biqs_cR2,
        biqs_eL1,
        biqs_eL2,
        biqs_eR1,
        biqs_eR2,
        biqs_outL,
        biqs_outR,
        biqs_total
    };
    double high[biqs_total];
    double hmid[biqs_total];
    double lmid[biqs_total];

    uint32_t fpdL;
    uint32_t fpdR;
    // default stuff
};

#endif
