/* ========================================
 *  Channel9 - Channel9.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Channel9_H
#define __Channel9_H

#include <cstdint>

class Channel9
{
   public:
    enum
    {
        kParamA = 0,
        kParamB = 1,
        kParamC = 2,
        kNumParameters = 3
    };  //

    Channel9(double sampleRate);
    ~Channel9();

    // Keep original processing methods but adapt signature
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

    double iirSampleLA;
    double iirSampleRA;
    double iirSampleLB;
    double iirSampleRB;
    double lastSampleAL;
    double lastSampleBL;
    double lastSampleCL;
    double lastSampleAR;
    double lastSampleBR;
    double lastSampleCR;
    double biquadA[15];
    double biquadB[15];
    double iirAmount;
    double threshold;
    double cutoff;
    bool flip;

    float A;
    float B;
    float C;
};

#endif
