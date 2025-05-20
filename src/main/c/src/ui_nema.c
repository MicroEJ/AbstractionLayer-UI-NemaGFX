/*
 * C
 *
 * Copyright 2024-2025 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

/*
 * @file
 * @brief MicroEJ MicroUI library low level API: implementation of ui_nema.h.
 *
 * @author MicroEJ Developer Team
 * @version 4.0.1
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "ui_util.h"
#include "ui_nema.h"
#include "ui_nema_configuration.h"

#include "nema_core.h"

// -----------------------------------------------------------------------------
// Private variables
// -----------------------------------------------------------------------------

static bool _gpu_running;
static bool _nema_initialized = false;
static nema_cmdlist_t _command_list;
static clip_rect _current_clip = { 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT };

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------

// See the header file for the function documentation
void UI_NEMA_initialize(void) {
	if (!_nema_initialized) {
		nema_init();

		// use a circular queue to prevent the OOM (managed internally by the nema library)
		_command_list = nema_cl_create();
		nema_cl_bind_circular(&_command_list);

		_gpu_running = false;
		_nema_initialized = true;
	}
}

// See the header file for the function documentation
void UI_NEMA_cmdlist_reset(MICROUI_GraphicsContext *gc, int msaa) {
	nema_clear_dirty_region();
	nema_enable_aa(msaa, msaa, msaa, msaa);

	// configure destination
	nema_bind_dst_tex((uintptr_t)LLUI_DISPLAY_getBufferAddress(&gc->image), gc->image.width, gc->image.height,
	                  NEMA_RGB565, LLUI_DISPLAY_getStrideInBytes(&gc->image));
}

// See the header file for the function documentation
DRAWING_Status UI_NEMA_cmdlist_submit(void) {
	// ensure GPU is not currently in use (circular queue management: the Nema library may submit the first operations)
	nema_cl_wait(&_command_list);

	// launch GPU
	_gpu_running = true;
	nema_cl_submit(&_command_list);

	return DRAWING_RUNNING;
}

clip_rect UI_NEMA_get_clip() {
	return _current_clip;
}

void UI_NEMA_set_clip(int32_t x, int32_t y, int32_t w, int32_t h) {
	_current_clip.x = x;
	_current_clip.y = y;
	_current_clip.w = w;
	_current_clip.h = h;
	nema_set_clip(x, y, w, h);
}

// See the header file for the function documentation
bool UI_NEMA_set_clip_target(MICROUI_GraphicsContext *gc) {
	bool ret;
	int32_t width = UI_RECT_get_width(&gc->clip);
	int32_t height = UI_RECT_get_height(&gc->clip);

	if (LLUI_DISPLAY_isClipEnabled(gc)) {
		if ((width > 0) && (height > 0)) {
			// not "empty" clip

			// set the clip for the next drawing
			UI_NEMA_set_clip(gc->clip.x1, gc->clip.y1, width, height);

			// perform drawing
			ret = true;
		} else {
			// empty clip: nothing to draw
			ret = false;
		}
	} else {
		// clip is disabled :

		// disable clip
		UI_NEMA_set_clip(0, 0, gc->image.width, gc->image.height);

		// perform drawing
		ret = true;
	}

	return ret;
}

// See the header file for the function documentation
bool UI_NEMA_set_clip_region(MICROUI_GraphicsContext *gc, int x1, int y1, int x2, int y2) {
	bool ret;

	if (LLUI_DISPLAY_isClipEnabled(gc)) {
		if (LLUI_DISPLAY_clipRectangle(gc, (jint *)&x1, (jint *)&y1, (jint *)&x2, (jint *)&y2)) {
			// drawing fully or partially fits the clip:

			// set the clip for next drawing
			UI_NEMA_set_clip(gc->clip.x1, gc->clip.y1, UI_RECT_get_width(&gc->clip), UI_RECT_get_height(&gc->clip));

			// perform drawing
			ret = true;
		} else {
			// drawing is fully outside of the clip, nothing to draw
			ret = false;
		}
	} else {
		// clip is disabled :

		// disable clip
		UI_NEMA_set_clip(0, 0, gc->image.width, gc->image.height);

		// perform drawing
		ret = true;
	}

	return ret;
}

// See the header file for the function documentation
bool UI_NEMA_set_clip_area(MICROUI_GraphicsContext *gc, jfloat width, jfloat height, const jfloat *matrix) {
	bool ret;

	if (LLUI_DISPLAY_isClipEnabled(gc)) {
		// Transform each corners points of the image viewbox
		// top/left, not impacted by scale and rotate
		int x0 = (int)matrix[2];
		int y0 = (int)matrix[5];

		// top/right
		int x1 = (int)((width * matrix[0]) + x0);
		int y1 = (int)((width * matrix[3]) + y0);

		// bottom/Left
		int x2 = (int)((height * matrix[1]) + x0);
		int y2 = (int)((height * matrix[4]) + y0);

		// bottom/right
		int x3 = (int)(x2 + x1 - x0);
		int y3 = (int)(y2 + y1 - y0);

		// Compute the clipping area from all points
		int render_area_x = MIN(x0, MIN(x1, MIN(x2, x3)));
		int render_area_y = MIN(y0, MIN(y1, MIN(y2, y3)));
		int render_area_width = MAX(x0, MAX(x1, MAX(x2, x3))) - render_area_x;
		int render_area_height = MAX(y0, MAX(y1, MAX(y2, y3))) - render_area_y;

		// get GC clip
		int master_clip_x = gc->clip.x1;
		int master_clip_y = gc->clip.y1;
		int master_clip_width = UI_RECT_get_width(&gc->clip);
		int master_clip_height = UI_RECT_get_height(&gc->clip);

		// crop x
		if (render_area_x < master_clip_x) {
			//-> decrease width and set x to the minimum coordinate
			render_area_width -= master_clip_x - render_area_x;
			render_area_x = master_clip_x;
		}

		// crop y
		if (render_area_y < master_clip_y) {
			//-> decrease height and set y to the minimum coordinate
			render_area_height -= master_clip_y - render_area_y;
			render_area_y = master_clip_y;
		}

		int master_clip_outside_x = master_clip_x + master_clip_width;
		int master_clip_outside_y = master_clip_y + master_clip_height;

		// crop x+width and y+height (size is set to 0 if negative == empty clip)
		render_area_width = MAX(0, MIN(render_area_width, master_clip_outside_x - render_area_x));
		render_area_height = MAX(0, MIN(render_area_height, master_clip_outside_y - render_area_y));

		if ((0 < render_area_width) && (0 < render_area_height)) {
			UI_NEMA_set_clip(render_area_x, render_area_y, render_area_width, render_area_height);

			// perform drawing
			ret = true;
		} else {
			// drawing is fully outside of the clip, nothing to draw
			ret = false;
		}
	} else {
		// clip is disabled :

		// disable clip
		UI_NEMA_set_clip(0, 0, gc->image.width, gc->image.height);

		// perform drawing
		ret = true;
	}

	return ret;
}

// See the header file for the function documentation
void UI_NEMA_post_operation(void) {
	if (_gpu_running) {
		_gpu_running = false;
		LLUI_DISPLAY_notifyAsynchronousDrawingEnd(true);
	}
	// else: this post operation is not expected by the ui_nema
}

// See the header file for the function documentation
void UI_NEMA_configure_memcpy(uint8_t *src, uint8_t *dst, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax) {
	nema_set_clip(xmin, ymin, xmax - xmin + 1u, ymax - ymin + 1u);

	// configure destination
	// cppcheck-suppress [misra-c2012-11.4] Pointer cast for function signature.
	nema_bind_dst_tex((uintptr_t)dst, DISPLAY_WIDTH, DISPLAY_HEIGHT, FRAME_BUFFER_FORMAT, -1);

	// configure source
	nema_set_blend_blit(NEMA_BL_SRC_OVER);
	// cppcheck-suppress [misra-c2012-11.4] Pointer cast for function signature.
	nema_bind_src_tex((uintptr_t)src, DISPLAY_WIDTH, DISPLAY_HEIGHT, BACK_BUFFER_FORMAT, -1, NEMA_FILTER_PS);

	// add "copy buffer" cmd
	nema_blit_subrect(xmin, ymin, xmax - xmin + 1u, ymax - ymin + 1u, xmin, ymin);

	// the gpu is not yet running but other drawings must be blocked until memcpy is executed
	_gpu_running = true;
}

// See the header file for the function documentation
void UI_NEMA_start_memcpy(void) {
	// launch the memcpy previously configured
	nema_cl_submit(&_command_list);
}

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------
