/* ========================================
 *  Pressure4 - Pressure4.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pressure4_H
#define __Pressure4_H

#include <atomic>
#include <cstdint>

class Pressure4
{
   public:
    enum
    {
        kParamA = 0,
        kParamB = 1,
        kParamC = 2,
        kParamD = 3,
        kNumParameters = 4
    };  //
    Pressure4(double sampleRate);
    ~Pressure4();

    void processReplacing(float** inputs, float** outputs, int sampleFrames);
    void processDoubleReplacing(double** inputs, double** outputs, int sampleFrames);

    void setParameter(int index, float value);
    float getParameter(int index);
    void setSampleRate(double sr) { sampleRate = sr; }

    float getGainReduction() { return currentGainReduction; }
    void resetGRTracking() { blockMinCoefficient = 1.0f; }
    void finalizeGR() { currentGainReduction.store(blockMinCoefficient); }

   private:
    double sampleRate;

    double muVary;
    double muAttack;
    double muNewSpeed;
    double muSpeedA;
    double muSpeedB;
    double muCoefficientA;
    double muCoefficientB;
    uint32_t fpdL;
    uint32_t fpdR;
    bool flip;

    float A;
    float B;
    float C;  // parameters. Always 0-1, and we scale/alter them elsewhere.
    float D;

    std::atomic<float> currentGainReduction{1.0f};
    float blockMinCoefficient = 1.0f;  // Reset per block
};

#endif
