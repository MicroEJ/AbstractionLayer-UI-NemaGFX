/*
 * C
 *
 * Copyright 2023-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <LLUI_DISPLAY_impl.h>

#include "ui_drawing_nema.h"
#include "ui_drawing_soft.h"
#include "dw_drawing_soft.h"
#include "bsp_util.h"

#include "nema_blender.h"
#include "nema_matrix3x3.h"
#include "nema_utils.h"

// -----------------------------------------------------------------------------
// Configuration Sanity Check
// -----------------------------------------------------------------------------

/**
 * Sanity check between the expected version of the configuration and the actual version of
 * the configuration.
 * If an error is raised here, it means that a new version of the CCO has been installed and
 * the configuration ui_drawing_nema_configuration.h must be updated based on the one provided
 * by the new CCO version.
 */

#if !defined NEMA_CONFIGURATION_VERSION
#error "Undefined NEMA_CONFIGURATION_VERSION, it must be defined in ui_drawing_nema_configuration.h"
#endif

#if defined NEMA_CONFIGURATION_VERSION && NEMA_CONFIGURATION_VERSION != 2
#error "Version of the configuration file ui_drawing_nema_configuration.h is not compatible with this implementation."
#endif

// -----------------------------------------------------------------------------
// Macros and Defines
// -----------------------------------------------------------------------------

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define MSAA_ENABLED (1)
#define MSAA_DISABLED (0)

/*
 * @brief A fade higher than 1 is not compatible with NemaGFX
 */
#define IS_COMPATIBLE_FADE(f) ((f) <= 1)

/*
 * @brief Gets the thickness according to a thickness (t) and a fade (f).
 */
#define GET_THICKNESS(t, f) ((t) + (f))

// -----------------------------------------------------------------------------
// Static Variables
// -----------------------------------------------------------------------------

static nema_cmdlist_t command_list;

/*
 * @brief This boolean is set to true before launching a GPU action and back to
 * false during GPU interrupt. It allows to know if the GPU is running or not.
 */
static bool g_gpu_running;

/*
 * @brief Binary semaphore used to lock a GPU user when GPU is already running.
 */
static void* _sem_gpu;

// -----------------------------------------------------------------------------
// Internal functions
// -----------------------------------------------------------------------------

/*
 * @brief Ensures GPU previous work is done before returning.
 */
static inline void _wait_previous_operation(void) {
	while(g_gpu_running) {
		LLUI_DISPLAY_IMPL_binarySemaphoreTake(_sem_gpu);
	}
}

/*
 * @brief Submits a new GPU operation and notifies it.
 */
static inline DRAWING_Status _submit_operation(void) {
	g_gpu_running = true;
	nema_cl_submit(&command_list);
	UI_DRAWING_NEMA_IMPL_operation_submitted();
	return DRAWING_RUNNING;
}

/*
 * @brief Converts a ARGB8888 color to NEMA color format ABGR8888
 */
static inline uint32_t _convert_color(uint32_t c) {
	return (c & 0xff00ff00u) | ((c & 0xff0000u) >> 16) | ((c & 0xffu) << 16);
}

/*
 * @brief Resets the GPU command list with the new destination (RGB565 format for sure).
 * Returns false when there is nothing to draw.
 * Updates nema_color with graphics context foreground color (in Nema GPU format)
 */
static bool _nemacmdlist_reset(MICROUI_GraphicsContext* gc, int x1, int y1, int x2, int y2, uint32_t* nema_color, int msaa) {
	bool ret = true;

	// ensure GPU is not currently in use
	_wait_previous_operation();

	// reset GPU commmand list
	nema_cl_rewind(&command_list);
	nema_cl_bind(&command_list);
	nema_clear_dirty_region();
	nema_enable_aa(msaa, msaa, msaa, msaa);
	nema_set_blend_fill(NEMA_BL_SRC);

	if (LLUI_DISPLAY_isClipEnabled(gc)) {
		if (LLUI_DISPLAY_clipRectangle(gc, &x1, &y1, &x2, &y2)) {
			// drawing fully or partially fits the clip: enable clip
			nema_set_clip(gc->clip.x1, gc->clip.y1, UI_RECT_get_width(&gc->clip), UI_RECT_get_height(&gc->clip));
		}
		else {
			// drawing is fully outside the clip, nothing to draw
			ret = false;
		}
	}
	else {
		// clip is disabled: reset clip
		nema_set_clip(0, 0, gc->image.width, gc->image.height);
	}

	if (ret) {
		// configure destination
		nema_bind_dst_tex(
				(uintptr_t)LLUI_DISPLAY_getBufferAddress(&gc->image),
				gc->image.width,
				gc->image.height,
				BACK_BUFFER_FORMAT,
				LLUI_DISPLAY_getStrideInBytes(&gc->image)
		);

		// retrieve the color to use
		*nema_color = _convert_color(gc->foreground_color);
	}

	// perform drawing
	return ret;
}

/*
 * @brief Configure the image source if the image format is supported by the GPU.
 * @return true if the image format is supported by the GPU
 */
static bool _nemacmdlist_configure_source(MICROUI_Image* img, uint32_t nema_color, uint32_t alpha, nema_tex_mode_t mode) {
	nema_tex_format_t format;
	uint32_t blending_mode;
	bool ret = true;

	switch(img->format) {
	case MICROUI_IMAGE_FORMAT_ARGB8888:
		format = NEMA_BGRA8888;
		blending_mode = NEMA_BL_SIMPLE;
		break;
	case MICROUI_IMAGE_FORMAT_RGB565:
		format = NEMA_RGB565;
		blending_mode = NEMA_BL_SRC_OVER;
		break;
	case MICROUI_IMAGE_FORMAT_A8:
		format = NEMA_A8;
		blending_mode = NEMA_BL_SIMPLE;
		nema_set_tex_color(nema_color);
		break;

	case MICROUI_IMAGE_FORMAT_A4:
	case MICROUI_IMAGE_FORMAT_A2:
	case MICROUI_IMAGE_FORMAT_A1:
		// [MicroEJ versus Nema] pixels are not in the same order in the byte

	case MICROUI_IMAGE_FORMAT_ARGB1555:
	case MICROUI_IMAGE_FORMAT_ARGB4444:
		// [MicroEJ versus Nema] blue and red are inverted

	default:
		ret = false;
		break;
	}

	if (ret) {
		nema_set_blend_blit (blending_mode | NEMA_BLOP_MODULATE_A);
		nema_set_const_color(alpha << 24);
		nema_bind_src_tex ((uintptr_t)LLUI_DISPLAY_getBufferAddress(img),
				img->width, img->height,
				format,
				LLUI_DISPLAY_getStrideInBytes(img),
				mode);
	}

	return ret;
}

/*
 * @brief Rotates an image according to given nema_tex_mode_t.
 * @return true when drawing is done/started; false when caller has to perform the
 * drawing in software.
 */
static bool _rotate_image(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jfloat factorX, jfloat factorY, jint alpha, nema_tex_mode_t mode, DRAWING_Status* status) {
	bool ret = true;

	uint32_t color;
	if (!_nemacmdlist_reset(gc, 0, 0, gc->image.width, gc->image.height, &color, MSAA_ENABLED)) {
		*status = DRAWING_DONE;
	} else {
		// configure GPU with the image to render
		if (!_nemacmdlist_configure_source(img, color, alpha, mode)) {
			ret = false;
		}
		else {
			// nema_blit_quad_fit reference point is image's centered point
			float x0 = ((float)-img->width) / 2.f;
			float y0 = ((float)-img->height) / 2.f;
			float x1 = x0 + img->width;
			float y1 = y0;
			float x2 = x0 + img->width;
			float y2 = y0 + img->height;
			float x3 = x0;
			float y3 = y0 + img->height;

			float tx = (-x0 * factorX) + x - rotationX;
			float ty = (-y0 * factorY) + y - rotationY;

			// calculate rotation matrix
			// (x,y) is the image's position when there is no transformation (top-left anchor)
			nema_matrix3x3_t m;
			nema_mat3x3_load_identity(m);
			nema_mat3x3_scale(m, factorX, factorY);
			nema_mat3x3_translate(m, tx, ty);
			nema_mat3x3_rotate(m, -angle);
			nema_mat3x3_translate(m, rotationX, rotationY);	

			// apply matrix
			nema_mat3x3_mul_vec(m, &x0, &y0);
			nema_mat3x3_mul_vec(m, &x1, &y1);
			nema_mat3x3_mul_vec(m, &x2, &y2);
			nema_mat3x3_mul_vec(m, &x3, &y3);

			// draw image
			nema_blit_quad_fit(x0, y0,
					x1, y1,
					x2, y2,
					x3, y3);

			*status = _submit_operation();
		}
	}

	return ret;
}

/*
 * @brief Scales an image according to given nema_tex_mode_t.
 * @return true when drawing is done/started; false when caller has to perform the
 * drawing in software.
 */
static bool _scale_image(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha, nema_tex_mode_t mode, DRAWING_Status* status) {
	bool ret = true;
	uint32_t color;
	if (!_nemacmdlist_reset(
			gc, x, y,
			(int) (x + (factorX * img->width)),
			(int) (y + (factorY * img->height)),
			&color, MSAA_ENABLED)) {
		*status = DRAWING_DONE;
	}
	else {
		// configure GPU with the image to render
		if (!_nemacmdlist_configure_source(img, color, alpha, mode)) {
			ret = false;
		} 
		else {
			nema_blit_rect_fit(x, y, (int)(img->width * factorX), (int)(img->height * factorY));
			*status = _submit_operation();
		}
	}
	return ret;
}

/*
 * @brief Draw in the same image with overlaps.
 * This function will slice the drawing in multiple chunks to avoid overlapping drawings.
 * @return the drawing status.
 */
static DRAWING_Status _draw_region(uint32_t rect[4], nema_matrix3x3_t matrix, uint32_t element_index) {
	// rect[x,y,w,h]
	uint32_t rect_index = element_index + 2u;

	// retrieve band's size (width or height)
	jint size = rect[rect_index];
	rect[rect_index] = (uint32_t)(matrix[element_index][2]) - rect[element_index];

	// go to x + band size
	rect[element_index] += size;
	// cppcheck-suppress [misra-c2012-10.8] Cast for include in nema matrix.
	matrix[element_index][2] = (float)(rect[element_index] + rect[rect_index]);

	while (size > 0) {
		// adjust band's size
		if (size < rect[rect_index]) {
			rect[rect_index] = size;
		}

		// adjust src & dest positions
		rect[element_index] -= rect[rect_index];
		matrix[element_index][2] -= rect[rect_index];

		size -= rect[rect_index];
		nema_blit_subrect((int)matrix[0][2], (int)matrix[1][2], rect[2], rect[3], rect[0], rect[1]);
	}

	return _submit_operation();
}

#ifdef ENABLE_DYNAMIC_SHAPES
DRAWING_Status _render_circle(MICROUI_GraphicsContext* gc, jfloat x, jfloat y, jfloat diameter, jfloat thickness, jint msaa) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc,
			(int)(x - (thickness / 2)),
			(int)(y - (thickness / 2)),
			(int)(x + diameter - 1 + (thickness / 2)),
			(int)(y + diameter - 1 + (thickness / 2)),
			&color, msaa)) {
		jfloat radius = diameter / 2;
		if (0 == thickness) {
			nema_fill_circle_aa(x + radius, y + radius, radius, color);
		}
		else {
			nema_draw_circle_aa(x + radius, y + radius, radius, thickness, color);
		}
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}
#endif // ENABLE_DYNAMIC_SHAPES

// -----------------------------------------------------------------------------
// Public functions
// -----------------------------------------------------------------------------

// See the header file for the function documentation
void UI_DRAWING_NEMA_initialize(void* binary_semaphore) {
	_sem_gpu = binary_semaphore;
	nema_init();

#ifdef ENABLE_DYNAMIC_SHAPES
#warning "GPU's command list size is dynamic!"
	// when the command list cannot be increased, the GPU discards the drawing.
	command_list = nema_cl_create();
#else
	command_list = nema_cl_create_sized(0x200); // at least 277 bytes
#endif
	g_gpu_running = false;
}

void UI_DRAWING_NEMA_post_operation(void) {

	bool from_isr;

#ifndef NEMA_INTERRUPT_MODE
	// "task mode": ensure last operation has been fully done
	nema_cl_wait(&command_list);
	from_isr = false;
#else
	// the operation is done for sure because it is called from the GPU interrupt
	from_isr = true;
#endif

	g_gpu_running = false;
	LLUI_DISPLAY_IMPL_binarySemaphoreGive(_sem_gpu, from_isr);
	LLUI_DISPLAY_notifyAsynchronousDrawingEnd(from_isr);
}

void UI_DRAWING_NEMA_configure_memcpy(uint8_t* src, uint8_t* dst, uint32_t xmin, uint32_t ymin, uint32_t xmax, uint32_t ymax) {

	// ensure GPU is not currently in use
	_wait_previous_operation();

	// reset GPU commmand list
	nema_cl_rewind(&command_list);
	nema_cl_bind(&command_list);
	nema_set_clip(xmin, ymin, xmax - xmin + 1u, ymax - ymin + 1u);

	// configure destination
	nema_bind_dst_tex(
			// cppcheck-suppress [misra-c2012-11.4] Pointer cast for function signature.
			(uintptr_t) dst,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
			FRAME_BUFFER_FORMAT,
			-1
	);

	// configure source
	nema_set_blend_blit (NEMA_BL_SRC_OVER);
	// cppcheck-suppress [misra-c2012-11.4] Pointer cast for function signature.
	nema_bind_src_tex ((uintptr_t)src,
			DISPLAY_WIDTH, DISPLAY_HEIGHT,
			BACK_BUFFER_FORMAT,
			-1,
			NEMA_FILTER_PS
	);

	// add "copy buffer" cmd
	nema_blit_subrect(xmin, ymin, xmax - xmin + 1u, ymax - ymin + 1u, xmin, ymin);

	// the gpu is not yet running but other drawings must be blocked until memcpy is executed
	g_gpu_running = true;
}

// See the header file for the function documentation
void UI_DRAWING_NEMA_start_memcpy(void) {
	// launch the memcpy previously configured
	(void)_submit_operation();
}

// -----------------------------------------------------------------------------
// Low Level API
// -----------------------------------------------------------------------------

#ifdef NEMA_INTERRUPT_MODE
// See the header file for the function documentation
BSP_DECLARE_WEAK_FCNT void UI_DRAWING_NEMA_IMPL_operation_submitted(void) {
	// nothing to do
}
#endif

// -----------------------------------------------------------------------------
// ui_drawing.h functions
// -----------------------------------------------------------------------------

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;
	int min_x = MIN(startX, endX);
	int min_y = MIN(startY, endY);
	int max_x = MAX(startX, endX);
	int max_y = MAX(startY, endY);

	if (_nemacmdlist_reset(gc, min_x, min_y, max_x, max_y, &color, MSAA_DISABLED)) {
		nema_draw_line(startX, startY, endX, endY, color);
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}

#ifdef ENABLE_SIMPLE_LINES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawHorizontalLine(MICROUI_GraphicsContext* gc, jint x1, jint x2, jint y) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc, x1, y, x2, y, &color, MSAA_DISABLED)) {
		nema_draw_line(x1, y, x2, y, color);
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}
#endif // ENABLE_SIMPLE_LINES

#ifdef ENABLE_SIMPLE_LINES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawVerticalLine(MICROUI_GraphicsContext* gc, jint x, jint y1, jint y2) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc, x, y1, x, y2, &color, MSAA_DISABLED)) {
		nema_draw_line(x, y1, x, y2, color);
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}
#endif // ENABLE_SIMPLE_LINES

#ifdef ENABLE_SIMPLE_LINES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc, x1, y1, x2, y2, &color, MSAA_DISABLED)) {
		nema_draw_rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, color);
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}
#endif // ENABLE_SIMPLE_LINES

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_fillRectangle(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc, x1, y1, x2, y2, &color, MSAA_DISABLED)) {
		nema_fill_rect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, color);
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}

#ifdef ENABLE_DYNAMIC_SHAPES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if ((cornerEllipseWidth == cornerEllipseHeight) && ((cornerEllipseWidth & 1) == 0)) {
		if (_nemacmdlist_reset(gc, x, y, x + width - 1, y + height - 1, &color, MSAA_DISABLED)) {
			nema_draw_rounded_rect(x, y, width, height, cornerEllipseWidth / 2, color);
			status = _submit_operation();
		}
		// else: out of clip
	}
	else {
		// GPU handles only circular corners with an even diameter
		status = UI_DRAWING_SOFT_drawRoundedRectangle(gc, x, y, width, height,cornerEllipseWidth, cornerEllipseHeight);
	}
	return status;
}
#endif // ENABLE_DYNAMIC_SHAPES

#ifdef ENABLE_DYNAMIC_SHAPES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_fillRoundedRectangle(MICROUI_GraphicsContext* gc, jint x, jint y, jint width, jint height, jint cornerEllipseWidth, jint cornerEllipseHeight) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if ((cornerEllipseWidth == cornerEllipseHeight) && ((cornerEllipseWidth & 1) == 0)){
		if(_nemacmdlist_reset(gc, x, y, x + width - 1, y + height - 1, &color, MSAA_DISABLED)) {
			nema_fill_rounded_rect(x, y, width, height, cornerEllipseWidth / 2, color);
			status = _submit_operation();
		}
		// else: out of clip
	}
	else {
		// GPU handles only circular corners with an even diameter
		status = UI_DRAWING_SOFT_fillRoundedRectangle(gc, x, y, width, height,cornerEllipseWidth, cornerEllipseHeight);
	}
	return status;
}
#endif // ENABLE_DYNAMIC_SHAPES

#ifdef ENABLE_DYNAMIC_SHAPES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter) {
	return _render_circle(gc, x, y, diameter, 1, MSAA_DISABLED);
}
#endif // ENABLE_DYNAMIC_SHAPES

#ifdef ENABLE_DYNAMIC_SHAPES
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_fillCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter) {
	return _render_circle(gc, x, y, diameter, 0, MSAA_DISABLED);
}
#endif // ENABLE_DYNAMIC_SHAPES

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y, jint alpha) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc, x, y, x + width - 1, y + height - 1, &color, MSAA_ENABLED)) {

		// configure GPU with the image to render
		if (_nemacmdlist_configure_source(img, color, alpha, NEMA_FILTER_PS)) {
			// render the image
			nema_blit_subrect(x, y, width, height, regionX, regionY);
			status = _submit_operation();
		}
		else {
			status = UI_DRAWING_SOFT_drawImage(gc, img, regionX, regionY, width, height, x, y, alpha);
		}
	}
	// else: out of clip

	return status;
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawRegion(MICROUI_GraphicsContext* gc, jint x_src, jint y_src, jint width, jint height, jint x_dest, jint y_dest, jint alpha) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (_nemacmdlist_reset(gc, x_dest, y_dest, x_dest + width - 1, y_dest + height - 1, &color, MSAA_ENABLED)) {

		if (_nemacmdlist_configure_source(&gc->image, color, alpha, NEMA_FILTER_PS)) {

			uint32_t blit_rect[4];
			blit_rect[0] = x_src;
			blit_rect[1] = y_src;
			blit_rect[2] = width;
			blit_rect[3] = height;

			nema_matrix3x3_t matrix;
			nema_mat3x3_load_identity(matrix);
			matrix[0][2] = x_dest;
			matrix[1][2] = y_dest;

			if ((y_dest == y_src) && (x_dest > x_src) && (x_dest < (x_src + width))) {
				// draw with overlap: cut the drawings in several widths
				status = _draw_region(blit_rect, matrix, 0);
			}
			else if ((y_dest > y_src) && (y_dest < (y_src + height))) {
				// draw with overlap: cut the drawings in several heights
				status = _draw_region(blit_rect, matrix, 1);
			}
			else {
				// draw in one shot
				nema_blit_subrect(x_dest, y_dest, width, height, x_src, y_src);
				status = _submit_operation();
			}
		}
		else {
			status = UI_DRAWING_SOFT_drawRegion(gc, x_src, y_src, width, height, x_dest, y_dest, alpha);
		}
	}
	// else: out of clip

	return status;
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_copyImage(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint regionX, jint regionY, jint width, jint height, jint x, jint y) {
	return (img == &gc->image) ?
			// have to manage the overlap
			UI_DRAWING_NEMA_drawRegion(gc, regionX, regionY, width, height, x, y, 0xff)
			// no overlap: draw image as usual
			: UI_DRAWING_NEMA_drawImage(gc, img, regionX, regionY, width, height, x, y, 0xff);
}

#ifdef ENABLE_IMAGE_ROTATION
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawRotatedImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha) {
	DRAWING_Status status;
	if (!_rotate_image(gc, img, x, y, rotationX, rotationY, angle, 1.0f, 1.0f, alpha, NEMA_FILTER_BL, &status)) {
		status = DW_DRAWING_SOFT_drawRotatedImageBilinear(gc, img, x, y, rotationX, rotationY, angle, alpha);
	}
	return status;
}
#endif // ENABLE_IMAGE_ROTATION

#ifdef ENABLE_IMAGE_ROTATION
// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jint rotationX, jint rotationY, jfloat angle, jint alpha) {
	DRAWING_Status status;
	if (!_rotate_image(gc, img, x, y, rotationX, rotationY, angle, 1.0f, 1.0f, alpha, NEMA_FILTER_PS, &status)) {
		status = DW_DRAWING_SOFT_drawRotatedImageNearestNeighbor(gc, img, x, y, rotationX, rotationY, angle, alpha);
	}
	return status;
}
#endif // ENABLE_IMAGE_ROTATION

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawScaledImageBilinear(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha) {
	DRAWING_Status status;
	if (!_scale_image(gc, img, x, y, factorX, factorY, alpha, NEMA_FILTER_BL, &status)) {
		status = DW_DRAWING_SOFT_drawScaledImageBilinear(gc, img, x, y, factorX, factorY, alpha);
	}
	return status;
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext* gc, MICROUI_Image* img, jint x, jint y, jfloat factorX, jfloat factorY, jint alpha) {
	DRAWING_Status status;
	if (!_scale_image(gc, img, x, y, factorX, factorY, alpha, NEMA_FILTER_PS, &status)) {
		status = DW_DRAWING_SOFT_drawScaledImageNearestNeighbor(gc, img, x, y, factorX, factorY, alpha);
	}
	return status;
}

// See the header file for the function documentation
DRAWING_Status UI_DRAWING_NEMA_drawThickLine(MICROUI_GraphicsContext* gc, jint x1, jint y1, jint x2, jint y2, jint thickness) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	int min_x = MIN(x1, x2);
	int min_y = MIN(y1, y2);
	int max_x = MAX(x1, x2);
	int max_y = MAX(y1, y2);

	if (_nemacmdlist_reset(
			gc,
			min_x - (thickness / 2),
			min_y - (thickness / 2),
			max_x + (thickness / 2),
			max_y + (thickness / 2),
			&color, MSAA_DISABLED)) {
		nema_draw_line_aa(x1, y1, x2, y2, thickness, color);
		status = _submit_operation();
	}
	// else: out of clip

	return status;
}

#ifdef ENABLE_DYNAMIC_SHAPES
DRAWING_Status UI_DRAWING_NEMA_drawThickCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness) {
	return _render_circle(gc, x, y, diameter, thickness, MSAA_DISABLED);
}
#endif

#ifdef ENABLE_FADED_LINES
DRAWING_Status UI_DRAWING_NEMA_drawThickFadedLine(MICROUI_GraphicsContext* gc, jint startX, jint startY, jint endX, jint endY, jint thickness, jint fade, DRAWING_Cap startCap, DRAWING_Cap endCap) {

	DRAWING_Status status = DRAWING_DONE;
	uint32_t color;

	if (IS_COMPATIBLE_FADE(fade) && (startCap == DRAWING_ENDOFLINE_NONE) && (endCap == DRAWING_ENDOFLINE_NONE)) {
		int min_x = MIN(startX, endX);
		int min_y = MIN(startY, endY);
		int max_x = MAX(startX, endX);
		int max_y = MAX(startY, endY);
		if (_nemacmdlist_reset(
				gc,
				min_x - (thickness / 2),
				min_y - (thickness / 2),
				max_x + (thickness / 2),
				max_y + (thickness / 2),
				&color, (fade == 0) ? MSAA_DISABLED : MSAA_ENABLED)) {
			nema_draw_line_aa(startX, startY, endX, endY, GET_THICKNESS(thickness, fade), color);
			status = _submit_operation();
		}
		// else: out of clip

	} else {
		status = DW_DRAWING_SOFT_drawThickFadedLine(gc, startX, startY, endX, endY, thickness, fade, startCap, endCap);
	}

	return status;
}
#endif

#ifdef ENABLE_DYNAMIC_SHAPES
DRAWING_Status UI_DRAWING_NEMA_drawThickFadedCircle(MICROUI_GraphicsContext* gc, jint x, jint y, jint diameter, jint thickness, jint fade) {
	DRAWING_Status status;
	if (IS_COMPATIBLE_FADE(fade)) {
		status = _render_circle(gc, x, y, diameter, GET_THICKNESS(thickness, fade), MSAA_ENABLED);
	}
	else {
		status = DW_DRAWING_SOFT_drawThickFadedCircle(gc, x, y, diameter, thickness, fade);
	}
	return status;
}
#endif

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------
