/*****************************************************************************/
/**
 *  @file   shader.vert
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: shading.vert 476 2010-02-14 15:45:46Z naohisa.sakamoto $
 */
/*****************************************************************************/
#version 120
#include "qualifire.h"

// Output parameters to fragment shader.
VertOut vec3 position;
VertOut vec3 normal;
VertOut vec2 center;

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewMatrix; // model-view matrix
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix
uniform mat3 NormalMatrix; // normal matrix

uniform float screen_width;
uniform float screen_height;

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

    vec2 screen_center = vec2( screen_width * 0.5, screen_height * 0.5 );
    center = screen_center + ( gl_Position.xy / gl_Position.w ) * screen_center;
}
