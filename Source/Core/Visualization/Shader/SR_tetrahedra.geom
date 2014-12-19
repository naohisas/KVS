/*****************************************************************************/
/**
 *  @file   tetrahedra.geom
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
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable
#include "qualifire.h"
#include "texture.h"


// Input parameters from vertex shader.
GeomIn vec4 position_in[4]; // vertex positions in camera coordinate
GeomIn vec4 position_ndc_in[4]; // vertex positions in normalized device coordinate
GeomIn vec3 normal_in[4]; // normal vectors in camera coordinate
GeomIn float value_in[4]; // scalar values for the vertex
GeomIn vec2 random_index_in[4]; // indices for accessing to the random texture

// Output parameters to fragment shader.
GeomOut vec3 position; // vertex position in camera coordinate
GeomOut vec3 normal; // normal vector in camera coordinate
GeomOut vec2 random_index; // index for accessing to the random texture
GeomOut float scalar_front; // scalar value on the front face
GeomOut float scalar_back; // scalar value on the back face
GeomOut float segment_length; // distance between the front and back face
GeomOut float wc_inv_front; // reciprocal value of the w-component at the front face in clip coordinate
GeomOut float wc_inv_back; // reciprocal value of the w-component at the back face in clip coordinate
#if defined( ENABLE_EXACT_DEPTH_TESTING )
GeomOut float depth_front; // depth value at the front face
GeomOut float depth_back; // depth value at the back face
#endif

// Uniform parameters.
uniform vec2 preintegration_scale_offset; // offset values for pre-integration table
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
 *  @brief  Returns distance for accessing pre-integration table.
 *  @param  dist [in] distance in camera coordinate
 *  @return distance
 */
/*===========================================================================*/
float Distance( const in float dist )
{
    return dist * preintegration_scale_offset.x + preintegration_scale_offset.y;
}

/*===========================================================================*/
/**
 *  @brief  Emits point specified by the given index with distance.
 *  @param  index [in] index of the point
 *  @param  dist [in] distance for the point
 */
/*===========================================================================*/
void EmitOriginalPoint( const in int index, const in float dist )
{
    gl_Position = gl_PositionIn[index];

    // Vertex position and normal vector in camera coodinate
    position = position_in[index].xyz;
    normal = normal_in[index].xyz;

    // Indices for accessing the pre-integration table
    scalar_front = value_in[index];
    scalar_back  = value_in[index];
    segment_length = dist;

    // Depth values in normalized device coordinate
#if defined( ENABLE_EXACT_DEPTH_TESTING )
    depth_front = gl_PositionIn[index].z / gl_PositionIn[index].w;
    depth_back  = depth_front;
#endif

    wc_inv_front = position_ndc_in[index].w;
    wc_inv_back  = position_ndc_in[index].w;

    EmitVertex();
}

/*===========================================================================*/
/**
 *  @brief  Emits crossing point.
 *  @param  c_gl_Position [in] position of the point with w-component
 *  @param  c_position [in] position of the point
 *  @param  c_normal [in] normal vector of the point
 *  @param  c_scalar_front [in] scalar value of the point on the front face
 *  @param  c_scalar_back [in] scalar value of the point on the back face
 *  @param  c_dist [in] distance between the front and back face
 *  @param  c_depth [in] depth value of the point
 */
/*===========================================================================*/
void EmitCrossingPoint(
    const in vec4 c_gl_Position,
    const in vec3 c_position,
    const in vec3 c_normal,
    const in float c_scalar_front,
    const in float c_scalar_back,
    const in float c_dist,
    const in float c_depth,
    const in float c_wc_inv_front,
    const in float c_wc_inv_back
    )
{
    gl_Position = c_gl_Position;

    // Vertex position and normal vector in camera coodinate
    position = c_position;
    normal = c_normal;

    // Indices for accessing the pre-integration table
    scalar_front = c_scalar_front;
    scalar_back  = c_scalar_back;
    segment_length = c_dist;

    // Depth values in normalized device coordinate
#if defined( ENABLE_EXACT_DEPTH_TESTING )
    depth_front = c_gl_Position.z / c_gl_Position.w;
    depth_back  = c_depth;
#endif

    wc_inv_front = c_wc_inv_front;
    wc_inv_back = c_wc_inv_back;

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
    vec3 v  = vec3( value_in[p1], value_in[p2], value_in[p3] );
    vec3 w  = vec3( position_ndc_in[p1].w, position_ndc_in[p2].w, position_ndc_in[p3].w );

    // Calculate position (p123), normal vector (n123) and scalar value (s123)
    // at the center point in camera coordinate.
    vec4 p123 = vec4( dot( px, t ), dot( py, t ), dot( pz, t ), dot( pw, t ) );
    vec3 n123 = vec3( dot( nx, t ), dot( ny, t ), dot( nz, t ) );
    float s123 = dot( v, t );
    float w123 = dot( w, t );

    // Parameters at the center point for passing to fragment shader.
    vec4 c_gl_Position;
    vec3 c_position;
    vec3 c_normal;
    float c_scalar_front;
    float c_scalar_back;
    float c_segment_length = Distance( length( position_in[p0] - p123 ) );
    float c_depth = 0.0;
    float c_wc_inv_front;
    float c_wc_inv_back;

    // In case that the vertex specified by p0 is behind the center point.
    if ( length( p123.z ) < length( position_in[p0].z ) )
    {
        c_gl_Position = gl_ProjectionMatrix * p123; // in clip coordinate
        c_position = p123.xyz; // in camera coordinate
        c_normal = n123.xyz; // in camera coordinate

        c_scalar_front = s123;
        c_scalar_back = value_in[p0];

#if defined( ENABLE_EXACT_DEPTH_TESTING )
        vec4 tmp = gl_PositionIn[p0];
        c_depth = tmp.z / tmp.w; // in normalized device coordinate
#endif

        c_wc_inv_front = w123;
        c_wc_inv_back = position_ndc_in[p0].w;
    }
    // In case that the center point is behind the vertex specified by p0.
    else
    {
        c_gl_Position = gl_PositionIn[p0]; // in camera coordinate
        c_position = position_in[p0].xyz; // in camera coordinate
        c_normal = normal_in[p0].xyz; // in camera coordinate

        c_scalar_front = value_in[p0];
        c_scalar_back = s123;

#if defined( ENABLE_EXACT_DEPTH_TESTING )
        vec4 tmp = gl_ProjectionMatrix * p123;
        c_depth = tmp.z / tmp.w; // in normalized device coordinate
#endif

        c_wc_inv_front = position_ndc_in[p0].w;
        c_wc_inv_back = w123;
    }

    // p1-p2-C p2-C-p3 C-p3-p1
    EmitOriginalPoint( p1, 0.0 );
    EmitOriginalPoint( p2, 0.0 );
    EmitCrossingPoint(
        c_gl_Position,
        c_position,
        c_normal,
        c_scalar_front,
        c_scalar_back,
        c_segment_length,
        c_depth,
        c_wc_inv_front,
        c_wc_inv_back );
    EmitOriginalPoint( p3, 0.0 );
    EmitOriginalPoint( p1, 0.0 );

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
    vec2 t01 = vec2( 1.0 - t.x, t.x );
    vec2 t23 = vec2( 1.0 - t.y, t.y );

    // Calculate positions (p01, p23), normal vectors (n01, n23), and scalar
    // values (s01, s23) at the crossing point in camera coordinate. Here,
    // (p01, n01, s01) and (p23, n23, s23) are defined on the line p0-p1 and
    // p2-p3, respectively.
    vec4  p01 = t01.x * position_in[p0] + t01.y * position_in[p1];
    vec3  n01 = t01.x * normal_in[p0] + t01.y * normal_in[p1];
    float s01 = t01.x * value_in[p0] + t01.y * value_in[p1];
    float w01 = t01.x * position_ndc_in[p0].w + t01.y * position_ndc_in[p1].w;

    vec4  p23 = t23.x * position_in[p2] + t23.y * position_in[p3];
    vec3  n23 = t23.x * normal_in[p2] + t23.y * normal_in[p3];
    float s23 = t23.x * value_in[p2] + t23.y * value_in[p3];
    float w23 = t23.x * position_ndc_in[p2].w + t23.y * position_ndc_in[p3].w;

    // Parameters at the center point for passing to fragment shader.
    vec4 c_gl_Position;
    vec3 c_position;
    vec3 c_normal;
    float c_scalar_front;
    float c_scalar_back;
    float c_segment_length = Distance( length( p01.z - p23.z ) );
    float c_depth = 0.0;
    float c_wc_inv_front;
    float c_wc_inv_back;

    // In case that the vertex p23 is behind the vertex p01.
    if ( length( p01.xyz ) < length( p23.xyz ) )
    {
        c_gl_Position = gl_ProjectionMatrix * p01;
        c_position = p01.xyz;
        c_normal = n01.xyz;
        c_scalar_front = s01;
        c_scalar_back = s23;

#if defined( ENABLE_EXACT_DEPTH_TESTING )
        vec4 tmp = gl_ProjectionMatrix * p23;
        c_depth = tmp.z / tmp.w; // in normalized device coordinate
#endif

        c_wc_inv_front = w01;
        c_wc_inv_back = w23;
    }
    // In case that the vertex p01 is behind the vertex p23.
    else
    {
        c_gl_Position = gl_ProjectionMatrix * p23;
        c_position = p23.xyz;
        c_normal = n23.xyz;
        c_scalar_front = s23;
        c_scalar_back = s01;

#if defined( ENABLE_EXACT_DEPTH_TESTING )
        vec4 tmp = gl_ProjectionMatrix * p01;
        c_depth = tmp.z / tmp.w; // in normalized decive coordinate
#endif

        c_wc_inv_front = w23;
        c_wc_inv_back = w01;
    }

    // right half: p0-p2-C, p2-C-p1
    EmitOriginalPoint( p0, 0.0 );
    EmitOriginalPoint( p2, 0.0 );
    EmitCrossingPoint(
        c_gl_Position,
        c_position,
        c_normal,
        c_scalar_front,
        c_scalar_back,
        c_segment_length,
        c_depth,
        c_wc_inv_front,
        c_wc_inv_back );
    EmitOriginalPoint( p1, 0.0 );
    EndPrimitive();

    // left half: p0-p3-C, p3-C-p1
    EmitOriginalPoint( p0, 0.0 );
    EmitOriginalPoint( p3, 0.0 );
    EmitCrossingPoint(
        c_gl_Position,
        c_position,
        c_normal,
        c_scalar_front,
        c_scalar_back,
        c_segment_length,
        c_depth,
        c_wc_inv_front,
        c_wc_inv_back );
    EmitOriginalPoint( p1, 0.0 );
    EndPrimitive();
}

/*===========================================================================*/
/**
 *  @brief  Decomposes a tetrahedral cell int two triangles.
 *  @param  p0 [in] index of the vertex 0
 *  @param  p1 [in] index of the vertex 1
 *  @param  p2 [in] index of the vertex 2
 *  @param  p3 [in] index of the vertex 3
 *
 *  Case 3: This is a special case of case 2.
 *
 *           p2
 *        _-~ |
 *     _-~    |
 *   p0-------p1
 *     \      |
 *       \    |
 *         \  |
 *           \|
 *            p3
 */
/*===========================================================================*/
void DecomposeInCase3( in int p0, in int p1, in int p2, in int p3 )
{
    DecomposeInCase2( p0, p1, p2, p3 );
}

/*===========================================================================*/
/**
 *  @brief  Decomposes a tetrahedral cell into a single triangle.
 *  @param  p0 [in] index of the vertex 0
 *  @param  p1 [in] index of the vertex 1
 *  @param  p2 [in] index of the vertex 2
 *  @param  p3 [in] index of the vertex 3
 *
 *  Case 4: This is a special case of case 2.
 *
 *           p0
 *            | ~-_
 *            |    ~-_ 
 *            |       p2,p3
 *            |      /
 *            |    /
 *            |  /
 *            |/
 *            p1
 */
/*===========================================================================*/
void DecomposeInCase4( in int p0, in int p1, in int p2, in int p3 )
{
    int p_front;
    int p_back;

    if ( position_in[p2].z < position_in[p3].z )
    {
        p_front = p2;
        p_back = p3;
    }
    else
    {
        p_front = p3;
        p_back = p2;
    }

    float c_segment_length = Distance( length( length( position_in[p_back] - position_in[p_front] ) ) );
    float c_depth = 0.0;

    EmitOriginalPoint( p0, 0.0 );
    EmitOriginalPoint( p1, 0.0 );
#if defined( ENABLE_EXACT_DEPTH_TESTING )
    vec4 c_gl_Position = gl_PositionIn[p_front];
    vec3 c_position = position_in[p_front].xyz;
    vec3 c_normal = normal_in[p_front].xyz;
    float c_scalar_front = value_in[p_front];
    float c_scalar_back = value_in[p_back];
    c_depth = gl_PositionIn[p_back].z / gl_PositionIn[p_back].w;
    float c_wc_inv_front = position_ndc_in[p_front].w;
    float c_wc_inv_back = position_ndc_in[p_back].w;
    EmitCrossingPoint(
        c_gl_Position,
        c_position,
        c_normal,
        c_scalar_front,
        c_scalar_back,
        c_segment_length,
        c_depth,
        c_wc_inv_front,
        c_wc_inv_back );
#else
    EmitOriginalPoint( p_front, c_segment_length );
#endif

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

    if      ( type == 1 ) { DecomposeInCase1( p0, p1, p2, p3 ); }
    else if ( type == 2 ) { DecomposeInCase2( p0, p1, p2, p3 ); }
    else if ( type == 3 ) { DecomposeInCase3( p0, p1, p2, p3 ); }
    else if ( type == 4 ) { DecomposeInCase4( p0, p1, p2, p3 ); }
}
