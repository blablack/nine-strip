# Highpass

## Parameter A: Frequency

**UI Label:** Frequency (or Cutoff)

**Function:** Sets the cutoff frequency where the highpass filter begins removing low-frequency content.

**Recommended Range:** 0-100% (normalized control mapping to approximately 20 Hz - 20 kHz range)

**Sound Character:**

- **Low values (0-30%):** Removes only deep sub-bass, cleans up rumble and DC offset
- **Medium values (30-60%):** Progressively removes more low end, useful for thinning out bass-heavy sources
- **High values (60-100%):** Aggressive low-end removal, can make sources sound thin or "tinny"

**Typical Values:**

- Sub-bass cleanup: 5-15% (approximately 30-80 Hz)
- Vocal de-muddying: 20-35% (approximately 100-150 Hz)
- Guitar clarity: 25-40% (approximately 120-180 Hz)
- Mix bus rumble removal: 5-10% (approximately 20-40 Hz)

**Interaction Notes:** This is NOT a standard digital highpass - it also gently rolls off extreme highs above ~16kHz, giving a more analog, organic character compared to brick-wall digital filters. The actual frequency response is affected by both the Frequency and Poles settings.

***

## Parameter B: Response

**UI Label:** Response (or Dynamics)

**Function:** Controls how the filter responds to signal level, creating dynamic filtering behavior.

**Recommended Range:** -100% to +100% (displayed as -1.0 to 1.0)

**Sound Character:**

- **Negative values (Loose):** Filter affects all signal levels more evenly, more consistent filtering behavior
- **Center (0%):** Balanced, neutral response
- **Positive values (Tight):** Filter responds more to louder signals; quiet passages get less filtering, preserving natural decay and ambience

**Typical Values:**

- Transparent filtering: -20% to +20%
- Dynamic bass control on drums: +40% to +70%
- Consistent vocal filtering: -30% to 0%
- Special effects: Extreme settings (-100% or +100%)

**Interaction Notes:** Extreme settings can cause unusual behavior, especially with silence or very quiet signals. For normal mixing use, stay within ±50%. At maximum tightness (+100%), near-silent passages may pass through unfiltered, which can sound unexpected when the filter "kicks in" on transients. This parameter modulates the filter strength based on input amplitude.

***

## Parameter C: Poles

**UI Label:** Slope (or Poles)

**Function:** Sets filter steepness by engaging multiple cascaded filter stages, from gentle 1-pole (6dB/octave) to steep 4-pole (24dB/octave).

**Recommended Range:** 0 to 4 (displayed directly as pole count)

**Sound Character:**

- **0-1 (Single pole):** Very gentle, musical rolloff - subtle low-end control, maintains natural tone
- **1-2 (Two poles):** Moderate slope - good for general-purpose highpass filtering
- **2-3 (Three poles):** Steeper rolloff - more aggressive low-end removal
- **3-4 (Four poles):** Maximum steepness - surgical low-end cuts, can sound more "digital"

**Typical Values:**

- Gentle warming/cleanup: 0.5-1.5
- Standard vocal/instrument filtering: 1.5-2.5
- Aggressive bass removal: 3.0-4.0
- Mix bus: 0.5-1.0 (subtle)

**Interaction Notes:** This control progressively adds filter stages. Each increment makes the filter slope steeper. Lower pole counts sound more transparent and analog-like; higher pole counts give more precise control but can sound harsher. The interleaved filter topology means you also get some subtle high-frequency rolloff, increasing slightly with more poles engaged.

***

## General Usage Notes for Mixing Engineers

**Analog Character:** Unlike standard DAW highpass filters, this highpass has built-in "analog-style" qualities:
- Gentle rolloff of extreme highs (>16kHz) that increases with more poles engaged
- Creates a warmer, less "digital" sound
- Particularly noticeable when using higher pole counts

**Typical Workflow:**
1. Set Frequency to your desired cutoff point
2. Set Poles to 2.0 for standard use
3. Leave Response at 0% for consistent filtering
4. Adjust Response if you need dynamic filtering behavior

---

# Lowpass

## Parameter A: Frequency

**UI Label:** Frequency (or Cutoff)

**Function:** Sets the cutoff frequency where the lowpass filter begins removing high-frequency content.

**Recommended Range:** 0-100% (normalized control mapping to approximately 20 Hz - 20 kHz range)

**Sound Character:**

- **Low values (0-30%):** Aggressive darkening, removes most highs and upper mids - useful for extreme filtering effects
- **Medium values (30-70%):** Moderate tone shaping, tames brightness while maintaining clarity
- **High values (70-100%):** Subtle high-frequency control, maintains brightness while gently rolling off extreme highs

**Typical Values:**

- Extreme darken/vintage effect: 10-25%
- Vocal warmth/de-essing: 60-80%
- Mix bus air control: 85-95%
- Sound design wind/outdoor effects: 20-50%

**Interaction Notes:** This is NOT a standard digital lowpass - it uses an interleaved IIR design that preserves super-high frequencies better than typical filters. At 44.1kHz, content near 22kHz won't be heavily affected even with moderate settings, giving this filter unusual clarity and transparency. The actual steepness depends heavily on the Poles setting.

***

## Parameter B: Response

**UI Label:** Response (or Character)

**Function:** Controls how the filter responds dynamically to signal level, creating level-dependent filtering behavior.

**Recommended Range:** -100% to +100% (displayed as -1.0 to 1.0)

**Sound Character:**

- **Negative values (Soft -100% to 0%):** Louder transients and peaks get filtered MORE aggressively; quieter sounds stay brighter - creates a "compressing" effect on brightness, evening out tonal balance
- **Center (0%):** Neutral, consistent filtering regardless of level
- **Positive values (Hard 0% to +100%):** Louder transients PUNCH THROUGH the filter with more highs; quieter sounds get darker - emphasizes attack and transients

**Typical Values:**

- Smooth, even tone: -30% to -50% (Soft)
- Neutral/transparent: 0%
- Emphasize drum transients: +40% to +70% (Hard)
- Extreme sound design effects: ±80% to ±100%

**Interaction Notes:** This operates on a sample-by-sample basis, creating subtle integrated dynamic filtering rather than envelope-follower style effects. With higher Poles settings, this parameter becomes more dramatic and can create unusual distortion/breakup effects. Extreme Soft settings with 4 poles can create "thumping" and unusual tonal shifts. Extreme Hard settings create "spiky" transients with "crackly" character. For sound design, Hard settings on noise create wind/hurricane effects; Soft settings create smoother wind-like sounds.

***

## Parameter C: Poles

**UI Label:** Slope (or Poles)

**Function:** Sets filter steepness by engaging cascaded filter stages, from gentle 1-pole to steep 4-pole response.

**Recommended Range:** 0 to 4 (continuous, representing 1-pole through 4-pole)

**Sound Character:**

- **0-1 (Single pole):** Very gentle, musical rolloff (~6dB/octave) - transparent and subtle
- **1-2 (Two poles):** Moderate slope (~12dB/octave) - more noticeable filtering, still musical
- **2-3 (Three poles):** Steep rolloff (~18dB/octave) - pronounced darkening effect
- **3-4 (Four poles):** Maximum steepness (~24dB/octave) - surgical tone shaping, striking effects

**Typical Values:**

- Gentle tone shaping/mix bus: 0.5-1.5
- Standard instrument filtering: 1.5-2.5
- Aggressive darkening: 3.0-4.0
- Sound design effects: 3.5-4.0 (especially with extreme Response settings)

**Interaction Notes:** The Poles control works progressively - from 0-1 it acts like a dry/wet control for a single pole filter. Beyond 1.0, each additional stage adds another pole of filtering. Lower pole counts sound more transparent; higher pole counts create more dramatic effects but can also produce distortion artifacts when combined with extreme Response settings.

***

## General Usage Notes for Mixing Engineers

**Unique Characteristics:**
Unlike standard DAW lowpass filters, this lowpass has several distinctive qualities:

1. **Interleaved IIR topology:** Preserves extreme high frequencies better than typical filters, giving unusual clarity even when filtering aggressively

2. **Level-dependent response:** The Response parameter creates dynamic filtering that changes with signal level on a sample-by-sample basis

3. **Sound design capabilities:** Extreme settings (especially 4 poles + extreme Response) can create wind effects, unusual textures, and specialized sounds useful for sound design

**Recommended Workflow:**
1. Start with Frequency at your desired cutoff point
2. Set Poles to 2.0 for standard use (12dB/octave)
3. Leave Response at 0% for predictable, consistent filtering
4. Experiment with Response settings for dynamic tone shaping or special effects

**Best For:**
- Transparent tone darkening with unusual clarity
- Dynamic filtering that responds to transient content
- Sound design (wind, outdoor ambience, texture generation)
- Creative effects when combined with other processing

**Avoid:**
Extreme Response settings (±80-100%) with 4 poles can cause distortion artifacts - great for sound design, but use cautiously in standard mixing contexts.