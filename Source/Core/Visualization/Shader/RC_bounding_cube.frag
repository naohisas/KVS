/*****************************************************************************/
/**
 *  @file   RC_bounding_cube.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#version 120


/*===========================================================================*/
/**
 *  @brief  Main function of fragment shader.
 */
/*===========================================================================*/
void main()
{
    // Store the depth value in window coordinates to the alpha channel
    // of color buffer.
    gl_FragColor.rgb = gl_Color.rgb;
    gl_FragColor.a = gl_FragCoord.z;
}
