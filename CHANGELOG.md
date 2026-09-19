# Changelog

## [v0.1.6] - XXXX-XX-XX

### Added
- The host's own bypass switch (VST3, AU, LV2) now drives Master Bypass, with the same click-free crossfade. CLAP hosts keep using their own bypass.
- The needle meters sit behind a pane of dirty glass.

### Changed
- Knob dragging is back to normal distance-based movement, with the pointer staying visible. Hold Ctrl (Cmd on macOS) while dragging for fine control.
- Saving a preset under an existing name asks before replacing it, and Delete asks for confirmation.
- The meters' Input/Output selector is no longer exposed to the host as an automatable parameter; it is still saved with the session and in presets.
- Enlarging the window now scales the whole interface, not just the knobs and meters.

### Fixed
- The window could be resized taller than the screen in hosts that don't limit it themselves (Bitwig, for one), leaving the resize corner unreachable. The size is now capped to the screen.
- Automating a parameter, or a host saving the session from a background thread, could in rare cases crash the host.
- Pressing OK in the Save Preset dialog after closing the plugin window could crash the host.
- The Pre/Post switch and the meter Input/Output buttons now register as automation gestures, so touch and latch automation modes record them.
- Preset names containing characters not allowed in file names (such as `/`) failed to save; those characters are now dropped.
- Restoring a session with no preset selected (e.g. via the host's undo) no longer leaves the previous preset name in the preset bar.
- A `.ninestrip` file in the preset folder that is not a NineStrip preset is now ignored instead of replacing the plugin state.
- The manual gave the wrong macOS preset folder; it is `~/Library/Audio/Presets/NineStrip/Presets/`.

## [v0.1.5] - 2026-09-13

### Added
- CLAP plugin format (Windows, macOS, Linux), built with clap-juce-extensions and validated with clap-validator in CI

### Fixed
- Driving a hot signal (e.g. maximum input gain) into the filter section with the hi-pass or low-pass near their limits, or into the saturation section, could make that stage burst into loud garbage and then go permanently silent (NaN/inf) until the plugin was reloaded. Such a stage is now reset immediately instead of blasting the burst through the output gain.
- CLAP hosts are told to re-read every parameter after a session or preset is restored, so their automation lanes and generic UIs match the restored state

## [v0.1.4] - 2026-09-13

### Added
- Juce 9.0.2
- Velocity-based knob drag sensitivity, with a modifier key (Ctrl/Cmd/Alt) to switch to normal distance-based dragging
- Click-free bypass switching: every section bypass, the master bypass and the saturation Pre/Post toggle now crossfade over 10 ms instead of hard-switching, and bypassed stages keep running so re-enabling them produces no transient

### Changed
- VU and gain-reduction meters only repaint when the needle or peak LED actually moves, and paint opaquely so the editor background is no longer redrawn behind them on every frame. An idle plugin now costs no UI repaints at all.

### Fixed
- Reopening a saved session no longer re-loads the preset file from disk, which was discarding any parameter tweaks made after loading a preset. The host's saved state is now authoritative; the preset name and its modified marker are restored for display only.

## [v0.1.3] - 2026-08-22

### Added
- Juce 9.0.1

## [v0.1.2] - 2026-05-25

### Added
- Juce 8.0.13

### Fixed
- Replaces the raw this capture with a juce::Component::SafePointer<NineStripProcessorEditor>. When the editor is destroyed, the SafePointer automatically becomes null, so the async callback does nothing instead of crashing.

## [v0.1.1] - 2026-04-16

### Fixed
- Linux: improved compatibility with older distros (Ubuntu 22.04+, Debian 12+)

## [v0.1.0] - 2026-03-19
### Added
- Replaced Highpass2/Lowpass2 with Capacitor2
- Channel9 (saturation) can now be pre/post

### Fixed
- Knob grabbing fix
- DSP optimizations
- UI resizing optimizations

## [v0.0.4] - 2026-03-10
### Added
- AU format support

### Fixed
- Ballistic needle movement

## [v0.0.3] - 2026-02-18
### Added
- Airwindows Interstage DSP algorithm
- LV2 format support

## [v0.0.2] - 2026-02-09
### Fixed
- macOS validation issue

## [v0.0.1] - 2026-02-08
### Added
- Initial release
