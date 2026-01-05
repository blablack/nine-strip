/* ========================================
 *  Highpass2 - Highpass2.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Highpass2_H
#define __Highpass2_H

#include <cstdint>

class Highpass2
{
   public:
    enum
    {
        kParamA = 0,
        kParamB = 1,
        kParamC = 2,
        kParamD = 3,
        kNumParameters = 4
    };

    Highpass2(double sampleRate);
    ~Highpass2();

    void processReplacing(float** inputs, float** outputs, int sampleFrames);
    void processDoubleReplacing(double** inputs, double** outputs, int sampleFrames);

    void setParameter(int index, float value);
    float getParameter(int index);
    void setSampleRate(double sr) { sampleRate = sr; }

   private:
    double sampleRate;

    uint32_t fpdL;
    uint32_t fpdR;
    // default stuff

    double iirSampleAL;
    double iirSampleBL;
    double iirSampleCL;
    double iirSampleDL;
    double iirSampleEL;
    double iirSampleFL;
    double iirSampleGL;
    double iirSampleHL;

    double iirSampleAR;
    double iirSampleBR;
    double iirSampleCR;
    double iirSampleDR;
    double iirSampleER;
    double iirSampleFR;
    double iirSampleGR;
    double iirSampleHR;

    bool fpFlip;

    float A;
    float B;
    float C;
    float D;
};

#endif
