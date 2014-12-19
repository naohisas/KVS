/*****************************************************************************/
/**
 *  @file   SR_point.vert
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
#include "qualifire.h"


// Input parameter from OpenGL.
VertIn vec2 random_index; // index for accessing to the random texture

// Output parameters to fragment shader.
VertOut vec3 position; // vertex position in camera coordinate
VertOut vec3 normal; // normal vector in camera coodinate
VertOut vec2 index; // index for accessing to the random texture

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewMatrix; // model-view matrix
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix
uniform mat3 NormalMatrix; // normal matrix


/*===========================================================================*/
/**
 *  @brief  Main function of vertex shader.
 */
/*===========================================================================*/
void main()
{
    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;

    position = ( ModelViewMatrix * gl_Vertex ).xyz;
    normal = NormalMatrix * gl_Normal;
    index = random_index;
}
