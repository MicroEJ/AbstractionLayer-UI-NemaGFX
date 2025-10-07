# Overview

MicroEJ C Component: `com.microej.clibrary.llimpl#microui-nemagfx`.

This component implements some Low-Level API (LLAPI) of MicroUI-3.x over the Think Silicon NemaGFX. 
Refer to the VEE Porting Guide > Graphics User Interface > C Module to have more information on this module.

# UI Pack Compatibility

This compoment is MicroEJ UI Pack dependent: it is compatible with one or several MicroEJ UI Pack versions, listed in the [C Module compatibility](https://docs.microej.com/en/latest/VEEPortingGuide/uiReleaseNotes.html#extended-c-modules) tables in the documentation.

> Note: this component is useless (and not compatible) for the MicroEJ UI Packs before version 13.5.1.

# Usage

1. Add the following line to your `module.ivy`:

    <dependency org="com.microej.clibrary.llimpl" name="microui-nemagfx" rev="5.0.0"/>

2. Follow the **Usage** chapters of each component part (see VEE Porting Guide > Graphics User Interface > C Module)

# Migration Guide

The migration guides are described in the VEE Porting Guide > Graphical User Interface > Migration Guide. 

# Requirements

N/A.

# Dependencies

_All dependencies are retrieved transitively by MicroEJ Module Manager_.

# Source

N/A.

# Restrictions

N/A.

# Known Errors

## Apollo4 Plus Board

### Drawing Image

With the Apollo4 Plus board, the UI testsuite does not fully work for rotating images. The macro `ENABLE_IMAGE_ROTATION` has been made to bypass the problem.

### Resources Location

To avoid a GPU hang on the Apollo4 plus board, you will need to put the resources out of the MRAM and SRAM memory range.

---
_Copyright 2023-2025 MicroEJ Corp._\
_Use of this source code is governed by a BSD-style license that can be found with this software._
