# Compressor

## Parameter A: Pressure

**UI Label:** Pressure

**Function:** Controls the compression threshold and amount, determining how much the signal is compressed when it exceeds the threshold.

**Display Range:** 0 to 10

**Sound Character:**

- **Low values (0-2.5):** Minimal compression, gentle dynamic control - soft compression that's subtle and less obvious
- **Medium values (2.5-6):** Moderate compression suitable for mix bus and general dynamic control
- **High values (6-8.5):** Heavy compression, noticeable gain reduction and tonal change
- **Extreme values (8.5-10):** Severely dynamic sound with organic quality even at aggressive settings

**Typical Values:**

- Mix bus (transparent glue): 1.5-3
- Drum bus (punch and control): 4-6
- Individual kick drum: 5-7
- Individual snare: 4.5-6.5
- Extreme effect/character: 8-9.5
- Peak limiting mode: 9-10 with high Speed setting

**Interaction Notes:** Makeup gain is automatically applied internally - "tends to constrain sound rather than inflate it." The compression is stereo-linked, taking the highest level from either L/R channel to avoid stereo image pumping. At extreme settings with high Speed, creates "artifacts as part of the sound of the compressor."

---

## Parameter B: Speed

**UI Label:** Speed

**Function:** Controls the attack and release time of the compressor, from slow, smooth compression to fast, aggressive pumping.

**Display Range:** 0 to 10

**Default Value:** 2.0

**Sound Character:**

- **Low values (0-2.5):** Very slow attack/release - "really stepping on it a lot," retains soundstage and relative volumes, smooth and natural
- **Medium values (2.5-6):** Moderate timing, musical compression suitable for most sources
- **High values (6-8.5):** Fast response, more upfront character, emphasizes room/ambience
- **Maximum (8.5-10):** "Keeping it all as loud as possible at all times," extremely fast response approaching peak limiting behavior, can create pumping artifacts

**Typical Values:**

- Mix bus (smooth glue): 1-3
- Vocals (natural): 2.5-4.5
- Drum bus (controlled): 3.5-5.5
- Kick drum (punchy): 4-6
- Snare (aggressive): 5.5-7.5
- Peak limiting/extreme effect: 8-10

**Interaction Notes:** At maximum Speed settings with high Pressure, the compressor can "drop out between notes, sometimes drop out between waveform cycles" creating characteristic artifacts. Turning Speed down causes it to "sit on its very heavily compressed level for a while." Higher Speed settings make the "environment of those loud bits stand out a little bit more." The release calculation is sample-rate compensated.

---

## Parameter C: Mewiness

**UI Label:** Mewiness

**Function:** Controls the compression character and ratio behavior, from inverse/parallel-like compression through normal compression to variable-ratio "very mu" style compression.

**Display Range:** -10 to +10

**Default Value:** 0

**Sound Character:**

**Negative Values (-10 to 0):** "Inverse" compression behavior

- Retains attack transient punch while still compressing
- Similar to parallel compression effect: "letting through peaks" while compressed sound happens "behind everything"
- Best for: Maintaining drum transients, adding body without losing attack

**Center (0):** Normal compression behavior

- Standard fixed-ratio compression response
- Most transparent and predictable
- Best for: General-purpose compression, mix bus

**Positive Values (0 to +10):** "Very mu" style compression

- Variable ratio that responds to signal level
- Creates "shortening effect" on drums
- Best for: Aggressive compression, controlling peaks, vintage character

**Typical Values:**

- Transparent mix bus: -1 to +1
- Kick drum (punch retention): -4 to -6
- Snare (natural): -2 to 0
- Drum bus (controlled): +2 to +4
- Vocal (smooth leveling): +3 to +5
- Aggressive limiting: +6 to +10

**Interaction Notes:** This compressor offers a unique approach to compression character with negative ratios for parallel-like effects without actual parallel routing. At positive extremes with high Pressure/Speed, creates heavily squished, "crushed up" character. At negative settings, creates the parallel compression-like effect while maintaining simplicity of a single instance.

---

## General Usage Notes for Mixing Engineers

**Unique Characteristics:**

1. **Stereo-Linked Intelligence:** Unlike simple stereo linking, this compressor tracks the highest level between L/R channels sample-by-sample, preventing stereo image disturbance while avoiding channel dropouts.

2. **Built-in Saturation Safety:** Second-stage sine saturation prevents digital overs even at extreme settings, allowing "bloody loud extremeness" safely.

3. **No Traditional Attack Control:** Speed controls both attack and release together in a musical relationship - you can't set them independently.

4. **Organic Sound:** "Pretty rich warm sounding compressor" with "transparency" and "organic quality" even at extreme settings.

5. **Artifact-as-Feature:** At extreme Speed/Pressure settings, the compression artifacts become part of the character rather than flaws.

**Recommended Workflow:**

1. **Start Conservative:** Begin with Pressure around 3-4, Speed around 4-5 (default is 2), Mewiness at 0
2. **Set Pressure:** Adjust to taste for desired amount of compression
3. **Adjust Speed:** Faster for more aggression, slower for smoothness
4. **Dial Mewiness:** Negative for punch, positive for smoothness/control
5. **Set Output:** Adjust output gain for gain staging

**Best For:**

- Mix bus glue compression
- Drum bus control with character
- Individual drum shaping (especially kick/snare)
- Aggressive peak control without harshness
- Creative effects with extreme settings

**Avoid:**

- Mastering (use gentler settings or dedicated mastering compressor)
- When you need independent attack/release control
- When you need precise ratio control (Mewiness is more about flavor than exact ratios)

**Integration with Channel Strip:**
Position after EQ in your signal chain. The compression will respond to your EQ moves, and the built-in saturation provides final safety limiting. For parallel compression effect, use negative Mewiness settings rather than actual parallel routing.
