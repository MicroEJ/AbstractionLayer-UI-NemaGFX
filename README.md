
# Overview

MicroEJ C Component: microui-nemagfx.

# Usage

1. Add the following line to your `module.ivy`:

    <dependency org="com.microej.clibrary.llimpl" name="microui-nemagfx" rev="1.0.0"/>
2. In the bsp project, add all the C files from these folder: `src\main\c\ui\src`.
3. Add this folder in the include path: `src\main\c\ui\inc`.
4. Follow the installation steps described in `ui_drawing_nema.h`. 
5. Adjust configuration: see `ui_drawing_nema_configuration.h` (and comment the directive `#error`).  

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
_Copyright 2023 MicroEJ Corp. All rights reserved.
_Use of this source code is governed by a BSD-style license that can be found with this software.