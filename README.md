
# Overview

MicroEJ C Component: `com.microej.clibrary.llimpl#microui-nemagfx`.

This component implements some Low-Level API (LLAPI) of MicroUI-3.x over the Think Silicon NemaGFX. 
Refer to the VEE Porting Guide > Graphics User Interface > C Module to have more information on this module.

## UI Pack Compatibility

This compoment is MicroEJ UI Pack dependent: it is compatible with a one or several MicroEJ UI Pack versions. The following table illustrates the compatibility:

| Component     | MicroEJ UI Pack   |
| ------------- | ----------------- |
| 2.0.0         | [14.0.0 - 15.0.0[ | 
| [1.1.0-1.2.0] | [13.7.0 - 14.0.0[ |
| 1.0.0         | [13.5.0 - 13.7.0[ |

> note: this component is useless (and not compatible) for the MicroEJ UI Packs before version 13.5.0.

# Usage

1. Add the following line to your `module.ivy`:

    <dependency org="com.microej.clibrary.llimpl" name="microui-nemagfx" rev="4.0.0"/>

2. Follow the **Usage** chapters of each component part (see VEE Porting Guide > Graphics User Interface > C Module)

# Migration Guide

The migration guides are described in the VEE Porting Guide > Graphical User Interface > Migration Guide. 

# Requirements

N/A

# Dependencies

_All dependencies are retrieved transitively by Ivy resolver._

# Source

N/A

# Restrictions

N/A

# Known Errors

## Apollo4 Plus Board

### Drawing Image

With the Apollo4 Plus board, the UI testsuite does not fully work for rotating images. The macro `ENABLE_IMAGE_ROTATION` has been made to bypass the problem.

### Resources Location

To avoid a GPU hang on the Apollo4 plus board, you will need to put the resources out of the MRAM and SRAM memory range.

---
_Copyright 2023-2024 MicroEJ Corp. All rights reserved._  
_Use of this source code is governed by a BSD-style license that can be found with this software._  
