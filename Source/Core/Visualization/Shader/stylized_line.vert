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

VertOut vec4 diffuse;
VertOut vec3 up_vec;
VertOut vec3 side_vec;
VertOut vec3 coord;
VertOut vec2 depth0;
VertOut vec2 depth1;

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewMatrix; // model-view matrix
uniform mat4 ProjectionMatrix; // projection matrix
uniform mat3 NormalMatrix; // normal matrix


/*===========================================================================*/
/**
 *  @brief  Main function of vertex shader.
 */
/*===========================================================================*/
void main()
{
    vec4 p = ModelViewMatrix * gl_Vertex; // vertex in world coordinate
    vec3 v = normalize( -p.xyz ); // vector from camera to the vertex
    vec3 t = NormalMatrix * gl_Normal; // tangent in world coordinate

    position = p.xyz;

    // Size and up extrusion vectors.
    side_vec = normalize( cross( -p.xyz, t ) );
    up_vec = normalize( cross( side_vec, t ) );

    // Extrude the vertex.
    p.xyz += side_vec * gl_MultiTexCoord0.x;
    gl_Position = ProjectionMatrix * p;

    diffuse = gl_Color;
    coord = gl_MultiTexCoord0.xzy;
    depth0 = gl_Position.zw;

    float depth = min( 1.0 / dot( up_vec, -v ), 1.0) * abs( gl_MultiTexCoord0.x );
    vec4 mp = ProjectionMatrix * vec4( p.xyz + v * depth, 1.0 );

    // transfer front position z and w values to fragment shader
    depth1 = mp.zw;

    // modify upvec in tangent direction
    up_vec += t * gl_MultiTexCoord0.w;
}
