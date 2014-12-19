/*****************************************************************************/
/**
 *  @file   resize.vert
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: resize.vert 260 2009-07-07 04:39:41Z naohisa.sakamoto $
 */
/*****************************************************************************/

/*===========================================================================*/
/**
 *  @brief  Resize texture.
 */
/*===========================================================================*/
void main()
{
    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
