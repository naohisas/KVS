/*****************************************************************************/
/**
 *  @file   tetrahedra.geom
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
//#extension GL_EXT_geometry_shader4 : enable
//#extension GL_EXT_gpu_shader4 : enable
#include <qualifire.h>
#include <texture.h>


// Input variables from vertex shader.
GeomIn vec4 position_in[4]; // vertex positions in camera coordinate
GeomIn vec4 position_ndc_in[4]; // vertex positions in normalized device coordinate (nd_in)
GeomIn vec3 normal_in[4]; // normal vectors in camera coordinate
GeomIn float value_in[4]; // scalar values for the vertex (value_in)
GeomIn vec2 random_index_in[4]; // indices for accessing to the random texture (id_in)

// Output variables to fragment shader.
GeomOutWithNoperspective vec3 position; // 3D vertex position in camera coordinate
GeomOutWithNoperspective vec3 normal; // normal vector in camera coordinate
GeomOutWithNoperspective vec2 random_index; // index for accessing to the random texture
GeomOutWithNoperspective vec2 position_ndc; // 2D vertex position in normalized device coordinate
GeomOutWithNoperspective float scalar_front; // scalar value on the front face
GeomOutWithNoperspective float scalar_back; // scalar value on the back face
GeomOutWithNoperspective float depth_front; // depth value at the front face
GeomOutWithNoperspective float depth_back; // depth value at the back face
GeomOutWithNoperspective float wc_inv_front; // reciprocal value of the w-component at the front face in clip coordinate
GeomOutWithNoperspective float wc_inv_back; // reciprocal value of the w-component at the back face in clip coordinate

// Uniform variables.
uniform sampler2D decomposition_texture; // decomposition texture


// Returns random index.
#define RANDOM_INDEX( index )                                           \
    index[0] * 3.0 + index[1] * 5.0 + index[2] * 7.0 + index[3] * 11.0

// Returns true if the point in clip coordinate is out of the front clipping face.
#define OUT_OF_RANGE_IN_CLP( clip )                                    \
    ( clip[0].w <= 0 || clip[1].w <= 0 || clip[2].w <= 0 || clip[3].w <= 0 )

// Returns true if the point in NDC is inside the view volume.
#define OUT_OF_RANGE_IN_NDC( ndc )                                      \
    ( ( ndc[0].x < -1.0 && ndc[1].x < -1.0 && ndc[2].x < -1.0 && ndc[3].x < -1.0 ) || \
      ( ndc[0].x >  1.0 && ndc[1].x >  1.0 && ndc[2].x >  1.0 && ndc[3].x >  1.0 ) || \
      ( ndc[0].y < -1.0 && ndc[1].y < -1.0 && ndc[2].y < -1.0 && ndc[3].y < -1.0 ) || \
      ( ndc[0].y >  1.0 && ndc[1].y >  1.0 && ndc[2].y >  1.0 && ndc[3].y >  1.0 )  )

/*===========================================================================*/
/**
 *  @brief  Returns cross product of the specified 2d vectors.
 *  @param  v0 [in] 2d vector 0
 *  @param  v1 [in] 2d vector 1
 *  @return cross product
 */
/*===========================================================================*/
float Cross( in vec2 v0, in vec2 v1 )
{
    return v0.x * v1.y - v0.y * v1.x;
}

/*===========================================================================*/
/**
 *  @brief  Returns intersection ratios for each line segment.
 *  @param  v0 [in] 2d vector 1
 *  @param  v1 [in] 2d vector 2
 *  @param  v [in] base 2d vector
 *  @return intersection ratios as 2d vector
 */
/*===========================================================================*/
vec2 IntersectionRatio( in vec2 v0, in vec2 v1, in vec2 v )
{
    float temp = 1.0 / Cross( v0, v1 );
    float t0 = Cross( v, v1 ) * temp;
    float t1 = Cross( v, v0 ) * temp;
    return abs( vec2( t0, t1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns direction.
 *  @param  p0 [in] index of vertex 0
 *  @param  p1 [in] index of vertex 1
 *  @param  p2 [in] index of vertex 2
 *  @return direction
 */
/*===========================================================================*/
int Direction( in int p0, in int p1, in int p2 )
{
    vec2 v1 = position_ndc_in[p1].xy - position_ndc_in[p0].xy;
    vec2 v2 = position_ndc_in[p2].xy - position_ndc_in[p0].xy;
    return int( sign( Cross( v1, v2 ) ) + 1.0 );
}

/*===========================================================================*/
/**
 *  @brief  Emits point specified by the given index with distance.
 *  @param  index [in] index of the point
 */
/*===========================================================================*/
void EmitOriginalPoint( const in int index )
{
    gl_Position = vec4( position_ndc_in[index].xyz, 1.0 );

    position = position_in[index].xyz;
    normal = normal_in[index].xyz;
    position_ndc = position_ndc_in[index].xy;

    scalar_front = value_in[index];
    scalar_back  = value_in[index];

    depth_front = position_ndc_in[index].z;
    depth_back = position_ndc_in[index].z;

    wc_inv_front = position_ndc_in[index].w;
    wc_inv_back  = position_ndc_in[index].w;

    EmitVertex();
}

/*===========================================================================*/
/**
 *  @brief  Emits crossing point.
 *  @param  crossing_position [in] 3D position in camera coordinate of the point
 *  @param  crossing_normal [in] normal vector of the point
 *  @param  crossing_position_ndc [in] 2D position in normalized device coordinate of the point
 *  @param  coord_front [in] coordinate value of the point on the front face
 *  @param  coord_back [in] coordinate value of the point on the back face
 *  @param  value_front [in] scalar value of the point on the front face
 *  @param  value_back [in] scalar value of the point on the back face
 */
/*===========================================================================*/
void EmitCrossingPoint(
    const in vec3 crossing_position,
    const in vec3 crossing_normal,
    const in vec2 crossing_position_ndc,
    const in vec2 coord_front,
    const in vec2 coord_back,
    const in float value_front,
    const in float value_back )
{
    gl_Position = vec4( crossing_position_ndc, 0.0, 1.0 );

    position = crossing_position;
    normal = crossing_normal;
    position_ndc = crossing_position_ndc;

    scalar_front = value_front;
    scalar_back  = value_back;

    depth_front = coord_front.x;
    depth_back  = coord_back.x;

    wc_inv_front = coord_front.y;
    wc_inv_back = coord_back.y;

    EmitVertex();
}

/*===========================================================================*/
/**
 *  @brief  Decomposes a tetrahedral cell into three triangles.
 *  @param  p0 [in] index of the vertex 0
 *  @param  p1 [in] index of the vertex 1
 *  @param  p2 [in] index of the vertex 2
 *  @param  p3 [in] index of the vertex 3
 *
 *  Case 1:
 *
 *  p3-------------p2
 *    \ ~-_   _-~ /
 *     \   ~p0   /
 *      \   |   /
 *       \  |  /
 *        \ | /
 *         \|/
 *         p1
 */
/*===========================================================================*/
void DecomposeInCase1( in int p0, in int p1, in int p2, in int p3 )
{
    EmitOriginalPoint( p1 );
    EmitOriginalPoint( p2 );

    // Parameters for interpolation.
    float S  = abs( Cross( position_ndc_in[p2].xy - position_ndc_in[p1].xy, position_ndc_in[p3].xy - position_ndc_in[p1].xy ) );
    float S1 = abs( Cross( position_ndc_in[p2].xy - position_ndc_in[p0].xy, position_ndc_in[p3].xy - position_ndc_in[p0].xy ) );
    float S2 = abs( Cross( position_ndc_in[p1].xy - position_ndc_in[p0].xy, position_ndc_in[p3].xy - position_ndc_in[p0].xy ) );
    float t1 = S1 / S;
    float t2 = S2 / S;
    float t3 = 1.0 - ( t1 + t2 );
    vec3 t = vec3( t1, t2, t3 );

    vec3 px = vec3( position_in[p1].x, position_in[p2].x, position_in[p3].x );
    vec3 py = vec3( position_in[p1].y, position_in[p2].y, position_in[p3].y );
    vec3 pz = vec3( position_in[p1].z, position_in[p2].z, position_in[p3].z );
    vec3 pw = vec3( position_in[p1].w, position_in[p2].w, position_in[p3].w );
    vec3 nx = vec3( normal_in[p1].x, normal_in[p2].x, normal_in[p3].x );
    vec3 ny = vec3( normal_in[p1].y, normal_in[p2].y, normal_in[p3].y );
    vec3 nz = vec3( normal_in[p1].z, normal_in[p2].z, normal_in[p3].z );
    vec3 v = vec3( value_in[p1], value_in[p2], value_in[p3] );
    vec3 z = vec3( position_ndc_in[p1].z, position_ndc_in[p2].z, position_ndc_in[p3].z );
    vec3 w = vec3( position_ndc_in[p1].w, position_ndc_in[p2].w, position_ndc_in[p3].w );

    vec4 center_p = vec4( dot( px, t ), dot( py, t ), dot( pz, t ), dot( pw, t ) );
    vec3 center_n = vec3( dot( nx, t ), dot( ny, t ), dot( nz, t ) );
    float center_v = dot( v, t );
    float center_z = dot( z, t );
    float center_w = dot( w, t );

    // In case that the vertex specified by p0 is behind the center point.
    if ( center_z < position_ndc_in[p0].z )
    {
        position = center_p.xyz;
        normal = center_n;

        scalar_front = center_v;
        depth_front = center_z;
        wc_inv_front = center_w;

        scalar_back = value_in[p0];
        depth_back = position_ndc_in[p0].z;
        wc_inv_back = position_ndc_in[p0].w;
    }
    // In case that the center point is behind the vertex specified by p0.
    else
    {
        position = position_in[p0].xyz;
        normal = normal_in[p0];

        scalar_front = value_in[p0];
        depth_front = position_ndc_in[p0].z;
        wc_inv_front = position_ndc_in[p0].w;

        scalar_back = center_v;
        depth_back = center_z;
        wc_inv_back = center_w;
    }

    gl_Position = vec4( position_ndc_in[p0].xyz, 1.0 );
    position_ndc = position_ndc_in[p0].xy;

    EmitVertex();

    EmitOriginalPoint( p3 );
    EmitOriginalPoint( p1 );
    EndPrimitive();
}

/*===========================================================================*/
/**
 *  @brief  Decomposes a tetrahedral cell into four triangles.
 *  @param  p0 [in] index of the vertex 0
 *  @param  p1 [in] index of the vertex 1
 *  @param  p2 [in] index of the vertex 2
 *  @param  p3 [in] index of the vertex 3
 *
 *  Case 2:
 *
 *           p0
 *        _-~ | ~-_
 *     _-~    |    ~-_ 
 *   p3 -  -  | -  -  p2
 *     \      |      /
 *       \    |    /
 *         \  |  /
 *           \|/
 *            p1
 */
/*===========================================================================*/
void DecomposeInCase2( in int p0, in int p1, in int p2, in int p3 )
{
    // Parameters for interpolation.
    vec2 t = IntersectionRatio( position_ndc_in[p1].xy - position_ndc_in[p0].xy,
                                position_ndc_in[p3].xy - position_ndc_in[p2].xy,
                                position_ndc_in[p2].xy - position_ndc_in[p0].xy );

    vec4 p01 = mix( position_in[p0], position_in[p1], t.x );
    vec4 p23 = mix( position_in[p2], position_in[p3], t.y );
    vec3 n01 = mix( normal_in[p0], normal_in[p1], t.x );
    vec3 n23 = mix( normal_in[p2], normal_in[p3], t.y );
    vec2 p01_ndc = mix( position_ndc_in[p0].zw, position_ndc_in[p1].zw, t.x );
    vec2 p23_ndc = mix( position_ndc_in[p2].zw, position_ndc_in[p3].zw, t.y );
    float v01 = mix( value_in[p0], value_in[p1], t.x );
    float v23 = mix( value_in[p2], value_in[p3], t.y );

    vec2 crossing_position_ndc = mix( position_ndc_in[p0].xy, position_ndc_in[p1].xy, t.x );

    vec3 crossing_position;
    vec3 crossing_normal;

    vec2 center_coord_front;
    vec2 center_coord_back;
    float center_value_front;
    float center_value_back;

    // In case that the vertex p23 is behind the vertex p01.
    if ( p01_ndc.x <= p23_ndc.x )
    {
        crossing_position = p01.xyz;
        crossing_normal = n01;

        center_coord_front = p01_ndc;
        center_value_front = v01;

        center_coord_back = p23_ndc;
        center_value_back = v23;
    }
    // In case that the vertex p01 is behind the vertex p23.
    else
    {
        crossing_position = p23.xyz;
        crossing_normal = n23;

        center_coord_front = p23_ndc;
        center_value_front = v23;

        center_coord_back = p01_ndc;
        center_value_back = v01;
    }

    // right half: p0-p2-C, p2-C-p1
    EmitOriginalPoint( p0 );
    EmitOriginalPoint( p2 );
    EmitCrossingPoint(
        crossing_position,
        crossing_normal,
        crossing_position_ndc,
        center_coord_front,
        center_coord_back,
        center_value_front,
        center_value_back );
    EmitOriginalPoint( p1 );
    EndPrimitive();

    // left half: p0-p3-C, p3-C-p1
    EmitOriginalPoint( p0 );
    EmitOriginalPoint( p3 );
    EmitCrossingPoint(
        crossing_position,
        crossing_normal,
        crossing_position_ndc,
        center_coord_front,
        center_coord_back,
        center_value_front,
        center_value_back );
    EmitOriginalPoint( p1 );
    EndPrimitive();
}

/*===========================================================================*/
/**
 *  @brief  Main function of geometry shader.
 */
/*===========================================================================*/
void main()
{
    if ( OUT_OF_RANGE_IN_CLP( gl_PositionIn ) ) return;
    if ( OUT_OF_RANGE_IN_NDC( position_ndc_in ) ) return;

    random_index = RANDOM_INDEX( random_index_in );

    int d321 = Direction( 3, 2, 1 );
    int d230 = Direction( 2, 3, 0 );
    int d103 = Direction( 1, 0, 3 );
    int d012 = Direction( 0, 1, 2 );

    int p = d321 * 27 + d230 * 9 + d103 * 3 + d012;
    vec2 t = vec2( ( float(p) + 0.5 ) / 81.0, 0.5 );
    vec4 info = LookupTexture2D( decomposition_texture, t );
    int type = int( ( info.a * 255.0 + 16.0 ) / 32.0 );
    int p0 = int( ( info.x * 255.0 + 16.0 ) / 32.0 );
    int p1 = int( ( info.y * 255.0 + 16.0 ) / 32.0 );
    int p2 = int( ( info.z * 255.0 + 16.0 ) / 32.0 );
    int p3 = 6 - ( p0 + p1 + p2 );

    if ( type == 1 )
    {
        DecomposeInCase1( p0, p1, p2, p3 );
    }
    else
    {
        DecomposeInCase2( p0, p1, p2, p3 );
    }
}
