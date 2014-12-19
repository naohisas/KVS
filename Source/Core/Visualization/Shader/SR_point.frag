/*****************************************************************************/
/**
 *  @file   SR_point.frag
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
#include "shading.h"
#include "qualifire.h"
#include "texture.h"


// Input parameters from vertex shader.
FragIn vec3 position; // vertex position in camera coordinate
FragIn vec3 normal; // normal vector in camera coodinate
FragIn vec2 index; // index for accessing to the random texture

// Uniform parameters.
uniform sampler2D random_texture; // random texture to generate random number
uniform float random_texture_size_inv; // reciprocal value of the random texture size
uniform vec2 random_offset; // offset values for accessing to the random texture
uniform ShadingParameter shading; // shading parameters
uniform float opacity; // opacity value


/*===========================================================================*/
/**
 *  @brief  Returns random index.
 *  @param  p [in] pixel coordinate value
 *  @return random index as 2d vector
 */
/*===========================================================================*/
vec2 RandomIndex( in vec2 p )
{
    float x = float( int( index.x ) * 73 );
    float y = float( int( index.y ) * 31 );
    return ( vec2( x, y ) + random_offset + p ) * random_texture_size_inv;
}

/*===========================================================================*/
/**
 *  @brief  Main function of fragment shader.
 */
/*===========================================================================*/
void main()
{
    vec3 color = gl_Color.rgb;
    if ( opacity == 0.0 ) { discard; return; }

    // Stochastic color assignment.
    float R = LookupTexture2D( random_texture, RandomIndex( gl_FragCoord.xy ) ).a;
    if ( R > opacity ) { discard; return; }

    // Light position in camera coordinate.
    vec3 light_position = gl_LightSource[0].position.xyz;

    // Light vector (L) and Normal vector (N) in camera coordinate.
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( normal );

    // Shading.
#if   defined( ENABLE_LAMBERT_SHADING )
    vec3 shaded_color = ShadingLambert( shading, color, L, N );

#elif defined( ENABLE_PHONG_SHADING )
    vec3 V = normalize( -position );
    vec3 shaded_color = ShadingPhong( shading, color, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
    vec3 V = normalize( -position );
    vec3 shaded_color = ShadingBlinnPhong( shading, color, L, N, V );

#else // DISABLE SHADING
    vec3 shaded_color = ShadingNone( shading, color );
#endif

    gl_FragColor = vec4( shaded_color, 1.0 );
}
