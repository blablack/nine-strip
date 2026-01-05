/* ========================================
 *  Baxandall2 - Baxandall2.h
 *  Created 8/12/11 by SPIAdmin
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Baxandall2_H
#define __Baxandall2_H

#include <cstdint>

class Baxandall2
{
   public:
    enum
    {
        kParamA = 0,
        kParamB = 1,
        kNumParameters = 2
    };

    Baxandall2(double sampleRate);
    ~Baxandall2();

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
    double trebleAL[9];
    double trebleBL[9];
    double bassAL[9];
    double bassBL[9];

    double trebleAR[9];
    double trebleBR[9];
    double bassAR[9];
    double bassBR[9];
    bool flip;

    float A;
    float B;
    float C;
    float D;
    float E;  // parameters. Always 0-1, and we scale/alter them elsewhere.
};

#endif
