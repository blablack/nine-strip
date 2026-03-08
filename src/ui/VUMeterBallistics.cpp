#include "VUMeterBallistics.h"

VUMeterBallistics::VUMeterBallistics(float initialPos) noexcept : pos(initialPos) {}

void VUMeterBallistics::reset(float initialPos) noexcept
{
    pos = initialPos;
    vel = 0.0f;
}

float VUMeterBallistics::tick(float targetDb) noexcept
{
    const float u = std::max(targetDb, kFloor);
    const float np = kAd00 * pos + kAd01 * vel + kBd0 * u;
    const float nv = kAd10 * pos + kAd11 * vel + kBd1 * u;
    pos = np;
    vel = nv;
    return pos;
}
