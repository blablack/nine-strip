# Gain Staging and Bypass Controls

## Input Gain

**UI Label:** Input Gain

**Function:** Controls the input level entering the channel strip processing chain. Applied before all processing (saturation, EQ, compression).

**Display Range:** -10 to +10

**Default Value:** 0 (unity gain)

**Purpose:**

The Input Gain control serves multiple critical functions in your signal flow:

1. **Gain Staging:** Optimizes signal level for the analog-modeled saturation stage
2. **Drive Control:** Higher input levels push the console saturation harder
3. **Headroom Management:** Prevents clipping before processing begins
4. **Signal Matching:** Matches levels from different sources

**Typical Values:**

- **+3 to +6:** Driving saturation harder for more harmonic content and color
- **+1 to +3:** Gentle level boost, subtle saturation enhancement
- **0:** Unity gain, neutral starting point
- **-1 to -3:** Gentle reduction, controlling hot input signals
- **-4 to -6:** Significant reduction for very hot sources
- **-7 to -10:** Maximum reduction for extremely hot signals or special effects

**Usage Guidelines:**

**For Saturation Character:**

- Increase Input Gain and Drive together for maximum analog warmth
- Watch for excessive distortion - some saturation is musical, too much becomes harsh
- Different console types respond differently to input level (Neve/API are more forgiving, Teac/Mackie saturate more aggressively)

**For Clean Processing:**

- Keep Input Gain at 0 or slightly negative
- Use minimal Drive setting
- Focus on EQ and compression for tone shaping

**For Gain Staging:**

- Aim for average levels around -18 dBFS to -12 dBFS after input gain
- Leave headroom for peaks and transients
- Monitor input meters to avoid clipping

**Interaction with Other Parameters:**

- **Drive:** Input Gain + Drive work together to determine saturation intensity
- **Compressor:** Higher input levels trigger more compression (if Pressure is set)
- **EQ:** Boosting frequencies after high input gain may cause clipping

---

## Output Gain

**UI Label:** Output Gain

**Function:** Controls the output level leaving the channel strip. Applied after all processing (saturation, EQ, compression).

**Display Range:** -10 to +10

**Default Value:** 0 (unity gain)

**Purpose:**

The Output Gain control provides:

1. **Makeup Gain:** Compensates for gain reduction from compression or EQ cuts
2. **Level Matching:** Ensures consistent output level across different processing settings
3. **Mix Integration:** Adjusts processed signal to sit properly in the mix
4. **Headroom Control:** Prevents clipping at the output stage

**Typical Values:**

- **+3 to +6:** Makeup gain after heavy compression or EQ cuts
- **+1 to +3:** Moderate makeup gain, general level boost
- **0:** Unity gain, level unchanged
- **-1 to -3:** Reducing output level to avoid clipping or for mix balance
- **-4 to -6:** Significant reduction after heavy EQ boosts
- **-7 to -10:** Maximum reduction for special effects or parallel processing

**Usage Guidelines:**

**After Compression:**

- Compressor adds automatic makeup gain, but Output Gain provides fine control
- Increase Output Gain if compression made signal too quiet
- Decrease if compression + makeup made signal too loud

**After EQ:**

- Boost Output Gain to compensate for EQ cuts
- Reduce Output Gain to compensate for EQ boosts
- Use meters to ensure output isn't clipping

**For A/B Comparison:**

- Match output levels when bypassing processing sections
- Louder often sounds "better" - level-matched comparison is crucial
- Use Output Gain to achieve equal loudness for fair comparison

**Professional Workflow:**

1. Set Input Gain for optimal drive/saturation
2. Apply EQ and compression
3. Adjust Output Gain to maintain consistent level
4. Monitor output meters to ensure no clipping
5. A/B with bypass to confirm improvement, not just loudness

---

## Master Bypass

**UI Label:** Bypass

**Function:** Bypasses ALL processing in the channel strip (saturation, filters, shelves, parametric EQ, compression). Signal passes through completely unprocessed with only Input/Output gain applied.

**Type:** Boolean toggle (On/Off)

**Default State:** Off (processing active)

**Behavior:**

When activated:

- All AirWindows processors are bypassed
- Input Gain is still applied
- Output Gain is still applied
- Meters show input/output levels with gain adjustments only

**Usage:**

- **A/B Comparison:** Instantly compare processed vs. unprocessed signal
- **Troubleshooting:** Isolate whether the channel strip is causing issues
- **Reference:** Check if processing is actually improving the sound
- **CPU Saving:** Disable processing on unused instances (though gain is still applied)

**Tip:** Master Bypass is your reality check - use it frequently to ensure your processing is actually helping!

---

## Saturation Bypass

**UI Label:** Saturation Bypass

**Function:** Bypasses only the Console Type saturation stage (Channel9 processor). EQ and compression remain active.

**Type:** Boolean toggle (On/Off)

**Default State:** Off (saturation active)

**When to Use:**

**Bypass Saturation When:**

- You want completely clean, transparent processing
- Saturation is adding unwanted artifacts or harshness
- Working with already-saturated sources (tape, analog gear)
- You need maximum headroom for heavy EQ/compression
- Modern, pristine digital sound is the goal

**Keep Saturation Active When:**

- Adding analog warmth and character
- Smoothing harsh digital sources
- Creating vintage/retro aesthetics
- Gluing mix elements together with harmonic content
- Emulating specific console workflows (Neve, SSL, etc.)

**Workflow Tip:** Try processing with saturation, then bypass it to hear the contribution. Sometimes the saturation is adding more color than you realize!

---

## EQ Bypass

**UI Label:** EQ Bypass

**Function:** Bypasses ALL EQ processing (Hipass, Lowpass, High Shelf, Low Shelf, High-Mid parametric). Saturation and compression remain active.

**Type:** Boolean toggle (On/Off)

**Default State:** Off (EQ active)

**What's Bypassed:**

- Hipass filter (all parameters)
- Lowpass filter (all parameters)
- High Shelf (Baxandall treble)
- Low Shelf (Baxandall bass)
- High-Mid Freq parametric band (all parameters)

**When to Use:**

**Bypass EQ When:**

- Checking if EQ is actually improving tonal balance
- Source already has perfect tone
- You only need dynamics control (compression)
- Troubleshooting phase issues
- Maximum transparency is required

**Keep EQ Active When:**

- Correcting tonal imbalances
- Removing problem frequencies
- Enhancing character and presence
- Matching sources to the mix
- Creating specific tonal aesthetics

**A/B Technique:**

1. Set all EQ carefully while listening
2. Bypass EQ completely
3. Listen for 10-20 seconds to reset ears
4. Re-enable EQ
5. Confirm improvement is worth the processing

---

## Compressor Bypass

**UI Label:** Compressor Bypass

**Function:** Bypasses only the compressor (Pressure4 processor). Saturation and EQ remain active.

**Type:** Boolean toggle (On/Off)

**Default State:** Off (compression active)

**When to Use:**

**Bypass Compression When:**

- Checking the compressor's contribution to the sound
- Dynamics are already perfect
- You want maximum natural dynamic range
- Transients need maximum impact
- Compression is over-squashing the signal

**Keep Compression Active When:**

- Controlling excessive dynamics
- Adding punch and sustain
- Creating glue and cohesion
- Achieving vintage compression character
- Aggressive dynamic shaping is needed

**Critical Listening:**

- Compressor adds automatic makeup gain - bypass to hear true dynamic change
- Pay attention to: transient impact, sustain, apparent loudness, punch
- Sometimes less compression sounds bigger and more powerful!

---

## Bypass Strategy & Workflow

**The Three-Stage Check:**

1. **Full Bypass (Master):** Does the processing help at all?
2. **Section Bypass:** Which sections are contributing most?
3. **Parameter Sweep:** Within active sections, are settings optimal?

**Level-Matched Comparison:**

When using any bypass:

1. Note the output level with processing active
2. Bypass the section
3. Adjust Output Gain to match the level
4. Now compare tone/character, not just loudness
5. Louder almost always sounds "better" - don't be fooled!

**CPU Management:**

- Master Bypass still applies gain (minimal CPU)
- Section bypasses save more CPU than parameter settings at zero
- For unused instances, use Master Bypass or section bypasses

**Mixing Workflow:**

1. Start with everything bypassed
2. Activate Saturation - does it help?
3. Activate EQ - set it, then bypass/re-enable to confirm
4. Activate Compression - set it, then bypass/re-enable to confirm
5. Use Master Bypass periodically to check overall contribution
6. Make final adjustments with frequent bypass checks

**The Bypass Honesty Test:**

If you can't hear a clear improvement when re-enabling processing:

- Your settings might be too subtle
- Your settings might be counterproductive
- The source might not need that processing
- Your monitoring environment might not be revealing enough

**Remember:** Bypass is not defeat - it's wisdom. If it sounds better bypassed, bypass it!
