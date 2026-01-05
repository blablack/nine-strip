# Saturation

## Parameter A: Console Type

**UI Label:** Console Type

**Function:** Selects the analog console emulation character, affecting frequency response, transient handling, and overall tonal color through different slew clipping curves and filter responses.

**Recommended Range:** Five discrete options (Neve, API, SSL, Teac, Mackie)

**Typical Values:**

- **Neve:** Large format console sound - extended highs, slight presence lift in ultrasonics, fuller low end, slightly darker overall character. Best for tracking, vocals, acoustic instruments where you want that "big record" sound
- **API:** Similar to Neve but with slightly less extended top end - great all-rounder for drums, bass, and mix bus processing
- **SSL:** Large format desk with more controlled top end - classic mix bus glue, works well on full mixes and groups
- **Teac:** Budget mixer character - darker tone, more aggressive slew clipping, reduced extreme lows. Good for electronic music, lo-fi aesthetic, or taming harsh digital synths
- **Mackie:** Prosumer board - brighter than Teac but still limited bass response. Excellent for French house, electronic productions, or vintage sampler aesthetic

**Interaction Notes:** Console type affects how the Drive parameter saturates and clips. Budget consoles (Teac/Mackie) have more aggressive slew clipping with cheaper op-amp characteristics, while large format types (Neve/API/SSL) are more transparent and musical.

***

## Parameter B: Drive

**UI Label:** Drive

**Function:** Controls saturation intensity through two distinct stages - clean "Spiral" saturation from 0-100%, transitioning to thicker "Density" saturation from 100-200%.

**Recommended Range:** 0% to 200%

**Typical Values:**

- **0-50%:** Subtle analog warmth and glue - use on individual tracks where you want gentle character without obvious saturation
- **50-100%:** Increasing Spiral saturation - smooth, clean enhancement that doesn't add excessive thickness. Good starting point for vocals, acoustic guitars, mix bus
- **100-150%:** Blending Spiral and Density - adds body and presence while maintaining clarity. Works well on drums, bass, lead instruments
- **150-200%:** Full Density mode - maximum thickness and harmonic richness. Use sparingly on individual elements or for aggressive character on synths and drums

**Sound Character:**

- **0-100% range:** Crossfades into "Spiral" algorithm - very clean, smooth saturation that enhances without obvious distortion. Lets transients through while adding harmonic content
- **100-200% range:** Crossfades from Spiral into "Density" - traditional fuller saturation with more obvious harmonic thickness and weight

**Interaction Notes:** The Drive parameter works in conjunction with Console Type - budget console types (Teac/Mackie) will clip more aggressively at higher Drive settings, while large format types remain more transparent. The internal slew clipping acts like op-amp limitations, naturally controlling transients.

