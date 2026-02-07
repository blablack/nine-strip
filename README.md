# Nine Strip

A VST3 channel strip plugin built with JUCE framework, incorporating classic Airwindows processing algorithms into a comprehensive mixing tool.

![Screenshot](pics/screenshot.png)

## Overview

NineStrip is a complete channel strip solution that combines console modeling, filtering, EQ, and dynamics processing in a single plugin. Built on proven Airwindows algorithms, it provides a streamlined workflow for mixing and mastering tasks.

## Signal Chain

```
PURESTGAIN - Input Gain
    ↓
CHANNEL9 - Console model + Saturation + Drive
    ↓
HIGHPASS2 - High Pass
    ↓
LOWPASS2 - Low Pass
    ↓
BAXANDALL2 - Bass and Treble Shelves
    ↓
PARAMETRIC - Mid Band
    ↓
PRESSURE4 - Compressor
    ↓
PURESTGAIN - Output Gain
```

## Features

- Console Modeling: Channel9 provides authentic analog console character with saturation and drive
- Flexible Filtering: User-controlled high-pass and low-pass filters for precise frequency shaping
- Three-Band EQ: Bass and treble shelves plus parametric midrange control using Baxandall topology
- Dynamics Processing: Pressure4 compression for transparent gain control
- Input/Output Gain Staging: Complete control over signal levels throughout the chain

## Building from Source

```bash
git clone --recurse-submodules https://github.com/blablack/nine-strip.git
cd nine-strip
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
ninja -C build
```

### Compiling documentation

```bash
cd doc
pandoc NineStrip_Manual.md -o NineStrip_Manual.pdf --pdf-engine=xelatex
```

## Usage

Load NineStrip on any channel where you want complete processing control. The signal flows from top to bottom through each stage, allowing you to shape tone, dynamics, and character in a single pass.


## Credits

This plugin incorporates processing algorithms from [Airwindows](https://www.airwindows.com/) by Chris Johnson. Massive thanks to Chris for his open-source contributions to the audio community.

The UI look and feel is based on the 3D knob from [SoundDevelopment/3D_knob_lnf](https://github.com/SoundDevelopment/3D_knob_lnf) (MIT License).

## License

This project is licensed under the GNU Affero General Public License v3.0 (AGPL-3.0) due to the use of the JUCE framework under its open-source license terms.

## License Notes

- JUCE Framework: Used under GPL/AGPL license terms
- Airwindows algorithms: MIT License (compatible with AGPL-3.0)
- 3D Knob LnF: MIT License (compatible with AGPL-3.0)
- NineStrip: AGPL-3.0

See the [LICENSE](./LICENSE) file for full details.

## Contributing

Contributions are welcome! Please submit pull requests or open issues for bugs and feature requests.
Report issues or request features at [GitHub Issues](https://github.com/blablack/nine-strip/issues).



