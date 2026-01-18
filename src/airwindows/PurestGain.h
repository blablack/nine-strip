/* ========================================
 *  PurestGain - PurestGain.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestGain_H
#define __PurestGain_H

#include <cstdint>

class PurestGain
{
   public:
    enum
    {
        kParamA = 0,
        kParamB = 1,
        kNumParameters = 2
    };

    PurestGain(double sampleRate);
    ~PurestGain();

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

    double gainchase;
    double settingchase;
    double gainBchase;
    double chasespeed;

    float A;
    float B;
};

#endif