/*****************************************************************************/
/**
 *  @file   tetrahedra.vert
 *  @author Naoya Maeda, Naohisa Sakamoto
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
#include <qualifire.h>


// Input variables from OpenGL.
VertIn vec2 random_index; // index for accessing to the random texture
VertIn float value; // normalized scalar value for the vertex

// Output variables to geometry shader.
VertOut vec4 position_in; // vertex position in camera coordinate
VertOut vec4 position_ndc_in; // vertex position in normalized device coordinate
VertOut vec3 normal_in; // normal vector in camera coordinate
VertOut float value_in; // scalar value for the vertex
VertOut vec2 random_index_in; // index for accessing to the random texture

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

    position_in = ModelViewMatrix * gl_Vertex;
    normal_in = NormalMatrix * gl_Normal;
    position_ndc_in = vec4( gl_Position.xyz, 1.0 ) / gl_Position.w;

    value_in = value / gl_Position.w;
    random_index_in = random_index;
}
