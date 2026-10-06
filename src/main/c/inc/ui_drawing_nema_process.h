/*
 * Copyright 2023-2026 MicroEJ Corp.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined UI_DRAWING_NEMA_PROCESS_H
#define UI_DRAWING_NEMA_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file
 * @brief MicroEJ MicroUI library low level API: implementation over NemaGFX. Provides
 * a set of drawing functions over the NemaGFX library. First the MicroUI parameters
 * are converted to NemaGFX objects and then a drawing function is called to render the
 * drawing.
 * @author MicroEJ Developer Team
 * @version 5.0.2
 */

// --------------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------------

#include "ui_drawing.h"

#include "nema_vg.h"

// --------------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------------

/**
 * @brief Tells if the GPU can draw a fade (f).
 */
#define UI_DRAWING_NEMA_IS_COMPATIBLE_FADE(f) (true)

/**
 * @brief Gets the thickness according to a thickness (t) and a fade (f).
 */
#define UI_DRAWING_NEMA_GET_THICKNESS(t, f) ((t) + (f))

// --------------------------------------------------------------------------------
// Typedef
// --------------------------------------------------------------------------------

/**
 * @brief Function to draw a path.
 */
typedef DRAWING_Status (* UI_DRAWING_NEMA_PROCESS_draw_path_t) (MICROUI_GraphicsContext *gc, int seg_size, uint8_t *seg,
                                                                int data_size, nema_vg_float_t *data,
                                                                const nema_vg_float_t *bbox, uint8_t fill_rule,
                                                                nema_matrix3x3_t matrix, uint32_t blend,
                                                                uint32_t color);

/**
 * @brief Function to blit a part of the source.
 */
typedef DRAWING_Status (* UI_DRAWING_NEMA_PROCESS_blit_rect_t)(MICROUI_GraphicsContext *gc, void *source,
                                                               uint32_t *rect, nema_matrix3x3_t matrix, uint32_t blend,
                                                               uint32_t color, nema_tex_mode_t filter);

/**
 * @brief Function to clear the source.
 */
typedef DRAWING_Status (* UI_DRAWING_NEMA_PROCESS_clear_t)(MICROUI_GraphicsContext *gc, float x, float y, float w,
                                                           float h, uint32_t color);

// --------------------------------------------------------------------------------
// Public API
// --------------------------------------------------------------------------------

/**
 * @brief Tells if the source is GPU compatible. If yes, configures the given matrix,
 * blit rectangle and color and returns a NemaGFX source buffer to draw a raster image.
 * Returns NULL if the image cannot be drawn (image format not supported by the GPU)
 * or if the image region to draw is fully outside the clip.
 *
 * @param[in] gc the MicroUI GraphicsContext of the destination.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x_src the x coordinate of the region to draw in the image.
 * @param[in] y_src the y coordinate of the region to draw in the image.
 * @param[in] width the width of the region to draw.
 * @param[in] height the height of the region to draw.
 * @param[in] x_dest the x coordinate of the region in the destination.
 * @param[in] y_dest the y coordinate of the region in the destination.
 * @param[in] alpha the opacity to apply.
 * @param[out] color the color to configure.
 * @param[out] matrix the matrix to configure.
 * @param[out] blit_rect the blit rectangle to configure.
 *
 * @return a NemaGFX source buffer if the GPU can be used to draw the image.
 */
void * UI_DRAWING_NEMA_PROCESS_prepare_draw_image(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x_src,
                                                  jint y_src, jint width, jint height, jint x_dest, jint y_dest,
                                                  jint alpha, uint32_t *color, nema_matrix3x3_t matrix,
                                                  uint32_t *blit_rect);

#if 0
/**
 * @brief Implementation of drawLine over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the first pixel line X coordinate.
 * @param[in] startY the first pixel line Y coordinate.
 * @param[in] endX the last pixel line X coordinate.
 * @param[in] endY the last pixel line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawLine(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer, MICROUI_GraphicsContext *gc,
                                                jint startX, jint startY, jint endX, jint endY);

/**
 * @brief Implementation of drawHorizontalLine over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the first pixel line X coordinate.
 * @param[in] x2 the last pixel line X coordinate.
 * @param[in] y the both pixels line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawHorizontalLine(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                          MICROUI_GraphicsContext *gc, jint x1, jint x2, jint y);

/**
 * @brief Implementation of drawVerticalLine over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the both pixels line X coordinate.
 * @param[in] y1 the first pixel line Y coordinate.
 * @param[in] y2 the last pixel line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawVerticalLine(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                        MICROUI_GraphicsContext *gc, jint x, jint y1, jint y2);
#endif // if 0

/**
 * @brief Implementation of fillRectangle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to clear the source.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the top-left pixel X coordinate.
 * @param[in] y1 the top-left pixel Y coordinate.
 * @param[in] x2 the bottom-right pixel X coordinate.
 * @param[in] y2 the bottom-right pixel Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_fillRectangle(UI_DRAWING_NEMA_PROCESS_clear_t drawer,
                                                     MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2);

#if 0
/**
 * @brief Implementation of drawRoundedRectangle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] cornerEllipseWidth the horizontal diameter of the arc at the corners.
 * @param[in] cornerEllipseHeight the vertical diameter of the arc at the corners.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawRoundedRectangle(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                            MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                            jint height, jint cornerEllipseWidth,
                                                            jint cornerEllipseHeight);

/**
 * @brief Implementation of fillRoundedRectangle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] cornerEllipseWidth the horizontal diameter of the arc at the corners.
 * @param[in] cornerEllipseHeight the vertical diameter of the arc at the corners.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_fillRoundedRectangle(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                            MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                            jint height, jint cornerEllipseWidth,
                                                            jint cornerEllipseHeight);

/**
 * @brief Implementation of drawCircleArc over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the diameter of the arc to draw.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawCircleArc(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                     MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                                     jfloat startAngle, jfloat arcAngle);

/**
 * @brief Implementation of drawEllipseArc over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawEllipseArc(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                      MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                      jint height, jfloat startAngle, jfloat arcAngle);

/**
 * @brief Implementation of fillCircleArc over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the diameter of the arc to draw.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_fillCircleArc(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                     MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                                     jfloat startAngle, jfloat arcAngle);

/**
 * @brief Implementation of fillEllipseArc over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the rectangle width.
 * @param[in] height the rectangle height.
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_fillEllipseArc(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                      MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                      jint height, jfloat startAngle, jfloat arcAngle);

/**
 * @brief Implementation of drawEllipse over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the ellipse width.
 * @param[in] height the ellipse height.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawEllipse(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                   MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                   jint height);

/**
 * @brief Implementation of fillEllipse over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] width the ellipse width.
 * @param[in] height the ellipse height.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_fillEllipse(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                   MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                   jint height);

/**
 * @brief Implementation of drawCircle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawCircle(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                  MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter);

/**
 * @brief Implementation of fillCircle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_fillCircle(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                  MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter);

/**
 * @brief Implementation of drawThickFadedPoint over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the point X coordinate.
 * @param[in] y the point Y coordinate.
 * @param[in] thickness the point thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickFadedPoint(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                           MICROUI_GraphicsContext *gc, jint x, jint y, jint thickness);

/**
 * @brief Implementation of drawThickFadedLine over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the x coordinate of the start of the line
 * @param[in] startY the y coordinate of the start of the line
 * @param[in] endX the x coordinate of the end of the line
 * @param[in] endY the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 * @param[in] startCap cap representation of start of shape
 * @param[in] endCap cap representation of end of shape
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickFadedLine(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                          MICROUI_GraphicsContext *gc, jint startX, jint startY,
                                                          jint endX, jint endY, jint thickness, DRAWING_Cap startCap,
                                                          DRAWING_Cap endCap);

/**
 * @brief Implementation of drawThickFadedCircle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickFadedCircle(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                            MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                                            jint thickness);

/**
 * @brief Implementation of drawThickFadedCircleArc over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 * @param[in] thickness the arc thickness.
 * @param[in] start cap representation of start of shape
 * @param[in] end cap representation of end of shape
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickFadedCircleArc(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                               MICROUI_GraphicsContext *gc, jint x, jint y,
                                                               jint diameter, jfloat startAngle, jfloat arcAngle,
                                                               jint thickness, DRAWING_Cap start, DRAWING_Cap end);

/**
 * @brief Implementation of drawThickFadedEllipse over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] y the y coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] width the width of the ellipse to draw
 * @param[in] height the height of the ellipse to draw
 * @param[in] thickness the ellipse thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickFadedEllipse(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                             MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                             jint height, jint thickness);

/**
 * @brief Implementation of drawThickLine over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the x coordinate of the start of the line
 * @param[in] startY the y coordinate of the start of the line
 * @param[in] endX the x coordinate of the end of the line
 * @param[in] endY the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickLine(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                     MICROUI_GraphicsContext *gc, jint startX, jint startY, jint endX,
                                                     jint endY, jint thickness);

/**
 * @brief Implementation of drawThickCircle over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickCircle(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                       MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                                       jint thickness);

/**
 * @brief Implementation of drawThickEllipse over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] y the y coordinate of the upper-left corner of the rectangle where the ellipse is drawn
 * @param[in] width the width of the ellipse to draw
 * @param[in] height the height of the ellipse to draw
 * @param[in] thickness the ellipse thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickEllipse(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                        MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                        jint height, jint thickness);

/**
 * @brief Implementation of drawThickCircleArc over NemaVG. See ui_drawing.h
 *
 * @param[in] drawer the function to draw a path.
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the arc is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] startAngle the beginning angle of the arc to draw
 * @param[in] arcAngle the angular extent of the arc from startAngle
 * @param[in] thickness the arc thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawThickCircleArc(UI_DRAWING_NEMA_PROCESS_draw_path_t drawer,
                                                          MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                                          jfloat startAngle, jfloat arcAngle, jint thickness);

/*
 * @brief Implementation of drawFlippedImage over NemaVG. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawFlippedImage(UI_DRAWING_NEMA_PROCESS_blit_rect_t drawer,
                                                        MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint regionX,
                                                        jint regionY, jint width, jint height, jint x, jint y,
                                                        DRAWING_Flip transformation, jint alpha,
                                                        bool *is_gpu_compatible);

/*
 * @brief Implementation of drawRotatedImageNearestNeighbor over NemaVG. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawRotatedImageNearestNeighbor(UI_DRAWING_NEMA_PROCESS_blit_rect_t drawer,
                                                                       MICROUI_GraphicsContext *gc, MICROUI_Image *img,
                                                                       jint x, jint y, jint rotationX, jint rotationY,
                                                                       jfloat angle, jint alpha,
                                                                       bool *is_gpu_compatible);

/*
 * @brief Implementation of drawRotatedImageBilinear over NemaVG. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawRotatedImageBilinear(UI_DRAWING_NEMA_PROCESS_blit_rect_t drawer,
                                                                MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x,
                                                                jint y, jint rotationX, jint rotationY, jfloat angle,
                                                                jint alpha, bool *is_gpu_compatible);

/*
 * @brief Implementation of drawScaledImageNearestNeighbor over NemaVG. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawScaledImageNearestNeighbor(UI_DRAWING_NEMA_PROCESS_blit_rect_t drawer,
                                                                      MICROUI_GraphicsContext *gc, MICROUI_Image *img,
                                                                      jint x, jint y, jfloat factorX, jfloat factorY,
                                                                      jint alpha, bool *is_gpu_compatible);

/*
 * @brief Implementation of drawScaledImageBilinear over NemaVG. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_PROCESS_drawScaledImageBilinear(UI_DRAWING_NEMA_PROCESS_blit_rect_t drawer,
                                                               MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x,
                                                               jint y, jfloat factorX, jfloat factorY, jint alpha,
                                                               bool *is_gpu_compatible);

#endif // if 0

// --------------------------------------------------------------------------------
// EOF
// --------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // UI_DRAWING_NEMA_PROCESS_H
