/*****************************************************************************/
/**
 *  @file   tetrahedra.frag
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
#include <shading.h>
#include <qualifire.h>
#include <texture.h>


// Input variables from geometry shader
FragInWithNoperspective vec3 position; // 3D vertex position in camera coordinate
FragInWithNoperspective vec3 normal; // normal vector in camera coordinate
FragInWithNoperspective vec2 random_index; // index for accessing to the random texture
FragInWithNoperspective vec2 position_ndc; // 2D vertex position in normalized device coordinate
FragInWithNoperspective float scalar_front; // scalar value on the front face
FragInWithNoperspective float scalar_back; // scalar value on the back face
FragInWithNoperspective float depth_front; // depth value at the front face
FragInWithNoperspective float depth_back; // depth value at the back face
FragInWithNoperspective float wc_inv_front; // reciprocal value of the w-component at the front face in clip coordinate
FragInWithNoperspective float wc_inv_back; // reciprocal value of the w-component at the back face in clip coordinate

// Uniform variables.
uniform sampler2D random_texture; // random number texture
uniform sampler2D preintegration_texture; // pre-integration texture
uniform sampler1D transfer_function_texture; // transfer function texture
uniform sampler1D T_texture; // T value texture
uniform sampler1D invT_texture; // inverse value of T texture
uniform float random_texture_size_inv; // reciprocal value of random texture size
uniform vec2 random_offset; // offset values for accessing to the random texture
uniform float random_bias;
uniform float sampling_step_inv; // inverse value of the sampling step
uniform float maxT;
uniform float delta;
uniform float delta2;
uniform ShadingParameter shading; // shading parameters

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewProjectionMatrixInverse; // inverse matrix of model-view projection matrix


// Returns an adjected index for refering the texture.
#define ADJUST( x ) \
    ( delta + x * ( 1.0 - 2.0 * delta ) )

// Returns an adjusted index for refering the T-inverse texture.
#define ADJUST_FOR_INVT( x ) \
    ( ( delta2 + x * ( 1.0 - 2.0 * delta2 ) ) / maxT )

// Returns a depth value in window coordinate.
#define DEPTH( z ) \
    ( gl_DepthRange.diff * Z + gl_DepthRange.near + gl_DepthRange.far ) / 2.0

/*===========================================================================*/
/**
 *  @brief  Returns a random number.
 *  @return random number
 */
/*===========================================================================*/
float RandomNumber()
{
    float x = float( int( random_index.x ) * 73 );
    float y = float( int( random_index.y ) * 31 );
    vec2 p = gl_FragCoord.xy;
    vec2 index = ( vec2( x, y ) + random_offset + p ) * random_texture_size_inv;
    return LookupTexture2D( random_texture, index ).r;
}

/*===========================================================================*/
/**
 *  @brief  Returns a transformed object coordinate from normalized device coordinate.
 *  @param  ndc [in] coordinate in normalized device coordinate
 *  @return coordinate in object coordinate
 */
/*===========================================================================*/
vec3 NormalizedDeviceCoordinateToObjectCoordinate( const in vec4 ndc )
{
    vec4 temp = ModelViewProjectionMatrixInverse * ndc;
    return temp.xyz / temp.w;
}

/*===========================================================================*/
/**
 *  @brief  Returns a distance between depths in object coordinate.
 *  @param  Zf [in] depth value of the front face
 *  @param  Zb [in] depth value of the back face
 *  @return distance
 */
/*===========================================================================*/
float DistanceInObjectCoordinate( const in float Zf, const in float Zb )
{
    vec4 front_ndc = vec4( position_ndc, Zf, 1.0 );
    vec4 back_ndc = vec4( position_ndc, Zb, 1.0 );
    vec3 front_obj = NormalizedDeviceCoordinateToObjectCoordinate( front_ndc );
    vec3 back_obj = NormalizedDeviceCoordinateToObjectCoordinate( back_ndc );
    return length( front_obj - back_obj );
}

/*===========================================================================*/
/**
 *  @brief  Main function of fragment shader.
 */
/*===========================================================================*/
void main()
{
    // Scalar values and depth values at the front and back points.
    float Sf = scalar_front / wc_inv_front;
    float Sb = scalar_back / wc_inv_back;
    float Zf = depth_front;
    float Zb = depth_back;

    // Alpha correction factor.
    float a = DistanceInObjectCoordinate( Zf, Zb ) * sampling_step_inv;

    // Transparency calculated with pre-integration.
    float dS = Sb - Sf;
    float trans = exp( -LookupTexture2D( preintegration_texture, vec2( ADJUST( Sf ), ADJUST( Sb ) ) ).r * a );

    // Stochastic color assignment.
    float R = RandomNumber();
    if ( R <= trans ) { discard; return; }

    // Depth calculation by inverse transformation sampling.
    float S; // scalar value at the estimated position
    float W; // probability variable for depth calculation
    float T_Sf = LookupTexture1D( T_texture, ADJUST( Sf ) ).r;
    float log_R = -log( R );
    const float epsilon = 0.001;
    if ( abs( dS ) >= epsilon )
    {
        S = LookupTexture1D( invT_texture, ADJUST_FOR_INVT( log_R * dS / a + T_Sf ) ).r;
        W = ( S - Sf ) / dS;
    }
    else
    {
        S = Sf;
        W = -log_R / log( trans );
    }
    W = clamp( W, 0.0, 1.0 );
    float Z = Zf + W * ( Zb - Zf );

    // Color value for the point.
    vec3 color = LookupTexture1D( transfer_function_texture, ADJUST( S ) ).rgb;

    // Light position in camera coordinate.
    vec3 light_position = gl_LightSource[0].position.xyz;

    // Light vector (L) and Normal vector (N) in camera coordinate.
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( normal );

#if   defined( ENABLE_LAMBERT_SHADING )
    vec3 shaded_color = ShadingLambert( shading, color, L, N );

#elif defined( ENABLE_PHONG_SHADING )
    vec3 V = normalize( - position );
    vec3 shaded_color = ShadingPhong( shading, color, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
    vec3 V = normalize( - position );
    vec3 shaded_color = ShadingBlinnPhong( shading, color, L, N, V );

#else // DISABLE SHADING
    vec3 shaded_color = ShadingNone( shading, color );
#endif

    gl_FragColor = vec4( shaded_color, 1.0 );
    gl_FragDepth = DEPTH( Z );
}
