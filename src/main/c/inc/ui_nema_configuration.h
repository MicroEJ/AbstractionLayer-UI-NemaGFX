/*
 * C
 *
 * Copyright 2023-2026 MicroEJ Corp.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 * Build: 7E4D1F7C
 */

/**
 * @file
 * @brief MicroEJ MicroUI library low level API: implementation over NemaGFX. Provides a set of defines to configure the
 * implementation.
 *
 * Refer to the VEE Porting Guide > Graphics User Interface > C Module documentation to have more information about the
 * feature of this Abstraction Layer, how to use it and how to configure it.
 *
 * @author MicroEJ Developer Team
 */

#if !defined UI_NEMA_CONFIGURATION_H
#define UI_NEMA_CONFIGURATION_H

#if defined __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "ui_configuration.h"

// -----------------------------------------------------------------------------
// Sanity checks
// -----------------------------------------------------------------------------

#if !defined DISPLAY_WIDTH || !defined DISPLAY_HEIGHT
#error "Please define the display resolution."
#endif

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

/**
 * @brief Specifies the back buffer format (the buffer used by MicroUI, see LLUI_DISPLAY_IMPL_initialize()). This format
 * must be the same than the one set in VEE Port configuration project. See display.properties, property "bpp".
 *
 * The BPP formats are not exactly the same between MicroEJ and NemaGFX. The available options are:
 *  _________________________________
 * | MicroEJ Format | NemaGFX Format |
 * |________________|________________|
 * |     RGB565     |  NEMA_RGB565   |
 * |    ARGB8888    | NEMA_BGRA8888  |
 * |________________|________________|
 *
 * Notes:
 *
 * - This format is also used as destination format of the MicroUI BufferedImages.
 * - The pool of "tsi_malloc" must be large enough to store the buffer DISPLAY_WIDTH x DISPLAY_HEIGHT x BPP / 8.
 * - The linker file probably needs adjusting.
 */
#ifndef BACK_BUFFER_FORMAT
#define BACK_BUFFER_FORMAT (NEMA_RGB565) // NEMA_RGB565 | NEMA_BGRA8888
#endif

/**
 * @brief Specifies the front buffer format. This format may be the different from the BACK_BUFFER_FORMAT. This is
 * useful to use a compressed format (TSCx) that requires less memory than standard raster format.
 *
 * Notes:
 *
 * - The implementation of LLUI_DISPLAY_impl.h must be in COPY mode: the implementation of flush() consists to copy the
 * content of the BACK buffer to the FRONT buffer (the implementation should use the functions
 * UI_NEMA_configure_memcpy() and UI_NEMA_start_memcpy()).
 * - In SWAP mode, both buffers (BACK and FRONT) must have the same format.
 *
 * The buffer size is:
 *
 * - TSC4: DISPLAY_WIDTH/4 * DISPLAY_HEIGHT/4 * 64/8 (64 bits per 4x4 block of pixels)
 * - TSC6: DISPLAY_WIDTH/4 * DISPLAY_HEIGHT/4 * 96/8 (96 bits per 4x4 block of pixels)
 * - TSC6A: DISPLAY_WIDTH/4 * DISPLAY_HEIGHT/4 * 96/8 (96 bits per 4x4 block of pixels)
 */
#ifndef FRAME_BUFFER_FORMAT
#define FRAME_BUFFER_FORMAT (BACK_BUFFER_FORMAT) // BACK_BUFFER_FORMAT | NEMA_TSC4 | NEMA_TSC6 | NEMA_TSC6A
#endif

// -----------------------------------------------------------------------------
// Options
// -----------------------------------------------------------------------------

/**
 * @brief Set this define to 1 to use the GPU to draw horizontal lines, vertical lines and rectangles.
 *
 * By default this option is disabled and the software algorithms are used instead (because the software algorithms are
 * often faster than the GPU to draw these simple shapes).
 */
#ifndef ENABLE_SIMPLE_LINES
#define ENABLE_SIMPLE_LINES (0)
#endif

/**
 * @brief Set this define to 1 to use the GPU to draw thick faded lines.
 *
 * The nema_draw_line_aa() API does not respect the thick faded line conditions exactly: the line can be one pixel
 * smaller across the width, one pixel shifted upwards, etc. The differences appear depending on the thickness and fade
 * of the line.
 *
 * By default this option is disabled and the software algorithm is used instead.
 */
#ifndef ENABLE_FADED_LINES
#define ENABLE_FADED_LINES (0)
#endif

/**
 * @brief Set this define to 1 to use the GPU to draw images with rotation.
 *
 * Some GPU might not able to render the images in specific memories.
 *
 * By default this option is disabled and the software algorithms are used instead.
 */
#ifndef ENABLE_IMAGE_ROTATION
#define ENABLE_IMAGE_ROTATION (1)
#endif

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // !defined UI_NEMA_CONFIGURATION_H
