/* ========================================
 *  Interstage - Interstage.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Interstage_H
#define __Interstage_H

#include <cstdint>

class Interstage
{
   public:
    enum
    {
        kNumParameters = 0  // Zero parameters - set and forget!
    };

    Interstage(double sampleRate);
    ~Interstage();

    void processReplacing(float** inputs, float** outputs, int sampleFrames);
    void processDoubleReplacing(double** inputs, double** outputs, int sampleFrames);

    void setSampleRate(double sr) { sampleRate = sr; }

   private:
    double sampleRate;

    uint32_t fpdL;
    uint32_t fpdR;

    // IIR filter state for left channel
    double iirSampleAL;
    double iirSampleBL;
    double iirSampleCL;
    double iirSampleDL;
    double iirSampleEL;
    double iirSampleFL;
    double lastSampleL;

    // IIR filter state for right channel
    double iirSampleAR;
    double iirSampleBR;
    double iirSampleCR;
    double iirSampleDR;
    double iirSampleER;
    double iirSampleFR;
    double lastSampleR;

    bool flip;
};

#endif
