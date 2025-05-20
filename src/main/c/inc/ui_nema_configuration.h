/*
 * C
 *
 * Copyright 2023-2025 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * @file
 * @brief MicroEJ MicroUI library low level API: implementation over NemaGFX. Provides
 * a set of defines to configure the implementation.
 *
 * Refer to the VEE Porting Guide > Graphics User Interface > C Module documentation to have more
 * information about the feature of this C module, how to use it and how to configure it.
 *
 * @author MicroEJ Developer Team
 */

#if !defined UI_NEMA_CONFIGURATION_H
#define UI_NEMA_CONFIGURATION_H

#if defined __cplusplus
extern "C" {
#endif

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or
 * modified.
 */
#define NEMA_CONFIGURATION_VERSION (3)

#error \
	"This header must be customized with VEE Port-specific configuration. Remove this #error when done. This file is not modified when a new version of the CCO is installed."

// -----------------------------------------------------------------------------
// Configuration
// -----------------------------------------------------------------------------

/*
 * @brief Specifies the display size
 */
#define DISPLAY_WIDTH (480)
#define DISPLAY_HEIGHT (480)

/*
 * @brief Specifies the back buffer format (the buffer used by MicroUI, see
 * LLUI_DISPLAY_IMPL_initialize()). This format must be the same than the one set
 * in VEE Port configuration project. See display.properties, property "bpp".
 *
 * The BPP formats are not exactly the same between MicroEJ and NemaGFX. The available
 * options are:
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
 * - The pool of "tsi_malloc" must be large enough to store the buffer
 *   DISPLAY_WIDTH x DISPLAY_HEIGHT x BPP / 8.
 * - The linker file probably needs adjusting.
 */
#define BACK_BUFFER_FORMAT (NEMA_RGB565) // NEMA_RGB565 | NEMA_BGRA8888

/*
 * @brief Specifies the front buffer format. This format may be the different
 * from the BACK_BUFFER_FORMAT. This is useful to use a compressed format (TSCx) that
 * requires less memory than standard raster format.
 *
 * Notes:
 *
 * - The implementation of LLUI_DISPLAY_impl.h must be in COPY mode: the implementation
 * of flush() consists to copy the content of the BACK buffer to the FRONT buffer (the
 * implementation should use the functions UI_NEMA_configure_memcpy() and
 * UI_NEMA_start_memcpy()).
 * - In SWAP mode, both buffers (BACK and FRONT) must have the same format.
 *
 * The buffer size is:
 *
 * - TSC4: DISPLAY_WIDTH/4 * DISPLAY_HEIGHT/4 * 64/8 (64 bits per 4x4 block of pixels)
 * - TSC6: DISPLAY_WIDTH/4 * DISPLAY_HEIGHT/4 * 96/8 (96 bits per 4x4 block of pixels)
 * - TSC6A: DISPLAY_WIDTH/4 * DISPLAY_HEIGHT/4 * 96/8 (96 bits per 4x4 block of pixels)
 */
#define FRAME_BUFFER_FORMAT (BACK_BUFFER_FORMAT) // BACK_BUFFER_FORMAT | NEMA_TSC4 | NEMA_TSC6 | NEMA_TSC6A

// -----------------------------------------------------------------------------
// Options
// -----------------------------------------------------------------------------

/*
 * @brief The software algorithms are faster than GPU to draw horizontal and vertical
 * lines; and by consequence to draw rectangles.
 *
 * By default these shapes are disabled and the software algorithms are used
 * instead.
 */
//#define ENABLE_SIMPLE_LINES

/*
 * @brief The nema_draw_line_aa() API does not respect the thick faded line conditions
 * exactly: the line can be one pixel smaller across the width, one pixel shifted upwards,
 * etc. The differences appear depending on the thickness and fade of the line.
 *
 * By default, this hardware acceleration is disabled.
 */
//#define ENABLE_FADED_LINES

/*
 * @brief Some GPU might not able to render the images in specific memories. Comment
 * this define to not use the GPU to render the rotated images.
 */
#define ENABLE_IMAGE_ROTATION

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // !defined UI_NEMA_CONFIGURATION_H
