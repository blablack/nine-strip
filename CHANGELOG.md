# Changelog

## [v0.1.4] - XXXX-XX-XX

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
