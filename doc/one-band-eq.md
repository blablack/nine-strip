# Parametric EQ - High Mid Band

## Parameter D: High Mid Frequency

**UI Label:** High Mid Freq

**Function:** Sets the center frequency for the high-midrange parametric band, covering the critical presence and attack range.

**Recommended Range:** 600 Hz to 7000 Hz (displayed in Hz)

**Frequency Mapping:**
The parameter uses a cubic curve, meaning the lower half of the control covers 600-2000 Hz, while the upper half focuses on 2000-7000 Hz for finer control in the presence region.

**Sound Character & Typical Values:**

- **600-1000 Hz:** Body and warmth region
  - Acoustic guitar body: 700-900 Hz
  - Vocal chest/thickness: 700-1000 Hz

- **1000-2000 Hz:** Presence foundation
  - Vocal clarity and forward position: 1000-1500 Hz
  - Guitar honk/nasal tones (often cut): 1000-1500 Hz

- **2000-3500 Hz:** Core presence range
  - Vocal presence and intelligibility: 2500-4500 Hz
  - Guitar "chug" and attack: 2000-3000 Hz
  - Snare drum body: 2000-3000 Hz

- **3500-5000 Hz:** Upper presence/definition
  - Vocal clarity and cut-through: 3000-5000 Hz
  - Guitar pick attack and definition: 3000-4000 Hz
  - Cymbals and hi-hat stick attack: 4000-5000 Hz

- **5000-7000 Hz:** Brilliance and edge
  - Vocal sibilance and air: 5000-8000 Hz
  - Guitar string brightness: 5000-7000 Hz
  - Snare snap and crack: 5000-6000 Hz

**Interaction Notes:** This is the most frequently used EQ band in mixing as it contains the fundamental presence range where most instruments compete for space. The cubic frequency curve provides more precise control in the critical 2-7 kHz range where small changes have big impact.

***

## Parameter E: High Mid Gain

**UI Label:** High Mid Gain

**Function:** Boosts or cuts the selected high-mid frequency, with enhanced boost range for adding presence and aggression.

**Recommended Range:** -∞ dB to approximately +18 dB (displayed in dB)

**Internal Behavior:**
- 0.0 = maximum cut
- 0.5 = unity/flat (0 dB)
- 1.0 = maximum boost (doubled for extra range)

**Sound Character:**

**Cutting (0.0-0.4):**
- **Small cuts (-1 to -3 dB):** Subtle de-emphasis, clearing space for other instruments
- **Medium cuts (-4 to -8 dB):** Removing harshness, boxiness, or resonances
- **Large cuts (-10 to -18 dB):** Surgical removal of problem frequencies

**Boosting (0.6-1.0):**
- **Small boosts (+1 to +3 dB):** Adding clarity and presence without obvious EQ character
- **Medium boosts (+4 to +8 dB):** Bringing instruments forward in the mix, emphasizing attack
- **Large boosts (+10 to +18 dB):** Aggressive "seeking" during mixing to find problem or sweet-spot frequencies

**Typical Values:**

- **Vocals (presence):** +2 to +4 dB around 3000-4000 Hz for clarity
- **Vocals (harshness reduction):** -2 to -6 dB around 2000-3000 Hz if nasal
- **Guitar (attack enhancement):** +3 to +6 dB around 2500-3500 Hz for "chug"
- **Guitar (clearing for vocals):** -3 to -6 dB around 2000-5000 Hz
- **Snare (body):** +2 to +5 dB around 2000-3000 Hz
- **Snare (crack):** +3 to +6 dB around 5000 Hz
- **Mix bus (presence):** ±1 to ±2 dB maximum (very subtle)
- **Frequency hunting:** +12 to +18 dB while sweeping to locate problem areas, then cut

**Interaction Notes:** This parameter has a special "nonlinear" quality that makes it respond dynamically to the signal level, similar to analog console EQs. The enhanced boost range (up to +18 dB) is specifically designed for frequency-hunting workflow: boost heavily while sweeping the frequency control to find harsh or resonant areas, then cut them; or find sweet spots and apply gentle boosts.

***

## Parameter F: High Mid Q

**UI Label:** High Mid Q

**Function:** Controls the bandwidth of the high-mid band from wide/gentle to narrow/surgical.

**Recommended Range:** 0.0 to 1.0 (can be displayed as Q value or descriptively as "Wide" to "Narrow")

**Q Behavior:**
- **Low values (0.0-0.3):** Wide, gentle curves affecting a broad frequency range
- **Medium values (0.4-0.6):** Moderate bandwidth, musical EQ adjustments
- **High values (0.7-1.0):** Narrow, surgical cuts or boosts targeting specific frequencies

**Sound Character & Usage:**

**Wide Q (0.0-0.3):**
- Broad tonal shaping that sounds natural and musical
- Use for: General presence boosts, overall brightness adjustments
- Example: Adding 2-3 dB of presence to vocals across 3-5 kHz range
- Affects multiple instruments simultaneously in a complementary way

**Medium Q (0.4-0.6):**
- Balanced between surgical and musical
- Use for: Instrument-specific character adjustments
- Example: Boosting guitar attack around 3 kHz without making it sound "EQ'd"
- Good starting point for most mixing tasks

**Narrow Q (0.7-1.0):**
- Surgical precision for problem frequencies or specific resonances
- Use for: Removing ringing, harshness, or feedback-prone frequencies
- Example: Cutting a specific vocal resonance at 2400 Hz by -6 dB
- Finding and removing single problem frequency in snare drum

**Typical Values:**

- **Vocals (presence boost):** 0.3-0.5 (moderate width) around 3-4 kHz
- **Vocals (resonance removal):** 0.7-0.9 (narrow) to surgically remove harshness
- **Guitar (character enhancement):** 0.4-0.6 (medium) for musical attack boost
- **Guitar (clearing for vocals):** 0.3-0.5 (moderate) for broad cut in 2-5 kHz
- **Snare (body boost):** 0.3-0.5 (moderate width)
- **Problem frequency removal:** 0.8-1.0 (very narrow) for surgical cuts
- **Frequency hunting:** 0.6-0.9 (narrow to medium) when boosting heavily to locate issues

**Interaction Notes:** Tightening the Q helps isolate specific frequency characteristics. Narrow Q when cutting problems and wider Q when boosting for enhancement. The resonance calculation is affected by both the frequency and gain settings, creating a dynamic relationship where the Q responds slightly differently depending on your other control settings.

***

## General Mixing Workflow

**The Frequency-Hunting Method:**

1. **Find Problems:** Set Gain to maximum boost (+18 dB), Q moderately narrow (0.6-0.7), sweep Frequency control to find harsh, annoying, or resonant areas

2. **Remove Problems:** Once found, set Q narrow (0.7-0.9) and apply cut (-3 to -12 dB) to remove harshness while preserving character

3. **Find Sweet Spots:** Keep high gain (+12 to +18 dB), sweep to find frequencies that make the instrument sound better, more present, or more aggressive

4. **Apply Musical Boosts:** Widen Q slightly (0.4-0.6), reduce gain to subtle amounts (+2 to +6 dB) for final musical enhancement

5. **Context Check:** Use the Dry/Wet control to compare before/after and ensure you haven't over-processed

**Best For:**
- Surgical EQ work in the critical presence range (2-7 kHz)
- Managing frequency competition between vocals, guitars, and snare
- Adding clarity and definition without harshness
- Frequency hunting and presence shaping

