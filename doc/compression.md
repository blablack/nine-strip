# Compressor

## Parameter A: Pressure

**UI Label:** Pressure (or Threshold/Amount)

**Function:** Controls the compression threshold and amount, determining how much the signal is compressed when it exceeds the threshold.

**Recommended Range:** 0% to 100% (or 0.0 to 1.0)

**Sound Character:**

- **Low values (0-25%):** Minimal compression, gentle dynamic control - soft compression that's subtle and less obvious
- **Medium values (25-60%):** Moderate compression suitable for mix bus and general dynamic control
- **High values (60-85%):** Heavy compression, noticeable gain reduction and tonal change
- **Extreme values (85-100%):** Severely dynamic sound with organic quality even at aggressive settings

**Typical Values:**

- Mix bus (transparent glue): 15-30%
- Drum bus (punch and control): 40-60%
- Individual kick drum: 50-70%
- Individual snare: 45-65%
- Extreme effect/character: 80-95%
- Peak limiting mode: 90-100% with high Speed setting

**Interaction Notes:** Makeup gain is automatically applied internally - "tends to constrain sound rather than inflate it." The compression is stereo-linked, taking the highest level from either L/R channel to avoid stereo image pumping. At extreme settings with high Speed, creates "artifacts as part of the sound of the compressor."

***

## Parameter B: Speed

**UI Label:** Speed (or Attack/Release)

**Function:** Controls the attack and release time of the compressor, from slow, smooth compression to fast, aggressive pumping.

**Recommended Range:** 0% to 100% (displays as percentage or qualitative descriptor)

**Sound Character:**

- **Low values (0-25%):** Very slow attack/release - "really stepping on it a lot," retains soundstage and relative volumes, smooth and natural
- **Medium values (25-60%):** Moderate timing, musical compression suitable for most sources
- **High values (60-85%):** Fast response, more upfront character, emphasizes room/ambience
- **Maximum (85-100%):** "Keeping it all as loud as possible at all times," extremely fast response approaching peak limiting behavior, can create pumping artifacts

**Typical Values:**

- Mix bus (smooth glue): 10-30%
- Vocals (natural): 25-45%
- Drum bus (controlled): 35-55%
- Kick drum (punchy): 40-60%
- Snare (aggressive): 55-75%
- Peak limiting/extreme effect: 80-100%

**Interaction Notes:** At maximum Speed settings with high Pressure, the compressor can "drop out between notes, sometimes drop out between waveform cycles" creating characteristic artifacts. Turning Speed down causes it to "sit on its very heavily compressed level for a while." Higher Speed settings make the "environment of those loud bits stand out a little bit more." The release calculation is sample-rate compensated.

***

## Parameter C: Mewiness (µ)

**UI Label:** Character (or Mu/Mewiness/Ratio)

**Function:** Controls the compression character and ratio behavior, from inverse/parallel-like compression through normal compression to variable-ratio "very mu" style compression.

**Recommended Range:** -100% to +100% (or -1.0 to +1.0)

**Sound Character:**

**Negative Values (-100% to 0%):** "Inverse" compression behavior
- Retains attack transient punch while still compressing
- Similar to parallel compression effect: "letting through peaks" while compressed sound happens "behind everything"
- Best for: Maintaining drum transients, adding body without losing attack

**Center (0%):** Normal compression behavior
- Standard fixed-ratio compression response
- Most transparent and predictable
- Best for: General-purpose compression, mix bus

**Positive Values (0% to +100%):** "Very mu" style compression
- Variable ratio that responds to signal level
- Creates "shortening effect" on drums
- Best for: Aggressive compression, controlling peaks, vintage character

**Typical Values:**

- Transparent mix bus: -10% to +10%
- Kick drum (punch retention): -40% to -60%
- Snare (natural): -20% to 0%
- Drum bus (controlled): +20% to +40%
- Vocal (smooth leveling): +30% to +50%
- Aggressive limiting: +60% to +100%

**Interaction Notes:** This compressor offers a unique approach to compression character with negative ratios for parallel-like effects without actual parallel routing. At positive extremes with high Pressure/Speed, creates heavily squished, "crushed up" character. At negative settings, creates the parallel compression-like effect while maintaining simplicity of a single instance.

***

## Parameter D: Output Gain

**UI Label:** Output Gain (or Output Level)

**Function:** Simple post-compression output level control for gain staging and makeup gain.

**Recommended Range:** 0% to 200% (or 0.0 to 2.0, can display in dB)

**Sound Character:**

- **Below 100%:** Reduces output level, useful when compression adds too much apparent loudness
- **100%:** Unity gain after compression
- **Above 100%:** Boosts output level for makeup gain or additional drive into the built-in saturation stage

**Typical Values:**

- With automatic makeup gain, often leave at 100% (unity)
- Mix bus: 90-110% for subtle adjustment
- Heavy compression makeup: 110-150%
- Extreme settings: Up to 200% to drive the output saturation

**Interaction Notes:** The compressor has built-in makeup gain calculated from the threshold, so Output Gain is often left at unity. If you do increase it significantly, you'll drive into a second-stage saturation: "sine-based overdrive to prevent overs and allow bloody loud extremeness." This saturation clips at approximately 1.57 (π/2) using sine shaping for smooth limiting.

***

## General Usage Notes for Mixing Engineers

**Unique Characteristics:**

1. **Stereo-Linked Intelligence:** Unlike simple stereo linking, this compressor tracks the highest level between L/R channels sample-by-sample, preventing stereo image disturbance while avoiding channel dropouts.

2. **Built-in Saturation Safety:** Second-stage sine saturation prevents digital overs even at extreme settings, allowing "bloody loud extremeness" safely.

3. **No Traditional Attack Control:** Speed controls both attack and release together in a musical relationship - you can't set them independently.

4. **Organic Sound:** "Pretty rich warm sounding compressor" with "transparency" and "organic quality" even at extreme settings.

5. **Artifact-as-Feature:** At extreme Speed/Pressure settings, the compression artifacts become part of the character rather than flaws.

**Recommended Workflow:**

1. **Start Conservative:** Begin with Pressure around 30-40%, Speed around 40-50%, Character at 0%
2. **Set Pressure:** Adjust to taste for desired amount of compression
3. **Adjust Speed:** Faster for more aggression, slower for smoothness
4. **Dial Character:** Negative for punch, positive for smoothness/control
5. **Set Output:** Adjust for gain staging, typically leave near 100%

**Best For:**
- Mix bus glue compression
- Drum bus control with character
- Individual drum shaping (especially kick/snare)
- Aggressive peak control without harshness
- Creative effects with extreme settings

**Avoid:**
- Mastering (use gentler settings or dedicated mastering compressor)
- When you need independent attack/release control
- When you need precise ratio control (Character is more about flavor than exact ratios)

**Integration with Channel Strip:**
Position after EQ in your signal chain. The compression will respond to your EQ moves, and the built-in saturation provides final safety limiting. For parallel compression effect, use negative Character settings rather than actual parallel routing.