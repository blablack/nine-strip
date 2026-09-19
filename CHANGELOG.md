# Changelog

## [v0.1.6] - XXXX-XX-XX

### Changed
- The Save and Delete preset dialogs open centred over the plugin window instead of the middle of the screen.

### Fixed
- Automating any parameter could, in rare cases, crash the host: the preset "modified" tracking read shared data on the audio thread while the interface was changing it. That tracking now happens only on the message thread.
- The Delete button now asks for confirmation before removing a preset, as the manual has always described; it used to delete immediately, one click away from Save.
- The manual gave the wrong macOS preset folder; it is `~/Library/Audio/Presets/NineStrip/Presets/`.
- Closing the plugin window while the Save Preset dialog was open and then pressing OK in the dialog could crash the host. The dialog now checks that the plugin window still exists before saving.
- Knob dragging is back to normal distance-based movement by default, with the mouse pointer staying visible. Hold Ctrl (Cmd on macOS) while dragging for fine control at one tenth of the speed; the key can be pressed or released mid-drag without the knob jumping. The velocity-based mode introduced in v0.1.4 made slow, fine movement the default, required the modifier for normal movement, and hid the pointer while dragging.
- The input and output faders get the same fine control: Ctrl-drag (Cmd on macOS) moves them at one tenth of the speed with the pointer visible, and a Ctrl-click nudges from the current position instead of jumping to the mouse. Previously the modifier put the faders into a speed-based mode that hid the mouse pointer.
- The VU and gain-reduction meter faces no longer look jagged and broken at small window sizes: the scale ticks, digits and arc were being decimated by the downscale and would shimmer while resizing. They are now downscaled with a proper filter so the face stays clean at the default size.
- The meter needles now read correctly against the printed scale at every window size. The needle pivot was fixed in screen pixels rather than scaling with the artwork, so at larger sizes the needle sat on the wrong part of the arc. The needle also scales its thickness with the window instead of staying a 2 px hairline.

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
