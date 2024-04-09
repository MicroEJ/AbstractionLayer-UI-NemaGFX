# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.0.1] - 2024-04-09

- Compatible with MicroEJ UI Pack 14.0.x.

### Changed

- Homogenize the notions of back and front buffers.

## [2.0.0] - 2024-02-14

- Compatible with MicroEJ UI Pack 14.0.x.

### Changed

- Make the CCO compatible with the UI Pack 14.0.0 (CCO MicroUI 4.0.0).

### Fixed

- Fix drawVerticalLine issue on y1 argument of nema_draw_line().

## 1.2.0 - 2023-12-21

- Compatible with MicroEJ UI Pack 13.7.x.

### Changed

- Disable the rendering of thick faded line with the GPU by default (see option `ENABLE_FADED_LINES`).
- Increase the version of the configuration file (2).

## 1.1.1 - 2023-12-20

- Compatible with MicroEJ UI Pack 13.7.x.

### Fixed

- Fix the drawing status when a thick line is out-of-clip (results in an infinite loop).

## 1.1.0 - 2023-10-23

- Compatible with MicroEJ UI Pack 13.7.x.

### Changed

- Fetch CCO com.microej.clibrary.llimpl#microui#3.1.0 (compatibility with UI Pack 13.7.0).

### Fixed

- Fix some comments.

## 1.0.0 - 2023-07-17

- Compatible with MicroEJ UI Pack 13.5.x and 13.6.x.

### Added

 - Creation of the CCO.
 
---
_Copyright 2023-2024 MicroEJ Corp. All rights reserved._  
_Use of this source code is governed by a BSD-style license that can be found with this software._  
