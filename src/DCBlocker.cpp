#include "DCBlocker.h"

#include <cmath>

#if defined(_MSC_VER)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
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
    SampleType* RESTRICT left = channels[0];
    SampleType* RESTRICT right = channels[1];

    // Cache state in locals to avoid repeated member access through 'this'
    const double a = alpha;
    double xpL = x_prevL, ypL = y_prevL;
    double xpR = x_prevR, ypR = y_prevR;

    for (int i = 0; i < numSamples; ++i)
    {
        const double inL = static_cast<double>(left[i]);
        const double inR = static_cast<double>(right[i]);

        const double yL = inL - xpL + a * ypL;
        const double yR = inR - xpR + a * ypR;

        xpL = inL;
        ypL = yL;
        xpR = inR;
        ypR = yR;

        left[i] = static_cast<SampleType>(yL);
        right[i] = static_cast<SampleType>(yR);
    }

    x_prevL = xpL;
    y_prevL = ypL;
    x_prevR = xpR;
    y_prevR = ypR;
}

// Explicit template instantiations
template void DCBlocker::processStereo<float>(float** channels, int numSamples);
template void DCBlocker::processStereo<double>(double** channels, int numSamples);