# Shelves (Baxandall2)

The Baxandall2 is a classic tone control circuit inspired by the Baxandall equalizer found in vintage hi-fi equipment. It provides two shelving filters with fixed Q values and automatically adjusting corner frequencies that track musically with gain adjustments.

**Important:** This is NOT a parametric shelving EQ. Frequency and Q values are fixed and adjust automatically based on gain settings for musical behavior.

---

## Parameter A: High Shelf

**UI Label:** High Shelf

**Function:** Controls high-frequency shelf gain with an automatically adjusting corner frequency that becomes higher as you boost.

**Display Range:** -10 to +10

**Default Value:** 0

**Technical Details:**

- **Base corner frequency:** ~4.4 kHz at 44.1 kHz sample rate (scales with sample rate)
- **Frequency behavior:** `(4410.0 * linearGain) / sampleRate` - frequency increases with boost, decreases with cut
- **Q value:** Fixed at 0.4 (gentle, musical slope)
- **Filter type:** Second-order lowpass-based shelf affecting frequencies above the corner

**How It Works:**
The Baxandall design automatically adjusts the shelf frequency based on gain:

- **Boosting (+):** Corner frequency moves higher, creating a gentler, more natural boost
- **Cutting (-):** Corner frequency moves lower, creating broader darkening effect
- **Unity (0):** Corner around 4.4 kHz baseline

**Sound Character:**

**Cutting (-10 to 0):**

- **-1 to -3:** Subtle de-brightening, gentle warmth
- **-4 to -6:** Noticeable darkening, vintage character, reducing harshness
- **-7 to -9:** Significant darkening, lo-fi aesthetic, muffled character
- **-10:** Extreme darkening, telephone/AM radio effect

**Neutral (0):**

- Transparent passthrough with no tonal coloration

**Boosting (0 to +10):**

- **+1 to +3:** Subtle air and clarity enhancement
- **+4 to +6:** Noticeable brightness, increased presence and sparkle
- **+7 to +9:** Strong brightness, enhanced attack and detail
- **+10:** Extreme brightness (use carefully - potential harshness)

**Typical Values:**

- **Mix bus air:** +0.5 to +2 - adds subtle polish without harshness
- **Vocal brightness:** +2 to +5 - enhances clarity and presence
- **Acoustic guitar sparkle:** +3 to +6 - brings out string detail
- **De-essing/warmth:** -2 to -5 - reduces sibilance and harshness
- **Vintage/warm character:** -4 to -8 - analog warmth, tape-like sound
- **Drum overhead air:** +2 to +4 - cymbal brightness and air
- **Electric guitar cut:** -3 to -6 - taming harsh amp tones

---

## Parameter B: Low Shelf

**UI Label:** Low Shelf

**Function:** Controls low-frequency shelf gain with an automatically adjusting corner frequency that inversely tracks with gain (lower frequency when boosting for more focused low-end enhancement).

**Display Range:** -10 to +10

**Default Value:** 0

**Technical Details:**

- **Base corner frequency:** ~8.8 kHz / gain factor at 44.1 kHz sample rate (scales with sample rate)
- **Frequency behavior:** `(8820.0 * 10^(-dB/20)) / sampleRate` - frequency DECREASES with boost, increases with cut
- **Q value:** Fixed at 0.2 (very gentle, broad slope - gentler than treble)
- **Filter type:** Second-order lowpass-based shelf affecting low frequencies

**How It Works:**
The bass control has inverse frequency tracking for musical bass response:

- **Boosting (+):** Corner frequency moves LOWER, creating focused deep bass enhancement
- **Cutting (-):** Corner frequency moves HIGHER, creating broader bass reduction
- **Unity (0):** Corner around calculated midpoint

This inverse behavior ensures bass boosts feel tight and controlled rather than bloated.

**Sound Character:**

**Cutting (-10 to 0):**

- **-1 to -3:** Subtle reduction of bass weight, slight tightening
- **-4 to -6:** Noticeable thinning, clearing muddiness and boxiness
- **-7 to -9:** Significant bass removal, lightweight character
- **-10:** Extreme thinning, almost no low end (transistor radio effect)

**Neutral (0):**

- Transparent passthrough with no bass coloration

**Boosting (0 to +10):**

- **+1 to +3:** Subtle warmth and body, gentle low-end enhancement
- **+4 to +6:** Noticeable bass boost, increased weight and warmth
- **+7 to +9:** Strong bass enhancement, substantial low-end power
- **+10:** Extreme bass boost (watch for muddiness and speaker stress)

**Typical Values:**

- **Mix bus warmth:** +0.5 to +2 - adds subtle fullness without mud
- **Kick drum body:** +4 to +6 - enhances fundamental and punch
- **Bass guitar warmth:** +2 to +5 - increases presence and weight
- **Vocal body/chest:** +2 to +4 - adds warmth without muddiness
- **Rumble removal:** -3 to -5 - cleans up excessive low end
- **Thin/clear mix:** -2 to -4 - reduces muddiness and boxiness
- **808 bass boost:** +5 to +8 - hip-hop/electronic bass power
- **Acoustic guitar cut:** -2 to -4 - removes boominess

---

## Baxandall Design Characteristics

**Key Features:**

1. **Automatic Frequency Tracking:**

   - Frequencies adjust automatically based on gain for musical behavior
   - High Shelf: Higher frequency when boosting (natural air enhancement)
   - Low Shelf: Lower frequency when boosting (focused low-end power)

2. **Fixed Q Values:**

   - High Shelf Q = 0.4 (moderate gentleness)
   - Low Shelf Q = 0.2 (very gentle, broad)
   - No resonant peaks at corner frequencies
   - Smooth, musical transitions

3. **Sample Rate Compensation:**

   - All frequencies scale proportionally with sample rate
   - Maintains consistent character at 44.1k, 48k, 96k, 192k, etc.

4. **Classic Hi-Fi Lineage:**
   - Based on Peter Baxandall's legendary 1952 tone control circuit
   - Used in countless vintage hi-fi amplifiers and mixing consoles
   - Known for extremely musical, non-fatiguing sound

**What This Is NOT:**

- ❌ Not a parametric EQ with adjustable frequency
- ❌ Not a constant-Q shelving filter
- ❌ Not a "modern" transparent EQ
- ❌ Cannot adjust Q or slope independently

**What This IS:**

- ✅ A vintage-style tone control with automatic frequency adaptation
- ✅ Musical, broad tonal shaping tool
- ✅ Ideal for overall balance and vintage character
- ✅ Excellent for "set and forget" tone adjustments

---

## Usage Notes

**Best Practices:**

1. **Broad Strokes First:**

   - Use Baxandall for overall tonal balance
   - Think of it as a "tone shaper" not a "problem solver"
   - Start with small adjustments (±1-3)

2. **Vintage Character:**

   - Perfect for adding analog warmth (bass boost, treble cut)
   - Great for "British console" sound (slight bass and treble lift)
   - Excellent for retro/lo-fi aesthetics

3. **Complementary to Parametric:**

   - Use Baxandall for general balance
   - Use Parametric EQ for specific problem frequencies
   - Baxandall + Parametric = comprehensive tonal control

4. **Mix Bus Applications:**
   - Very gentle adjustments (±0.5-2)
   - Can add cohesive "glue" and character
   - Less fatiguing than aggressive parametric EQ

**Common Combinations:**

- **Warm Vintage Mix:** Low Shelf +2, High Shelf -3
- **Modern Bright Mix:** Low Shelf -1, High Shelf +2
- **Classic British Console:** Low Shelf +1.5, High Shelf +1
- **Lo-Fi Character:** Low Shelf +3, High Shelf -6
- **Clean/Tight Mix:** Low Shelf -2, High Shelf +1
