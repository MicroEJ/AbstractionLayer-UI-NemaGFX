![ARCH](https://shields.microej.com/endpoint?url=https://repository.microej.com/packages/badges/arch_7.16.json)

# Overview

This Abstraction Layer implements a part of the drawings of the MicroUI Low Level API, described in [LLUI_DISPLAY: Display](https://docs.microej.com/en/latest/VEEPortingGuide/appendix/llapi.html#llui-display-display), over the Think Silicon NemaGFX library.
It provides the drawing functions of `ui_drawing.h` that the GPU can perform, for the MicroUI and Drawing libraries.
When the GPU cannot perform a drawing, the software drawing of the Graphics Engine performs it.
See the [MicroUI Over NemaGFX](https://docs.microej.com/en/latest/VEEPortingGuide/uiCco.html#section-ui-c-module-microui-nemagfx) section of the MicroEJ documentation.

This Abstraction Layer is tied to the MicroEJ UI Pack: the versions it is compatible with are listed in the [compatibility](https://docs.microej.com/en/latest/VEEPortingGuide/uiReleaseNotes.html#section-ui-releasenotes-cmodule-extended) tables of the documentation.
The badge above gives the oldest MicroEJ Architecture it supports, not the latest one: the [MicroEJ Architecture Compatibility Version](https://docs.microej.com/en/latest/VEEPortingGuide/uiReleaseNotes.html#microej-architecture-compatibility-version) tables list the Architectures of each UI Pack version.

# Usage

1. Add the sources of this Abstraction Layer to the BSP of the VEE Port, in either of these two ways:

   - From the MicroEJ repository: install the `.cco` archive of the version the VEE Port needs, as the [installation guide](https://docs.microej.com/en/latest/VEEPortingGuide/appendix/cmodules.html) describes.
     The headers land in `bsp/vee/port/ui_nemagfx/inc` and the sources in `bsp/vee/port/ui_nemagfx/src`.
   - From GitHub: add the [GitHub repository](https://github.com/MicroEJ/AbstractionLayer-UI-NemaGFX) to the BSP of the VEE Port as a git submodule, at the tag of the version the VEE Port needs.
     The VEE Port chooses the folder of the submodule, and either of these two layouts works.
     A flat folder sits beside the other components of the BSP:

     ```sh
     git submodule add https://github.com/MicroEJ/AbstractionLayer-UI-NemaGFX.git bsp/vee/port/ui_nemagfx
     git -C bsp/vee/port/ui_nemagfx checkout <version>
     ```

     A folder grouped by domain sits under the folder of the other components of the same domain:

     ```sh
     git submodule add https://github.com/MicroEJ/AbstractionLayer-UI-NemaGFX.git bsp/vee/port/ui/UI-NemaGFX
     git -C bsp/vee/port/ui/UI-NemaGFX checkout <version>
     ```

     The headers then sit in `<folder>/src/main/c/inc` and the sources in `<folder>/src/main/c/src`, where `<folder>` is the folder of the submodule.
     A copy of these sources in the VEE Port repository works as well.

   Then add the folder of the headers to the include paths of the BSP project, and the files of the folder of the sources to its sources.

2. The configuration file [ui_nema_configuration.h](src/main/c/inc/ui_nema_configuration.h) stores the default values of the Abstraction Layer options.
   To change an option, define it in the file `veeport_configuration.h` of the VEE Port, which `ui_nema_configuration.h` includes first through `ui_configuration.h`.

3. When the VEE Port moves to another version of this Abstraction Layer, follow the [MicroUI migration guide](https://docs.microej.com/en/latest/VEEPortingGuide/uiMigrationGuide.html).

# Requirements

None.

# Validation

This Abstraction Layer is tested on the following boards:

- STM32U5A9J-DK, with the NemaGFX library [1.4.8-1.4.11].

# MISRA Compliance

This Abstraction Layer is MISRA-compliant (MISRA C:2012) with some noted exceptions.
It has been verified with Cppcheck v2.13.
Here is the list of deviations from the MISRA standard:

| Deviation  | Category  | Justification                                                                                          |
|:----------:|:---------:|:------------------------------------------------------------------------------------------------------ |
| Rule 2.5   | Advisory  | A macro can be defined at API level and not used by the application.                                   |
| Rule 5.5   | Required  | Voluntary use of macro naming.                                                                         |
| Rule 8.2   | Required  | Functions with no parameters are declared with `(void)`.                                               |
| Rule 8.4   | Required  | The compatible declarations are the UI Pack's functions.                                               |
| Rule 8.7   | Advisory  | The LLAPI functions require external linkage, but cppcheck cannot detect their invocation by the Pack. |
| Rule 8.9   | Advisory  | The code is clearer with the object defined at file scope.                                             |
| Rule 10.8  | Required  | Cast to store the value in a NemaGFX matrix.                                                           |
| Rule 11.4  | Advisory  | Cast of a buffer pointer to an integer, as required by the signature of the NemaGFX functions.         |
| Rule 17.3  | Mandatory | The implicitly declared functions are the UI Pack's functions.                                         |

# Dependencies

- The MicroUI Abstraction Layer.
- The NemaGFX library, with its NemaVG part.

# Changelog

The changes of this Abstraction Layer are listed in the [public changelog of the MicroEJ UI Pack](https://docs.microej.com/en/latest/VEEPortingGuide/uiChangeLog.html), section `C Module NemaGFX`.

# Source

- `ui_nema_configuration.h`: the options of the Abstraction Layer.
- `ui_drawing_nema.h`, `ui_drawing_nema.c`: the drawing functions of `ui_drawing.h` over NemaGFX.
- `ui_drawing_nema_process.h`: the NemaGFX drawings that another Abstraction Layer reuses for its own destination.
- `ui_nema.h`, `ui_nema.c`: the helper functions to target the NemaGFX library.

# Restrictions

## Apollo4 Plus board

### Drawing image

With the Apollo4 Plus board, the UI testsuite does not fully work for rotating images.
The macro `ENABLE_IMAGE_ROTATION` bypasses the problem.

### Resources location

To avoid a GPU hang on the Apollo4 Plus board, the resources must be placed out of the MRAM and SRAM memory range.

---
_Copyright 2023-2026 MicroEJ Corp._\
_Use of this source code is governed by a BSD-style license that can be found with this software._\
_Build: 7E4D1F7C_
