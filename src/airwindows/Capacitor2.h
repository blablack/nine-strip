/* ========================================
 *  Capacitor2 - Capacitor2.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Capacitor2_H
#define __Capacitor2_H

#include <cstdint>

class Capacitor2
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

    Capacitor2(double sampleRate);
    ~Capacitor2();

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

    double iirHighpassAL;
    double iirHighpassBL;
    double iirHighpassCL;
    double iirHighpassDL;
    double iirHighpassEL;
    double iirHighpassFL;
    double iirLowpassAL;
    double iirLowpassBL;
    double iirLowpassCL;
    double iirLowpassDL;
    double iirLowpassEL;
    double iirLowpassFL;

    double iirHighpassAR;
    double iirHighpassBR;
    double iirHighpassCR;
    double iirHighpassDR;
    double iirHighpassER;
    double iirHighpassFR;
    double iirLowpassAR;
    double iirLowpassBR;
    double iirLowpassCR;
    double iirLowpassDR;
    double iirLowpassER;
    double iirLowpassFR;

    int count;

    double lowpassChase;
    double highpassChase;
    double wetChase;

    double lowpassBaseAmount;
    double highpassBaseAmount;
    double wet;

    double lastLowpass;
    double lastHighpass;
    double lastWet;

    float A;
    float B;
    float C;
    float D;
};

#endif
