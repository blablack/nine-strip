#include "DCBlocker.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

DCBlocker::DCBlocker() : alpha(0.0), x_prevL(0.0), y_prevL(0.0), x_prevR(0.0), y_prevR(0.0) {}

DCBlocker::~DCBlocker() {}

void DCBlocker::prepare(double sampleRate)
{
    const double fc = 5.0;  // 5 Hz cutoff
    alpha = std::exp(-2.0 * M_PI * fc / sampleRate);
}

void DCBlocker::reset()
{
    x_prevL = x_prevR = 0.0;
    y_prevL = y_prevR = 0.0;
}

template <typename SampleType>
void DCBlocker::processStereo(SampleType** channels, int numSamples)
{
    SampleType* left = channels[0];
    SampleType* right = channels[1];

    for (int i = 0; i < numSamples; ++i)
    {
        // Process left channel
        double yL = left[i] - x_prevL + alpha * y_prevL;
        x_prevL = left[i];
        y_prevL = yL;
        left[i] = static_cast<SampleType>(yL);

        // Process right channel
        double yR = right[i] - x_prevR + alpha * y_prevR;
        x_prevR = right[i];
        y_prevR = yR;
        right[i] = static_cast<SampleType>(yR);
    }
}

// Explicit template instantiations
template void DCBlocker::processStereo<float>(float** channels, int numSamples);
template void DCBlocker::processStereo<double>(double** channels, int numSamples);