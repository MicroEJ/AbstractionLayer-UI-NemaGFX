# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [5.0.2] - 2026-10-06

### Changed

- Compatible with MicroEJ UI Pack 14.5.5.
- Complete the Doxygen documentation of the headers, and align the README with the other MicroEJ Abstraction Layers.

## [5.0.1] - 2026-08-04

### Fixed

- Fix incorrect Doxygen comments in the NemaGFX drawing headers.

## [5.0.0] - 2025-10-07

- Compatible with MicroEJ UI Pack 14.5.0.

### Changed

- Use the UI Pack 14.5.0 options convention.

### Fixed

- Fix the copyright headers.
- Fix the useless include directive.

## [4.0.1] - 2025-05-20

### Fixed

- Fix the rendering of a RGB565 image with a transparent opacity.
- Fix the implementation of `UI_DRAWING_NEMA_copyImage()` to use the right software algorithm.

## [4.0.0] - 2024-10-09

- Compatible with MicroEJ UI Pack 14.1.0.

### Changed

- Fetch the MicroUI Abstraction Layer 14.1.0.

## [3.0.1] - 2024-10-01

- Compatible with MicroEJ UI Pack 14.0.3.

### Changed

- Fetch the MicroUI Abstraction Layer 14.0.3.

## [3.0.0] - 2024-07-19

- Compatible with MicroEJ UI Pack 14.0.x.

### Added

- Add some functions to manipulate the clip:
    - `UI_NEMA_get_clip`,
    - `UI_NEMA_set_clip`,
    - `UI_NEMA_set_clip_target`,
    - `UI_NEMA_set_clip_region`,
    - `UI_NEMA_set_clip_area`.
- Add some functions to manipulate the GPU command list:
    - `UI_NEMA_cmdlist_reset`,
    - `UI_NEMA_cmdlist_submit`.
- Add `UI_NEMA_convert_color`.

### Changed

- Install the files in the `ui_nemagfx` directory.
- Rename `ui_drawing_nema_configuration.h` to `ui_nema_configuration.h`.
- Use a circular GPU command queue.
- Increase the version of the configuration file (3).
- Move some functions from `ui_drawing_nema.h` to `ui_nema.h`.
    - `UI_DRAWING_NEMA_initialize` becomes `UI_NEMA_initialize`
    - `UI_DRAWING_NEMA_post_operation` becomes `UI_NEMA_post_operation`
    - `UI_DRAWING_NEMA_configure_memcpy` becomes `UI_NEMA_configure_memcpy`
    - `UI_DRAWING_NEMA_start_memcpy` becomes `UI_NEMA_start_memcpy`

### Fixed

- Fix the function declarations according to the define `ENABLE_SIMPLE_LINES`.
- Fix copying an image region from and to the same buffer.

### Removed

- Remove the configuration `ENABLE_DYNAMIC_SHAPES`: useless with a circular queue.
- Remove the GPU "task" mode and the configuration `NEMA_INTERRUPT_MODE`: only GPU "interrupt" mode is available.
    - `UI_DRAWING_NEMA_IMPL_operation_submitted` is removed.

## [2.0.1] - 2024-04-09

- Compatible with MicroEJ UI Pack 14.0.x.

### Changed

- Homogenize the notions of back and front buffers.

## [2.0.0] - 2024-02-14

- Compatible with MicroEJ UI Pack 14.0.x.

### Changed

- Make the Abstraction Layer compatible with the UI Pack 14.0.0 (MicroUI Abstraction Layer 4.0.0).

### Fixed

- Fix drawVerticalLine issue on y1 argument of nema_draw_line().

## [1.2.0] - 2023-12-21

- Compatible with MicroEJ UI Pack 13.7.x.

### Changed

- Disable the rendering of thick faded line with the GPU by default (see option `ENABLE_FADED_LINES`).
- Increase the version of the configuration file (2).

## [1.1.1] - 2023-12-20

- Compatible with MicroEJ UI Pack 13.7.x.

### Fixed

- Fix the drawing status when a thick line is out-of-clip (results in an infinite loop).

## [1.1.0] - 2023-10-23

- Compatible with MicroEJ UI Pack 13.7.x.

### Changed

- Fetch Abstraction Layer com.microej.clibrary.llimpl#microui#3.1.0 (compatibility with UI Pack 13.7.0).

### Fixed

- Fix some comments.

## [1.0.0] - 2023-07-17

- Compatible with MicroEJ UI Pack 13.5.x and 13.6.x.

### Added

 - Creation of the Abstraction Layer.

---
_Copyright 2023-2026 MicroEJ Corp._\
_Use of this source code is governed by a BSD-style license that can be found with this software._\
_Build: 7E4D1F7C_
