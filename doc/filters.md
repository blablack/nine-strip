# Highpass and Lowpass Filters

**Note:** These are NOT standard digital filters. They use IIR topology with dynamic level-dependent response and progressive pole staging.

---

# Highpass

## Parameter A: Frequency

**UI Label:** Hipass

**Function:** Controls the IIR filter coefficient, determining how much filtering is applied.

**Display Range:** 0 to 10

**Default Value:** 0

**Sound Character:**

- **0:** No filtering (completely bypassed)
- **Low values (0.1-3):** Gentle highpass filtering
- **High values (7-10):** More aggressive filtering

**Typical Values:**

- Subtle rumble removal: 0.1-0.5
- Vocal cleanup: 1-3
- Aggressive bass removal: 5-8

**Important:** This is an IIR coefficient, NOT a frequency in Hz. Higher values = more filtering.

---

## Parameter B: Ls/Tite (Loose/Tight)

**UI Label:** HP Ls/Tite

**Function:** Creates dynamic, level-dependent filtering by modulating the filter coefficient based on input signal amplitude.

**Display Range:** -10 to +10

**Default Value:** 0

**Sound Character:**

- **Negative values (Loose):** Filter coefficient is REDUCED when signal level is high
  - Quiet signals get MORE filtering
  - Loud transients get LESS filtering (more highs pass through)
  - Effect: Transients punch through brighter
- **0 (Center):** Constant filtering regardless of signal level
- **Positive values (Tight):** Filter coefficient is INCREASED when signal level is high
  - Loud signals get MORE filtering
  - Quiet signals get LESS filtering
  - Effect: Transients are darkened, reverb tails stay brighter

**Typical Values:**

- Neutral filtering: 0
- Transient emphasis: -3 to -7 (Loose)
- Transient control: +3 to +7 (Tight)

**Implementation Detail:** For negative values, the effect is halved (`tight *= 0.5`) to prevent extreme behavior.

---

## Parameter C: Poles

**UI Label:** HP Slope

**Function:** Progressive wet/dry control that adds cascaded filter stages.

**Display Range:** 0 to 10

**Default Value:** 2.5

**How It Works:** The behavior creates progressive staging where:

- **0-2.5:** Single filter stage, varying from subtle to full wet
- **2.5-5:** Adding second stage influence
- **5-7.5:** Adding third stage influence
- **7.5-10:** Maximum steepness with fourth stage

**Sound Character:**

- **0-2.5:** Gentle filtering, like a dry/wet control
- **2.5-5:** Steeper slope, more pronounced effect
- **5-7.5:** Even steeper, more aggressive filtering
- **7.5-10:** Maximum steepness, most dramatic effect

**Typical Values:**

- Subtle: 1-2.5 (default)
- Standard: 4-6
- Aggressive: 7.5-10

---

# Lowpass

## Parameter A: Frequency

**UI Label:** Lowpass

**Function:** Controls the IIR filter coefficient for lowpass filtering.

**Display Range:** 0 to 10

**Default Value:** 10 (fully open, no filtering)

**Sound Character:**

- **Low values (0-2):** Aggressive darkening, removes most highs
- **Medium values (3-7):** Moderate tone shaping
- **High values (8-10):** Subtle high-frequency rolloff

**Typical Values:**

- Extreme darkening: 1-3
- Natural warmth: 5-7
- Gentle air control: 8-9.5
- No effect: 10

**Important:** Like Hipass, this is an IIR coefficient, NOT a frequency in Hz. Lower values = more filtering.

---

## Parameter B: Soft/Hard

**UI Label:** LP Soft/Hard

**Function:** Dynamic level-dependent filtering (same mechanism as Hipass's Ls/Tite).

**Display Range:** -10 to +10

**Default Value:** 0

**Sound Character:**

- **Negative values (Soft):** Filter MORE when signal is LOUD
  - Transients are darkened
  - Quiet passages stay brighter
  - Creates "compressing" effect on brightness
- **0 (Center):** Constant filtering
- **Positive values (Hard):** Filter LESS when signal is LOUD
  - Transients punch through with more highs
  - Quiet sounds are darker
  - Emphasizes attacks

**Typical Values:**

- Even tone: -3 to -5 (Soft)
- Neutral: 0
- Transient emphasis: +4 to +7 (Hard)

---

## Parameter C: Poles

**UI Label:** LP Slope

**Function:** Identical to Hipass slope - progressive staging from 0-4 filter stages.

**Display Range:** 0 to 10

**Default Value:** 2.5

**Sound Character:** Same progressive behavior as HP Slope.

---

## General Usage Notes

**Key Differences from Standard Filters:**

1. **NOT frequency-based:** Frequency parameters control IIR coefficients, not Hz values
2. **Dynamic filtering:** Soft/Hard and Ls/Tite parameters create sample-by-sample level-dependent behavior
3. **Progressive staging:** Slope parameters use wet/dry blending rather than traditional pole multiplication
4. **Interleaved topology:** Uses alternating filter banks (A/C/E/G vs B/D/F/H) with fpFlip

**Parameter Interaction:**

- **Frequency + Slope:** Higher slope values amplify the effect of the frequency setting
- **Frequency + Soft/Hard (or Ls/Tite):** Dynamic behavior only occurs when frequency is actively filtering
- Extreme combinations can create unusual but musical artifacts

**Known Issues:**

- Extreme Soft/Hard or Ls/Tite values (±8 to ±10) can cause unusual artifacts
- Very low frequency values (0-1) can cause filter instability
