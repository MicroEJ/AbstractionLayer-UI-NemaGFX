/*
 * C
 *
 * Copyright 2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#ifndef UI_NEMA_H
#define UI_NEMA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @file
 * @brief MicroEJ MicroUI library low level API: implementation over NEMA. Provides
 * a set of helper functions to target the NEMA library.
 *
 * @author MicroEJ Developer Team
 * @version 4.0.0
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <LLUI_DISPLAY.h>

// -----------------------------------------------------------------------------
// Defines
// -----------------------------------------------------------------------------

/*
 * @brief Value to enable the MSAA for the four edges (see UI_NEMA_cmdlist_reset())
 */
#define MSAA_ENABLED (1)

/*
 * @brief Value to disable the MSAA for the four edges (see UI_NEMA_cmdlist_reset())
 */
#define MSAA_DISABLED (0)

// -----------------------------------------------------------------------------
// Typedefs
// -----------------------------------------------------------------------------

/*
 * @brief The NEMA library does not provide a way to retrieve the clip set using
 * nema_set_clip(). This structure holds the current clip.
 *
 * @see UI_NEMA_get_clip()
 * @see UI_NEMA_set_clip()
 */
typedef struct clip_rect {
	int32_t x;
	int32_t y;
	int32_t w;
	int32_t h;
} clip_rect;

// -----------------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------------

/*
 * @brief Entry point to initialize the NEMA library with a GPU command list.
 */
void UI_NEMA_initialize(void);

/*
 * @brief Configures the GPU commands list with the new destination (RGB565 format
 * for sure).
 *
 * @param[in] gc the destination
 * @param[in] msaa MSAA_ENABLED | MSAA_DISABLED to configure the MSAA for the four
 * edges (see nema_enable_aa())
 */
void UI_NEMA_cmdlist_reset(MICROUI_GraphicsContext *gc, int msaa);

/*
 * @brief Submits the commands list to the GPU.
 *
 * This function is blocking: it waits the end of the management of the GPU circular
 * command list by the NEMA library.
 *
 * @return DRAWING_RUNNING
 */
DRAWING_Status UI_NEMA_cmdlist_submit(void);

/*
 * @brief Function to call to notify the end of the last GPU operation (in the NEMA interrupt
 * routine).
 */
void UI_NEMA_post_operation(void);

/*
 * @brief Configures the copy from a buffer to another buffer.
 *
 * This function resets the GPU commands list and it is blocking: it waits the end of the
 * current GPU drawing before resetting the GPU commands list.
 *
 * @param[in] src the address of the buffer to copy.
 * @param[in] dst the address of the destination buffer.
 * @param[in] xmin the left X coordinate of the rectangle to copy.
 * @param[in] ymin the top Y coordinate of the rectangle to copy.
 * @param[in] xmax the right X coordinate of the rectangle to copy.
 * @param[in] ymax the bottom Y coordinate of the rectangle to copy.
 */
void UI_NEMA_configure_memcpy(uint8_t *src, uint8_t *dst, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax);

/*
 * @brief Starts the copy previously configured by a call to "UI_NEMA_configure_memcpy()".
 */
void UI_NEMA_start_memcpy(void);

/*
 * @brief Gets the clip set using UI_NEMA_set_clip().
 */
clip_rect UI_NEMA_get_clip(void);

/*
 * @brief Stores the given clip and sets it in the NEMA library with nema_set_clip().
 */
void UI_NEMA_set_clip(int32_t x, int32_t y, int32_t w, int32_t h);

/*
 * @brief Configures NEMA's clip according to the target's clip.
 *
 * If the clip is empty, the drawing is useless and this function returns false.
 * Otherwise the clip is configured to match the MicroUI clip.
 *
 * @param[in] gc: the target where performing the drawing (MicroUI Graphics Context)
 *
 * @return false when the drawing is useless (bounds out of clip), true otherwise
 */
bool UI_NEMA_set_clip_target(MICROUI_GraphicsContext *gc);

/*
 * @brief Configures NEMA's clip according to the MicroUI clip and
 * the given region.
 *
 * If the clip is empty or if the region is fully outside of the clip,
 * the drawing is useless and this function returns false.
 *
 * Otherwise the clip is configured to match the intersection of the
 * MicroUI clip and the region.
 *
 * @param[in] gc: the destination where performing the drawing (MicroUI Graphics Context)
 * @param[in] x1..y2: the region
 *
 * @return false when the drawing is useless (bounds out of clip), true otherwise
 */
bool UI_NEMA_set_clip_region(MICROUI_GraphicsContext *gc, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/*
 * @brief Configures NEMA's clip according to the MicroUI clip and
 * the given area, transformed with the given matrix.
 *
 * If the clip is empty or if the area is fully outside of the clip,
 * the drawing is useless and this function returns false.
 *
 * Otherwise the clip is configured to match the intersection of the
 * MicroUI clip and the transformed area.
 *
 * @param[in] gc: the destination where performing the drawing (MicroUI Graphics Context)
 * @param[in] width, height: the area size
 * @param[in] matrix: the transformation to apply on the area
 *
 * @return false when the drawing is useless (bounds out of clip), true otherwise
 */
bool UI_NEMA_set_clip_area(MICROUI_GraphicsContext *gc, jfloat width, jfloat height, const jfloat *matrix);

/*
 * @brief Converts an ARGB8888 color into NEMA's color format ABGR8888.
 */
static inline uint32_t UI_NEMA_convert_color(uint32_t c) {
	return (c & (uint32_t)0xff00ff00U) | ((c & (uint32_t)0xff0000u) >> 16) | ((c & (uint32_t)0xff) << 16);
}

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // UI_NEMA_H
