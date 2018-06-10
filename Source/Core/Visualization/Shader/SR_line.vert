/*****************************************************************************/
/**
 *  @file   SR_line.vert
 *  @author Jun Nishimura, Naohisa Sakamoto
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
#include "qualifire.h"


// Input parameters.
VertIn vec2 random_index; // index for accessing to the random texture

// Output parameters to fragment shader.
VertOut vec2 index; // index for accessing to the random texture

// Uniform parameter.
uniform float line_offset; // line offset in clip coordinate

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix


/*===========================================================================*/
/**
 *  @brief  Main function of vertex shader.
 */
/*===========================================================================*/
void main()
{
    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
    gl_Position.z -= line_offset;
    gl_FrontColor = gl_Color;

    index = random_index;
}
