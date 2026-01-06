# Parametric EQ - High Mid Band

## Parameter D: High Mid Frequency

**UI Label:** High-Mid Freq

**Function:** Sets the center frequency for the high-midrange parametric band, covering the critical presence and attack range.

**Display Range:** 0 to 10

**Default Value:** 5.0

**Sound Character & Typical Values:**

- **0-2:** Body and warmth region (approximately 600-1000 Hz)

  - Acoustic guitar body
  - Vocal chest/thickness

- **2-4:** Presence foundation (approximately 1000-2000 Hz)

  - Vocal clarity and forward position
  - Guitar honk/nasal tones (often cut)

- **4-6:** Core presence range (approximately 2000-3500 Hz)

  - Vocal presence and intelligibility
  - Guitar "chug" and attack
  - Snare drum body

- **6-8:** Upper presence/definition (approximately 3500-5000 Hz)

  - Vocal clarity and cut-through
  - Guitar pick attack and definition
  - Cymbals and hi-hat stick attack

- **8-10:** Brilliance and edge (approximately 5000-7000 Hz)
  - Vocal sibilance and air
  - Guitar string brightness
  - Snare snap and crack

**Interaction Notes:** This is the most frequently used EQ band in mixing as it contains the fundamental presence range where most instruments compete for space. The cubic frequency curve provides more precise control in the critical upper range where small changes have big impact.

---

## Parameter E: High Mid Gain

**UI Label:** High-Mid Gain

**Function:** Boosts or cuts the selected high-mid frequency, with enhanced boost range for adding presence and aggression.

**Display Range:** -10 to +10

**Default Value:** 0

**Sound Character:**

**Cutting (-10 to -1):**

- **Small cuts (-1 to -3):** Subtle de-emphasis, clearing space for other instruments
- **Medium cuts (-4 to -6):** Removing harshness, boxiness, or resonances
- **Large cuts (-7 to -10):** Surgical removal of problem frequencies

**Boosting (+1 to +10):**

- **Small boosts (+1 to +3):** Adding clarity and presence without obvious EQ character
- **Medium boosts (+4 to +6):** Bringing instruments forward in the mix, emphasizing attack
- **Large boosts (+7 to +10):** Aggressive "seeking" during mixing to find problem or sweet-spot frequencies

**Typical Values:**

- **Vocals (presence):** +2 to +4 around setting 4-6 for clarity
- **Vocals (harshness reduction):** -2 to -6 around setting 2-4 if nasal
- **Guitar (attack enhancement):** +3 to +6 around setting 4-6 for "chug"
- **Guitar (clearing for vocals):** -3 to -6 in the presence range
- **Snare (body):** +2 to +5 around setting 3-5
- **Snare (crack):** +3 to +6 around setting 7-8
- **Mix bus (presence):** ±1 to ±2 maximum (very subtle)
- **Frequency hunting:** +8 to +10 while sweeping to locate problem areas, then cut

**Interaction Notes:** This parameter has a special "nonlinear" quality that makes it respond dynamically to the signal level, similar to analog console EQs. The enhanced boost range is specifically designed for frequency-hunting workflow: boost heavily while sweeping the frequency control to find harsh or resonant areas, then cut them; or find sweet spots and apply gentle boosts.

---

## Parameter F: High Mid Q

**UI Label:** High-Mid Q

**Function:** Controls the bandwidth of the high-mid band from wide/gentle to narrow/surgical.

**Display Range:** 0 to 10

**Default Value:** 5.0

**Q Behavior:**

- **Low values (0-3):** Wide, gentle curves affecting a broad frequency range
- **Medium values (4-6):** Moderate bandwidth, musical EQ adjustments
- **High values (7-10):** Narrow, surgical cuts or boosts targeting specific frequencies

**Sound Character & Usage:**

**Wide Q (0-3):**

- Broad tonal shaping that sounds natural and musical
- Use for: General presence boosts, overall brightness adjustments
- Example: Adding 2-3 dB of presence to vocals across a wide range
- Affects multiple instruments simultaneously in a complementary way

**Medium Q (4-6):**

- Balanced between surgical and musical
- Use for: Instrument-specific character adjustments
- Example: Boosting guitar attack without making it sound "EQ'd"
- Good starting point for most mixing tasks

**Narrow Q (7-10):**

- Surgical precision for problem frequencies or specific resonances
- Use for: Removing ringing, harshness, or feedback-prone frequencies
- Example: Cutting a specific vocal resonance by -6 dB
- Finding and removing single problem frequency in snare drum

**Typical Values:**

- **Vocals (presence boost):** 3-5 (moderate width) for natural enhancement
- **Vocals (resonance removal):** 7-9 (narrow) to surgically remove harshness
- **Guitar (character enhancement):** 4-6 (medium) for musical attack boost
- **Guitar (clearing for vocals):** 3-5 (moderate) for broad cut
- **Snare (body boost):** 3-5 (moderate width)
- **Problem frequency removal:** 8-10 (very narrow) for surgical cuts
- **Frequency hunting:** 6-9 (narrow to medium) when boosting heavily to locate issues

**Interaction Notes:** Tightening the Q helps isolate specific frequency characteristics. Use narrow Q when cutting problems and wider Q when boosting for enhancement. The resonance calculation is affected by both the frequency and gain settings, creating a dynamic relationship where the Q responds slightly differently depending on your other control settings.

---

## General Mixing Workflow

**The Frequency-Hunting Method:**

1. **Find Problems:** Set Gain to maximum boost (+10), Reso moderately narrow (6-7), sweep Frequency control to find harsh, annoying, or resonant areas

2. **Remove Problems:** Once found, set Reso narrow (7-9) and apply cut (-3 to -8) to remove harshness while preserving character

3. **Find Sweet Spots:** Keep high gain (+8 to +10), sweep to find frequencies that make the instrument sound better, more present, or more aggressive

4. **Apply Musical Boosts:** Widen Reso slightly (4-6), reduce gain to subtle amounts (+2 to +6) for final musical enhancement

5. **Context Check:** Compare before/after and ensure you haven't over-processed

**Best For:**

- Surgical EQ work in the critical presence range
- Managing frequency competition between vocals, guitars, and snare
- Adding clarity and definition without harshness
- Frequency hunting and presence shaping
