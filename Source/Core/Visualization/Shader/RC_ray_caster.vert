/*****************************************************************************/
/**
 *  @file   RC_ray_caster.vert
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


// Output parameters.
VertOut vec3 position_ndc; // vertex position in normalized device coordinate

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix


/*===========================================================================*/
/**
 *  @brief  Main function of vertex shader.
 */
/*===========================================================================*/
void main()
{
//    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;

    position_ndc = gl_Position.xyz / gl_Position.w;
}
