# Nine Strip

A VST3 channel strip plugin built with JUCE framework, incorporating classic Airwindows processing algorithms into a comprehensive mixing tool. Also available as a standalone application.

![Screenshot](pics/screenshot.png)

## Overview

NineStrip is a complete channel strip solution that combines console modeling, filtering, EQ, and dynamics processing in a single plugin. Built on proven Airwindows algorithms, it provides a streamlined workflow for mixing and mastering tasks.

**Available formats:**
- VST3 plugin (for use in DAWs)
- Standalone application (for independent audio processing)

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
DC BLOCKER - DC Offset Removal
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

## Installation

### Linux

**VST3 Plugin:**
1. Download the `NineStrip.vst3` bundle
2. Copy to your VST3 directory:
   ```bash
   mkdir -p ~/.vst3
   cp -r NineStrip.vst3 ~/.vst3/
   ```
3. Rescan plugins in your DAW

**Alternative system-wide installation:**
```bash
sudo cp -r NineStrip.vst3 /usr/lib/vst3/
```

**Standalone Application:**
1. Download the `NineStrip` executable
2. Make it executable and run:
   ```bash
   chmod +x NineStrip
   ./NineStrip
   ```

### macOS

**VST3 Plugin:**
1. Download the `NineStrip.vst3` bundle
2. **Remove macOS quarantine flag**  
   macOS Gatekeeper blocks unsigned plugins with a "plugin is damaged" error. Until this plugin is code-signed, you'll need to remove the quarantine flag using Terminal:
   
   ```bash
   xattr -rd com.apple.quarantine ~/Downloads/NineStrip.vst3
   ```

3. Copy to one of these locations:
   ```
   ~/Library/Audio/Plug-Ins/VST3/        (user)
   /Library/Audio/Plug-Ins/VST3/         (system)
   ```
4. Rescan plugins in your DAW

**Standalone Application:**
1. Download the `NineStrip.app` bundle
2. Remove quarantine flag:
   ```bash
   xattr -rd com.apple.quarantine ~/Downloads/NineStrip.app
   ```
3. Drag to Applications folder or run directly

### Windows

**VST3 Plugin:**
1. Download the `NineStrip.vst3` bundle
2. Copy to your VST3 directory:
   ```
   C:\Program Files\Common Files\VST3\
   ```
3. Rescan plugins in your DAW

**Standalone Application:**
1. Download `NineStrip.exe`
2. Run directly or place in a convenient location

## Building from Source

```bash
git clone --recurse-submodules https://github.com/blablack/nine-strip.git
cd nine-strip
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
ninja -C build
```

Built artifacts will be in:
- VST3: `build/src/NineStrip_artefacts/Release/VST3/`
- Standalone: `build/src/NineStrip_artefacts/Release/Standalone/`

### Compiling documentation

```bash
cd doc
pandoc NineStrip_Manual.md -o NineStrip_Manual.pdf --pdf-engine=xelatex
```

## Usage

**Plugin Mode:** Load NineStrip on any channel where you want complete processing control.

**Standalone Mode:** Launch the application and configure your audio interface in the settings. Perfect for processing external hardware or standalone audio tasks.

The signal flows from top to bottom through each stage, allowing you to shape tone, dynamics, and character in a single pass.

## Credits

This plugin incorporates processing algorithms from [Airwindows](https://www.airwindows.com/) by Chris Johnson. Massive thanks to Chris for his open-source contributions to the audio community. Support his work on [Patreon](https://www.patreon.com/airwindows).

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
