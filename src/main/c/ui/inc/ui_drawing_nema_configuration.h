/*
 * C
 *
 * Copyright 2023-2024 MicroEJ Corp. All rights reserved.
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

#if !defined UI_DRAWING_NEMA_CONF_H
#define UI_DRAWING_NEMA_CONF_H

#if defined __cplusplus
extern "C" {
#endif

/**
 * @brief Compatibility sanity check value.
 * This define value is checked in the implementation to validate that the version of this configuration
 * is compatible with the implementation.
 *
 * This value must not be changed by the user of the CCO.
 * This value must be incremented by the implementor of the CCO when a configuration define is added, deleted or modified.
 */
#define NEMA_CONFIGURATION_VERSION (2)

#error "This header must be customized with VEE Port-specific configuration. Remove this #error when done. This file is not modified when a new version of the CCO is installed."

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
 * @brief Specifies the display frame buffer format. This format may be the different
 * than the BACK_BUFFER_FORMAT. This is useful to use a compressed format (TSCx) that
 * requires less memory than standard raster format.
 *
 * Notes:
 *
 * - The implementation of LLUI_DISPLAY_impl.h must be in COPY mode: the implementation
 * of flush() consists to copy the content of the BACK buffer to the FRAME buffer (the
 * implementation should use the functions UI_DRAWING_NEMA_configure_memcpy() and
 * UI_DRAWING_NEMA_start_memcpy()).
 * - In SWITCH mode, the both buffers (BACK and FRAME) must have the same format.
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
 * @brief Specifies the "end of GPU operation" mode.
 *
 * When set (== interrupt mode), the nema interrupt routine MUST call the function UI_DRAWING_NEMA_post_operation() to notify
 * the end of the last GPU operation.
 *
 * When unset (== task mode), the VEE Port has to use a task that MUST call the function UI_DRAWING_NEMA_post_operation() to
 * wait the end of the last GPU operation. It also has to implement the low level API UI_DRAWING_NEMA_IMPL_operation_submitted()
 * to unlock this task:
 *
 * void UI_DRAWING_NEMA_IMPL_operation_submitted(void) {
 * 	tx_semaphore_put(&_sem_gpu);
 * }
 *
 * static void _gpu_thread_entry(ULONG thread_input) {
 * 	while(true) {
 * 		LLUI_DISPLAY_IMPL_binarySemaphoreTake((void*)&_sem_gpu);
 * 		UI_DRAWING_NEMA_post_operation();
 * 	}
 * }
 *
 * By default the interrupt mode is selected.
 */
#define NEMA_INTERRUPT_MODE

/*
 * @brief To draw a shape, the GPU uses the command list. For rectangular shapes
 * (draw/fill rectangles and images), the max list size is fixed (around 300 bytes).
 *
 * For the other shapes (circle, etc.) the list increases according to the shape's size
 * (dynamic shape): several blocks of 1024 bytes and 40 bytes are allocated and never
 * freed (set breakpoints on assembler functions "tsi_malloc_pool" and "tsi_free").
 *
 * Note: "nema_draw_circle" internally calls "nema_draw_rounded_rect" that consumes the
 * 1024 & 40 bytes blocks.
 *
 * By default the dynamic shapes are disabled and the software algorithms are used
 * instead.
 *
 * To enable these drawings, the command list size must be calibrated.
 */
//#define ENABLE_DYNAMIC_SHAPES

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

#endif // !defined UI_DRAWING_NEMA_CONF_H
