#pragma once
#include <algorithm>

/**
 * Second-order VU meter ballistic — IEC 60268-17 spec:
 *   Rise time to 99% : 300 ms
 *   Overshoot        : 1.18 %  (within the 1–1.5 % spec)
 *   Fall time        : 300 ms  (symmetric — same coefficients)
 *
 * Implemented as an exact ZOH (Zero-Order Hold) discrete-time model
 * at 60 Hz. Per tick: 6 multiply-accumulates, no trig, no sqrt.
 *
 * Continuous model:  ẍ + 2ζω₀ẋ + ω₀²x = ω₀²u
 *   ω₀ = 13.6291 rad/s,  ζ = 0.8162
 *   A = [[0, 1], [-ω₀², -2ζω₀]],  B = [0, ω₀²]ᵀ
 *   Ad = expm(A·dt),  Bd = A⁻¹(Ad − I)·B   at dt = 1/60 s
 */
class VUMeterBallistics
{
   public:
    explicit VUMeterBallistics(float initialPos = -60.0f) noexcept;

    void reset(float initialPos = -60.0f) noexcept;
    float tick(float targetDb) noexcept;

    [[nodiscard]] float getPosition() const noexcept { return pos; }

   private:
    // ZOH coefficients — valid only at exactly 60 Hz
    static constexpr float kAd00 = 0.97721060f;
    static constexpr float kAd01 = 0.01380647f;
    static constexpr float kAd10 = -2.56458407f;
    static constexpr float kAd11 = 0.67004233f;
    static constexpr float kBd0 = 0.02278940f;
    static constexpr float kBd1 = 2.56458407f;

    static constexpr float kFloor = -100.0f;  // below any scale minimum

    float pos;
    float vel = 0.0f;
};
