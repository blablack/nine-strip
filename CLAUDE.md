# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Project Is

Nine-Strip is a JUCE-based audio plugin (VST3/AU/LV2, optional AAX) and standalone application implementing a channel strip. It chains 9 DSP stages, 7 of which are Airwindows algorithms, into a fixed-order stereo-only signal path (input gain → saturation → filters → EQ → compressor → output gain).

Note: this `CLAUDE.md` is listed in `.gitignore` — it is local, not checked in.

## Build Commands

```bash
# Configure (Release; use -DCMAKE_BUILD_TYPE=Debug for a debug build)
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
ninja -C build

# Run the standalone build
./build/src/NineStrip_artefacts/Release/Standalone/NineStrip
```

Artefacts land in `build/src/NineStrip_artefacts/<Config>/{VST3,AU,LV2,Standalone}/`. AAX is built only if `AAX_SDK_PATH` is set (env var or `-DAAX_SDK_PATH=`).

`compile_commands.json` at the repo root is a symlink into `build/`, so the build directory must be named `build` for clangd/clang-tidy to work.

### Lint / format

```bash
clang-format -i src/PluginProcessor.cpp            # Google style, 4-space indent, 128 cols, Allman braces
clang-tidy -p build src/PluginProcessor.cpp        # uses .clang-tidy; needs a configured build
```

### Validation

There are no unit tests. CI validates with `pluginval` at strictness level 5; run it locally with:

```bash
pluginval --strictness-level 5 --validate-in-process --vst3 build/src/NineStrip_artefacts/Release/VST3/NineStrip.vst3
```

### Manual

```bash
cd doc && pandoc NineStrip_Manual.md -o NineStrip_Manual.pdf --pdf-engine=xelatex
```

## Release Process

- Version lives in the top-level `CMakeLists.txt` (`project(... VERSION x.y.z)`).
- Pushing a `v*` tag triggers `.github/workflows/build-release.yml`: builds Linux/macOS/Windows, runs pluginval (and `auval` on macOS), packages, and creates a GitHub release.
- Release notes are extracted from `CHANGELOG.md` by matching a `## [vX.Y.Z]` heading against the tag name exactly — a release commit must add that heading.

## Architecture

### Build targets

`src/CMakeLists.txt` defines the `NineStrip` plugin target (`juce_add_plugin`) and links two static libraries plus generated binary data:

- `AirwindowsDSP` (`src/airwindows/`) — plain C++ with no JUCE dependency. Each algorithm is split into `X.cpp` (ctor/params) and `XProc.cpp` (`processReplacing` for float, `processDoubleReplacing` for double). All parameters are normalised 0–1 floats addressed by `kParamA..` enums.
- `NineStripUI` (`src/ui/`) — custom JUCE widgets (`CircularKnob`, `NeedleVUMeter`, `GlowButton`, `VUMeterBallistics`, `KnobLookAndFeel`, `FaderLookAndFeel`).
- `NineStripAssets` — PNGs from `assets/` embedded via `juce_add_binary_data` (the SVGs alongside are the sources).

**Do not change the DSP in `src/airwindows/`.** The algorithms stay as ported; fixes and workarounds go in project code (e.g. `resetCapacitor2State()` in `PluginProcessor.cpp` uses placement-new to reset filter state rather than adding a reset method). The one existing exception is a read-only metering tap — `Pressure4::getGainReductionLinear()` backed by an atomic written in the process loop — which observes the signal without altering it.

### Signal chain (`NineStripProcessor::processBlockInternal`)

Fixed order, stereo only. `Channel9` runs either pre (after Interstage) or post (before output gain) depending on the `saturationInput` bool — never both.

1. `PurestGain` — input gain
2. `Interstage` — analog conditioning (always on)
3. `Channel9` — console model + drive (if `saturationInput`)
4. `Capacitor2` — hi-pass / low-pass (`filterBypass`) — has a NaN guard that resets filter state
5. `Baxandall2` — bass/treble shelves (`eqBypass`)
6. `Parametric` — high-mid band only (`eqBypass`); the treble/low-mid bands are commented out throughout
7. `DCBlocker` — project-local, always on
8. `Pressure4` — compressor (`compressorBypass`)
9. `Channel9` — (if `!saturationInput`)
10. `PurestGain` — output gain

`processBlock` is templated on `SampleType`; `if constexpr (std::is_same_v<SampleType, float>)` picks `processReplacing` vs `processDoubleReplacing`. `masterBypass` short-circuits everything.

### Parameter flow

- APVTS is the single source of truth. Continuous parameter IDs (all 0–1): `inputGain`, `consoleType`, `drive`, `lowpass`, `hipass`, `non_lin`, `treble`, `bass`, `hm_freq`, `highmid`, `hm_reso`, `pressure`, `speed`, `mewiness`, `outputGain`.
- Bool IDs: `masterBypass`, `saturationBypass`, `filterBypass`, `eqBypass`, `compressorBypass`, `saturationInput` (pre/post Channel9), `inputMeasured` (VU meters show input vs output).
- Continuous params are pushed to the Airwindows objects through `NineStripProcessor::parameterChanged` (APVTS listener, registered for the IDs in `parameterIDs`). Adding a parameter means: add it to `createParameterLayout()`, to the `parameterIDs` list, to `parameterChanged`, and to the initial sync in `prepareToPlay`.
- Bool params are read directly on the audio thread via cached `std::atomic<float>*` pointers set in `prepareToPlay`.
- Any APVTS tree change also fires `valueTreePropertyChanged`, which marks the current preset modified and refreshes the editor's preset display.

### Threading

Audio thread never touches UI. Meters are `std::atomic<float>` on the processor (dB values; GR from `Pressure4`), polled by `NeedleVUMeter` timers. Metering is skipped entirely when the editor is closed (`editorOpen` atomic set by the editor ctor/dtor) or when rendering offline. Editor callbacks from parameter listeners go through `MessageManager::callAsync` with a `Component::SafePointer`.

### Editor

Base size 600×600, resizable up to 3× with a fixed aspect ratio; the chosen size persists in `ApplicationProperties` (`editorWidth`/`editorHeight`). The background is pre-scaled once in `resized()` into `scaledBackground`.

### Presets

`PresetManager` writes APVTS state as XML `.ninestrip` files under the user app-data dir (`~/.config/NineStrip/Presets` on Linux, `~/Library/Audio/Presets/NineStrip/Presets` on macOS).

### JUCE

JUCE is a git submodule in `lib/JUCE/` and is added with `add_subdirectory` (never `find_package`) to avoid picking up a system install. If missing after clone: `git submodule update --init`.
