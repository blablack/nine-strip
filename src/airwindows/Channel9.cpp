/* ========================================
 *  Channel9 - Channel9.cpp
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#include "Channel9.h"

#include <cstdlib>

Channel9::Channel9(double sampleRate) : sampleRate(sampleRate) {
  A = 0.0;
  B = 0.0;
  C = 1.0;

  for (int x = 0; x < 15; x++) {
    biquadA[x] = 0.0;
    biquadB[x] = 0.0;
  }

  fpdL = 1.0;
  while (fpdL < 16386) fpdL = rand() * UINT32_MAX;
  fpdR = 1.0;
  while (fpdR < 16386) fpdR = rand() * UINT32_MAX;

  iirSampleLA = 0.0;
  iirSampleRA = 0.0;
  iirSampleLB = 0.0;
  iirSampleRB = 0.0;
  lastSampleAL = lastSampleBL = lastSampleCL = 0.0;
  lastSampleAR = lastSampleBR = lastSampleCR = 0.0;
  flip = false;

  // Initialize with Neve values
  iirAmount = 0.005832;
  threshold = 0.33362176;
  cutoff = 28811.0;
}

Channel9::~Channel9() {}

void Channel9::setParameter(int index, float value) {
  switch (index) {
    case kParamA:
      A = value;
      break;
    case kParamB:
      B = value;
      break;
    case kParamC:
      C = value;
      break;
    default:
      return;
  }

  // Update console type settings
  switch (static_cast<int>(A)) {
    case 0:
      iirAmount = 0.005832;
      threshold = 0.33362176;
      cutoff = 28811.0;
      break;  // Neve
    case 1:
      iirAmount = 0.004096;
      threshold = 0.59969536;
      cutoff = 27216.0;
      break;  // API
    case 2:
      iirAmount = 0.004913;
      threshold = 0.84934656;
      cutoff = 23011.0;
      break;  // SSL
    case 3:
      iirAmount = 0.009216;
      threshold = 0.149;
      cutoff = 18544.0;
      break;  // Teac
    case 4:
      iirAmount = 0.011449;
      threshold = 0.092;
      cutoff = 19748.0;
      break;  // Mackie
    default:
      break;
  }
}

float Channel9::getParameter(int index) {
  switch (index) {
    case kParamA:
      return A;
    case kParamB:
      return B;
    case kParamC:
      return C;
    default:
      break;
  }
  return 0.0;
}

