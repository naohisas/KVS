/*****************************************************************************/
/**
 *  @file   multivariate_tetrahedra.vert
 *  @author Jun Nishimura
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
varying vec4 position_in;
varying vec2 random_index_in;
varying vec2 value_in;
varying vec3 normal_in;

attribute vec2 random_index;
attribute vec2 value;

void main()
{
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    position_in = gl_ModelViewMatrix * gl_Vertex;
    random_index_in = random_index;
    value_in = value;
    normal_in = gl_NormalMatrix * gl_Normal;
}
