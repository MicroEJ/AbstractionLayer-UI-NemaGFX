/*
 * C
 *
 * Copyright 2023-2024 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 */

#if !defined UI_DRAWING_NEMA_H
#define UI_DRAWING_NEMA_H

#if defined __cplusplus
extern "C" {
#endif

/*
 * @file
 * @brief Implementation of a set of ui_drawing.h drawing functions (MicroUI library). These are
 * implementations over the NemaGFX and the destination buffer format is NEMA_RGB565. When the
 * drawing cannot be performed by the GPU, the software implementation is used insted.
 *
 * @author MicroEJ Developer Team
 * @version 3.0.0
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

#if !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_NEMA_xxx() are directly called by LLUI_PAINTER_impl.c
 * and LLDW_PAINTER_impl.c. This file overrides each function independently to use the
 * NemaGFX.
 */

#define UI_DRAWING_NEMA_drawLine UI_DRAWING_drawLine

#ifdef ENABLE_SIMPLE_LINES

#define UI_DRAWING_NEMA_drawHorizontalLine UI_DRAWING_drawHorizontalLine
#define UI_DRAWING_NEMA_drawVerticalLine UI_DRAWING_drawVerticalLine
#define UI_DRAWING_NEMA_drawRectangle UI_DRAWING_drawRectangle

#endif // ENABLE_SIMPLE_LINES

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

#else // !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

/*
 * The functions UI_DRAWING_NEMA_xxx() are indirectly called through some tables.
 * The functions have got the identifier "0" as suffix. This file overrides each function
 * independently to use the NemaGFX.
 */

#define UI_DRAWING_NEMA_drawLine UI_DRAWING_drawLine_0

#ifdef ENABLE_SIMPLE_LINES

#define UI_DRAWING_NEMA_drawHorizontalLine UI_DRAWING_drawHorizontalLine_0
#define UI_DRAWING_NEMA_drawVerticalLine UI_DRAWING_drawVerticalLine_0
#define UI_DRAWING_NEMA_drawRectangle UI_DRAWING_drawRectangle_0

#endif // ENABLE_SIMPLE_LINES

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

#endif // !defined(LLUI_GC_SUPPORTED_FORMATS) || (LLUI_GC_SUPPORTED_FORMATS <= 1)

// --------------------------------------------------------------------------------
// ui_drawing.h API
// --------------------------------------------------------------------------------

/*
 * @brief Implementation of drawLine over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawLine(MICROUI_GraphicsContext *gc, jint startX, jint startY, jint endX, jint endY);

#ifdef ENABLE_SIMPLE_LINES

/*
 * @brief Implementation of drawHorizontalLine over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawHorizontalLine(MICROUI_GraphicsContext *gc, jint x1, jint x2, jint y);

/*
 * @brief Implementation of drawVerticalLine over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawVerticalLine(MICROUI_GraphicsContext *gc, jint x, jint y1, jint y2);

/*
 * @brief Implementation of drawRectangle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawRectangle(MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2);

#endif // ENABLE_SIMPLE_LINES

/*
 * @brief Implementation of fillRectangle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_fillRectangle(MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2);

/*
 * @brief Implementation of drawRoundedRectangle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawRoundedRectangle(MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                    jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Implementation of fillRoundedRectangle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_fillRoundedRectangle(MICROUI_GraphicsContext *gc, jint x, jint y, jint width,
                                                    jint height, jint cornerEllipseWidth, jint cornerEllipseHeight);

/*
 * @brief Implementation of drawCircle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter);

/*
 * @brief Implementation of fillCircle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_fillCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter);

/*
 * @brief Implementation of drawImage over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawImage(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint regionX, jint regionY,
                                         jint width, jint height, jint x, jint y, jint alpha);

/*
 * @brief Implementation of copyImage over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_copyImage(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint regionX, jint regionY,
                                         jint width, jint height, jint x, jint y);

/*
 * @brief Implementation of drawRegion over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawRegion(MICROUI_GraphicsContext *gc, jint x_src, jint y_src, jint width, jint height,
                                          jint x_dest, jint y_dest, jint alpha);

/*
 * @brief Implementation of drawRotatedImageBilinear over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawRotatedImageBilinear(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x, jint y,
                                                        jint rotationX, jint rotationY, jfloat angle, jint alpha);

/*
 * @brief Implementation of drawRotatedImageNearestNeighbor over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawRotatedImageNearestNeighbor(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x,
                                                               jint y, jint rotationX, jint rotationY, jfloat angle,
                                                               jint alpha);

/*
 * @brief Implementation of drawScaledImageBilinear over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawScaledImageBilinear(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x, jint y,
                                                       jfloat factorX, jfloat factorY, jint alpha);

/*
 * @brief Implementation of drawScaledImageNearestNeighbor over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawScaledImageNearestNeighbor(MICROUI_GraphicsContext *gc, MICROUI_Image *img, jint x,
                                                              jint y, jfloat factorX, jfloat factorY, jint alpha);

/*
 * @brief Implementation of drawThickLine( over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickLine(MICROUI_GraphicsContext *gc, jint x1, jint y1, jint x2, jint y2,
                                             jint thickness);

/*
 * @brief Implementation of drawThickFadedLine over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickFadedLine(MICROUI_GraphicsContext *gc, jint startX, jint startY, jint endX,
                                                  jint endY, jint thickness, jint fade, DRAWING_Cap startCap,
                                                  DRAWING_Cap endCap);

/*
 * @brief Implementation of drawThickCircle over NemaGFX. See ui_drawing.h
 */
DRAWING_Status UI_DRAWING_NEMA_drawThickCircle(MICROUI_GraphicsContext *gc, jint x, jint y, jint diameter,
                                               jint thickness);

/*
 * @brief Implementation of drawThickFadedCircle over NemaGFX. See ui_drawing.h
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
