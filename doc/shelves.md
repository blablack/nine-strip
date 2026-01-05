# Shelves

## Parameter A: Low Shelf Frequency

**UI Label:** Low Shelf Freq

**Function:** Sets the center frequency for the low-shelf EQ band, controlling the transition point for bass and low-midrange shaping.

**Recommended Range:** 20 Hz to 500 Hz

**Sound Character & Typical Values:**

- **20-50 Hz:** Sub-bass region
  - Deep low end and rumble control
  - Useful for removing subsonic content or adding weight to bass instruments
  
- **50-100 Hz:** Bass fundamental region
  - Kick drum body and warmth
  - Bass guitar fundamental presence
  
- **100-200 Hz:** Lower midrange warmth
  - Vocal body and chest resonance
  - Guitar low-end fullness
  
- **200-500 Hz:** Upper low-midrange
  - Overall warmth and body for instruments
  - Boxiness and muddiness region (often cut)

**Interaction Notes:** Works in conjunction with Gain and Q controls to shape the low end. Shelf filters affect all frequencies below the cutoff point, not just the center frequency like parametric EQ.

***

## Parameter B: Low Shelf Gain

**UI Label:** Low Shelf Gain

**Function:** Boosts or cuts the low-shelf frequency region.

**Recommended Range:** -∞ dB to +18 dB

**Sound Character:**

**Cutting:**
- **-1 to -3 dB:** Subtle reduction of bass weight, clearing muddiness
- **-4 to -8 dB:** Noticeable thinning, removing boxiness
- **-10 dB and below:** Aggressive bass removal, extreme thinning

**Boosting:**
- **+1 to +3 dB:** Adding subtle warmth and body
- **+4 to +8 dB:** Noticeable bass enhancement, increased presence
- **+10 dB and above:** Aggressive bass boost, potential for rumble

**Typical Values:**
- Kick drum warmth: +2 to +5 dB around 80-120 Hz
- Bass guitar body: +1 to +4 dB around 100-150 Hz
- Vocal warmth: +1 to +3 dB around 100-150 Hz
- Mix bus: ±1 to ±2 dB maximum
- Rumble removal: -2 to -6 dB around 20-50 Hz

***

## Parameter C: Low Shelf Q

**UI Label:** Low Shelf Q

**Function:** Controls the transition curve of the low-shelf filter from gradual to steep.

**Recommended Range:** 0.0 to 1.0

**Sound Character:**

**Low Q (0.0-0.3):**
- Gentle, gradual transition affecting a wide frequency range
- Natural-sounding tonal adjustments
- Use for: General warmth adjustments, transparent bass shaping

**Medium Q (0.4-0.6):**
- Moderate transition slope
- Balanced between broad and focused
- Use for: Instrument-specific low-end shaping

**High Q (0.7-1.0):**
- Steeper transition with more focused effect
- More concentrated on the shelf frequency
- Use for: Precise low-end control

**Typical Values:**
- General warmth: 0.3-0.5
- Instrument-specific shaping: 0.4-0.6
- Precise low-end control: 0.7-0.9

***

## Parameter D: High Shelf Frequency

**UI Label:** High Shelf Freq

**Function:** Sets the center frequency for the high-shelf EQ band, controlling the transition point for brightness and air region shaping.

**Recommended Range:** 2 kHz to 20 kHz

**Sound Character & Typical Values:**

- **2-4 kHz:** Upper midrange presence
  - Vocal clarity and attack
  - Instrument definition
  
- **4-8 kHz:** Presence and attack region
  - Snare crack and brightness
  - Guitar string attack
  - Vocal clarity and presence
  
- **8-15 kHz:** Brilliance and air
  - Cymbal and hi-hat brightness
  - Overall "air" and clarity
  - Vocal sibilance region
  
- **15-20 kHz:** Ultra-high air
  - Extremely subtle brightness enhancement
  - "Air" and spaciousness in stereo mixes

**Interaction Notes:** Shelf filters at high frequencies create a gentle brightness adjustment that becomes progressively more subtle as the shelf frequency increases above 10 kHz.

***

## Parameter E: High Shelf Gain

**UI Label:** High Shelf Gain

**Function:** Boosts or cuts the high-shelf frequency region and above.

**Recommended Range:** -∞ dB to +18 dB

**Sound Character:**

**Cutting:**
- **-1 to -3 dB:** Subtle de-brightening, reducing harshness
- **-4 to -8 dB:** Noticeable darkening, de-essing effect
- **-10 dB and below:** Aggressive darkening, vintage/warm character

**Boosting:**
- **+1 to +3 dB:** Adding subtle brightness and air
- **+4 to +8 dB:** Noticeable brightness enhancement, increased presence
- **+10 dB and above:** Aggressive brightening, potential for harshness

**Typical Values:**
- Vocal brightness: +1 to +4 dB around 5-8 kHz
- Mix bus air/clarity: +1 to +2 dB around 10-15 kHz
- De-essing (cutting): -2 to -6 dB around 6-10 kHz
- Brightness enhancement: +2 to +5 dB around 4-8 kHz
- Vintage warmth (cutting): -3 to -8 dB around 8-12 kHz

***

## Parameter F: High Shelf Q

**UI Label:** High Shelf Q

**Function:** Controls the transition curve of the high-shelf filter from gradual to steep.

**Recommended Range:** 0.0 to 1.0

**Sound Character:**

**Low Q (0.0-0.3):**
- Gentle, gradual transition affecting a wide frequency range
- Natural, musical brightness adjustments
- Use for: General air adjustments, transparent high-end shaping

**Medium Q (0.4-0.6):**
- Moderate transition slope
- Balanced between broad and focused
- Use for: Instrument-specific high-end shaping

**High Q (0.7-1.0):**
- Steeper transition with more focused effect
- More concentrated around the shelf frequency
- Use for: Precise brightness control and de-essing

**Typical Values:**
- General air/brightness: 0.3-0.5
- Instrument-specific shaping: 0.4-0.6
- De-essing (cutting): 0.6-0.8
- Precise brightness control: 0.7-0.9

***

## General Usage Notes

**Shelf vs. Parametric EQ:**
- Shelves affect all frequencies above (high shelf) or below (low shelf) the cutoff frequency
- Parametric bands only affect a narrow range around the center frequency
- Use shelves for broad tonal shaping; use parametrics for targeted problem-solving

**Typical Workflow:**
1. Use low shelf for overall bass balance and warmth
2. Use high shelf for overall brightness and air
3. Use the parametric band for specific frequency problems or enhancements
4. Combine for comprehensive tonal shaping from bass to treble

**Integration with Channel Strip:**
Position Shelves early in the EQ section for broad tonal shaping. The shelves work well before the parametric band, allowing the parametric to handle detailed problem-solving and character adjustments.
