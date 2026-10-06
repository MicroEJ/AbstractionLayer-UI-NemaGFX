/*
 * C
 *
 * Copyright 2023-2026 MicroEJ Corp.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined UI_DRAWING_NEMA_H
#define UI_DRAWING_NEMA_H

#if defined __cplusplus
extern "C" {
#endif

/**
 * @file
 * @brief Implementation of a set of ui_drawing.h drawing functions (MicroUI library). These are
 * implementations over the NemaGFX and the destination buffer format is NEMA_RGB565. When the
 * drawing cannot be performed by the GPU, the software implementation is used insted.
 *
 * @author MicroEJ Developer Team
 * @version 5.0.2
 */

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "ui_drawing.h"
#include "ui_nema_configuration.h"

#include "nema_cmdlist.h"

// -----------------------------------------------------------------------------
// Macros
// -----------------------------------------------------------------------------

#if !defined(UI_GC_SUPPORTED_FORMATS) || (UI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_NEMA_xxx() are directly called by LLUI_PAINTER_impl.c
 * and LLDW_PAINTER_impl.c. This file overrides each function independently to use the
 * NemaGFX.
 */

/** @cond */
#define UI_DRAWING_NEMA_drawLine UI_DRAWING_drawLine
/** @endcond */

#if defined ENABLE_SIMPLE_LINES && (ENABLE_SIMPLE_LINES == 1)

/** @cond */
#define UI_DRAWING_NEMA_drawHorizontalLine UI_DRAWING_drawHorizontalLine
#define UI_DRAWING_NEMA_drawVerticalLine UI_DRAWING_drawVerticalLine
#define UI_DRAWING_NEMA_drawRectangle UI_DRAWING_drawRectangle
/** @endcond */

#endif // ENABLE_SIMPLE_LINES

/** @cond */
#define UI_DRAWING_NEMA_fillRectangle UI_DRAWING_fillRectangle
#define UI_DRAWING_NEMA_drawRoundedRectangle UI_DRAWING_drawRoundedRectangle
#define UI_DRAWING_NEMA_fillRoundedRectangle UI_DRAWING_fillRoundedRectangle
#define UI_DRAWING_NEMA_drawCircle UI_DRAWING_drawCircle
#define UI_DRAWING_NEMA_fillCircle UI_DRAWING_fillCircle
#define UI_DRAWING_NEMA_drawImage UI_DRAWING_drawImage
#define UI_DRAWING_NEMA_drawRegion UI_DRAWING_drawRegion
#define UI_DRAWING_NEMA_copyImage UI_DRAWING_copyImage

#define UI_DRAWING_NEMA_drawRotatedImageBilinear UI_DRAWING_drawRotatedImageBilinear
#define UI_DRAWING_NEMA_drawRotatedImageNearestNeighbor UI_DRAWING_drawRotatedImageNearestNeighbor
#define UI_DRAWING_NEMA_drawScaledImageBilinear UI_DRAWING_drawScaledImageBilinear
#define UI_DRAWING_NEMA_drawScaledImageNearestNeighbor UI_DRAWING_drawScaledImageNearestNeighbor
#define UI_DRAWING_NEMA_drawThickLine UI_DRAWING_drawThickLine
#define UI_DRAWING_NEMA_drawThickFadedLine UI_DRAWING_drawThickFadedLine
#define UI_DRAWING_NEMA_drawThickCircle UI_DRAWING_drawThickCircle
#define UI_DRAWING_NEMA_drawThickFadedCircle UI_DRAWING_drawThickFadedCircle
/** @endcond */

#else // !defined(UI_GC_SUPPORTED_FORMATS) || (UI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_NEMA_xxx() are indirectly called through some tables.
 * The functions have got the identifier "0" as suffix. This file overrides each function
 * independently to use the NemaGFX.
 */

/** @cond */
#define UI_DRAWING_NEMA_drawLine UI_DRAWING_drawLine_0
/** @endcond */

#if defined ENABLE_SIMPLE_LINES && (ENABLE_SIMPLE_LINES == 1)

/** @cond */
#define UI_DRAWING_NEMA_drawHorizontalLine UI_DRAWING_drawHorizontalLine_0
#define UI_DRAWING_NEMA_drawVerticalLine UI_DRAWING_drawVerticalLine_0
#define UI_DRAWING_NEMA_drawRectangle UI_DRAWING_drawRectangle_0
/** @endcond */

#endif // ENABLE_SIMPLE_LINES

/** @cond */
#define UI_DRAWING_NEMA_fillRectangle UI_DRAWING_fillRectangle_0
#define UI_DRAWING_NEMA_drawRoundedRectangle UI_DRAWING_drawRoundedRectangle_0
#define UI_DRAWING_NEMA_fillRoundedRectangle UI_DRAWING_fillRoundedRectangle_0
#define UI_DRAWING_NEMA_drawCircle UI_DRAWING_drawCircle_0
#define UI_DRAWING_NEMA_fillCircle UI_DRAWING_fillCircle_0
#define UI_DRAWING_NEMA_drawImage UI_DRAWING_drawImage_0
#define UI_DRAWING_NEMA_drawRegion UI_DRAWING_drawRegion_0
#define UI_DRAWING_NEMA_copyImage UI_DRAWING_copyImage_0

#define UI_DRAWING_NEMA_drawRotatedImageBilinear UI_DRAWING_drawRotatedImageBilinear_0
#define UI_DRAWING_NEMA_drawRotatedImageNearestNeighbor UI_DRAWING_drawRotatedImageNearestNeighbor_0
#define UI_DRAWING_NEMA_drawScaledImageBilinear UI_DRAWING_drawScaledImageBilinear_0
#define UI_DRAWING_NEMA_drawScaledImageNearestNeighbor UI_DRAWING_drawScaledImageNearestNeighbor_0
#define UI_DRAWING_NEMA_drawThickLine UI_DRAWING_drawThickLine_0
#define UI_DRAWING_NEMA_drawThickFadedLine UI_DRAWING_drawThickFadedLine_0
#define UI_DRAWING_NEMA_drawThickCircle UI_DRAWING_drawThickCircle_0
#define UI_DRAWING_NEMA_drawThickFadedCircle UI_DRAWING_drawThickFadedCircle_0
/** @endcond */

#endif // !defined(UI_GC_SUPPORTED_FORMATS) || (UI_GC_SUPPORTED_FORMATS <= 1)

// --------------------------------------------------------------------------------
// ui_drawing.h API
// --------------------------------------------------------------------------------

/**
 * @brief Implementation of drawLine over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the first pixel line X coordinate.
 * @param[in] startY the first pixel line Y coordinate.
 * @param[in] endX the last pixel line X coordinate.
 * @param[in] endY the last pixel line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawLine(MICROUI_GraphicsContext *gc, jint startX, jint startY, jint endX, jint endY);

#if defined ENABLE_SIMPLE_LINES && (ENABLE_SIMPLE_LINES == 1)

/**
 * @brief Implementation of drawHorizontalLine over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the first pixel line X coordinate.
 * @param[in] x2 the last pixel line X coordinate.
 * @param[in] y the both pixels line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawHorizontalLine(MICROUI_GraphicsContext *gc, jint x1, jint x2, jint y);

/**
 * @brief Implementation of drawVerticalLine over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the both pixels line X coordinate.
 * @param[in] y1 the first pixel line Y coordinate.
 * @param[in] y2 the last pixel line Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawVerticalLine(MICROUI_GraphicsContext *gc, jint x, jint y1, jint y2);

/**
 * @brief Implementation of drawRectangle over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the top-left pixel X coordinate.
 * @param[in] y1 the top-left pixel Y coordinate.
 * @param[in] x2 the bottom-right pixel X coordinate.
 * @param[in] y2 the bottom-right pixel Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawRectangle(MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2);

#endif // ENABLE_SIMPLE_LINES

/**
 * @brief Implementation of fillRectangle over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the top-left pixel X coordinate.
 * @param[in] y1 the top-left pixel Y coordinate.
 * @param[in] x2 the bottom-right pixel X coordinate.
 * @param[in] y2 the bottom-right pixel Y coordinate.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_fillRectangle(MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2);

/**
 * @brief Implementation of drawRoundedRectangle over NemaGFX. See ui_drawing.h
 *
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
DRAWING_Status UI_DRAWING_NEMA_drawRoundedRectangle(MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                    jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/**
 * @brief Implementation of fillRoundedRectangle over NemaGFX. See ui_drawing.h
 *
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
DRAWING_Status UI_DRAWING_NEMA_fillRoundedRectangle(MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                    jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/**
 * @brief Implementation of drawCircle over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter);

/**
 * @brief Implementation of fillCircle over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the top-left pixel X coordinate.
 * @param[in] y the top-left pixel Y coordinate.
 * @param[in] diameter the circle square size.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_fillCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter);

/**
 * @brief Implementation of drawImage over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to copy.
 * @param[in] regionY the y coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawImage(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint regionX, jint regionY,
                                         jint width, jint height, jint x, jint y, jint alpha);

/**
 * @brief Implementation of copyImage over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to copy.
 * @param[in] regionX the x coordinate of the upper-left corner of the region to copy.
 * @param[in] regionY the y coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x the x coordinate of the top-left point in the destination.
 * @param[in] y the y coordinate of the top-left point in the destination.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_copyImage(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint regionX, jint regionY,
                                         jint width, jint height, jint x, jint y);

/**
 * @brief Implementation of drawRegion over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext source and target.
 * @param[in] x_src the x coordinate of the upper-left corner of the region to copy.
 * @param[in] y_src the y coordinate of the upper-left corner of the region to copy.
 * @param[in] width the width of the region to copy.
 * @param[in] height the height of the region to copy.
 * @param[in] x_dest the x coordinate of the top-left point in the destination.
 * @param[in] y_dest the y coordinate of the top-left point in the destination.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawRegion(MICROUI_GraphicsContext *gc, jint x_src, jint y_src, jint width, jint height,
                                          jint x_dest, jint y_dest, jint alpha);

/**
 * @brief Implementation of drawRotatedImageBilinear over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] rotationX the x coordinate of the rotation center.
 * @param[in] rotationY the y coordinate of the rotation center.
 * @param[in] angle the rotation angle.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawRotatedImageBilinear(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x, jint y,
                                                        jint rotationX, jint rotationY, jfloat angle, jint alpha);

/**
 * @brief Implementation of drawRotatedImageNearestNeighbor over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] rotationX the x coordinate of the rotation center.
 * @param[in] rotationY the y coordinate of the rotation center.
 * @param[in] angle the rotation angle.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x,
                                                               jint y, jint rotationX, jint rotationY, jfloat angle,
                                                               jint alpha);

/**
 * @brief Implementation of drawScaledImageBilinear over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] factorX scaling X factor.
 * @param[in] factorY scaling Y factor.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawScaledImageBilinear(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x, jint y,
                                                       jfloat factorX, jfloat factorY, jint alpha);

/**
 * @brief Implementation of drawScaledImageNearestNeighbor over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] img the MicroUI Image to draw.
 * @param[in] x the x coordinate of the image reference anchor top-left point.
 * @param[in] y the y coordinate of the image reference anchor top-left point.
 * @param[in] factorX scaling X factor.
 * @param[in] factorY scaling Y factor.
 * @param[in] alpha the opacity level to apply to the region.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x,
                                                              jint y, jfloat factorX, jfloat factorY, jint alpha);

/**
 * @brief Implementation of drawThickLine over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x1 the x coordinate of the start of the line
 * @param[in] y1 the y coordinate of the start of the line
 * @param[in] x2 the x coordinate of the end of the line
 * @param[in] y2 the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickLine(MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2,
                                             jint thickness);

/**
 * @brief Implementation of drawThickFadedLine over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] startX the x coordinate of the start of the line
 * @param[in] startY the y coordinate of the start of the line
 * @param[in] endX the x coordinate of the end of the line
 * @param[in] endY the y coordinate of the end of the line
 * @param[in] thickness the line thickness.
 * @param[in] fade the fade to apply.
 * @param[in] startCap cap representation of start of shape
 * @param[in] endCap cap representation of end of shape
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickFadedLine(MICROUI_GraphicsContext *gc, jint startX, jint startY, jint endX,
                                                  jint endY, jint thickness, jint fade, DRAWING_Cap startCap,
                                                  DRAWING_Cap endCap);

/**
 * @brief Implementation of drawThickCircle over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                               jint thickness);

/**
 * @brief Implementation of drawThickFadedCircle over NemaGFX. See ui_drawing.h
 *
 * @param[in] gc the MicroUI GraphicsContext target.
 * @param[in] x the x coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] y the y coordinate of the upper-left corner of the square where the circle is drawn
 * @param[in] diameter the diameter of the circle to draw
 * @param[in] thickness the circle thickness.
 * @param[in] fade the fade to apply.
 *
 * @return the drawing status.
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickFadedCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                                    jint thickness, jint fade);

// -----------------------------------------------------------------------------
// EOF
// -----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif // !defined UI_DRAWING_NEMA_H
