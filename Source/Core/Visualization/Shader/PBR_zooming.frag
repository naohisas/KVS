/*****************************************************************************/
/**
 *  @file   zooming.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: zooming.frag 490 2010-03-22 12:00:41Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "shading.h"

varying vec3  position;
varying vec3  normal;
varying vec2  center;
varying float radius;

uniform ShadingParameter shading;

/*===========================================================================*/
/**
 *  @brief  Calculates a shaded color of the particle with Lambert shading.
 */
/*===========================================================================*/
void main( void )
{
#if defined( ENABLE_PARTICLE_ZOOMING )
    // Discard pixels outside circle.
    if ( radius > 0.0 )
    {
        if ( distance( gl_FragCoord.xy, center ) > radius ) discard;
    }
#endif

    // Light position.
    vec3 light_position = gl_LightSource[0].position.xyz;

    // Light vector (L) and Normal vector (N)
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( gl_NormalMatrix * normal );

#if   defined( ENABLE_LAMBERT_SHADING )
    vec3 shaded_color = ShadingLambert( shading, gl_Color.xyz, L, N );

#elif defined( ENABLE_PHONG_SHADING )
    vec3 V = normalize( -position );
    vec3 shaded_color = ShadingPhong( shading, gl_Color.xyz, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
    vec3 V = normalize( -position );
    vec3 shaded_color = ShadingBlinnPhong( shading, gl_Color.xyz, L, N, V );

#else // DISABLE SHADING
    vec3 shaded_color = ShadingNone( shading, gl_Color.xyz );
#endif

    gl_FragColor = vec4( shaded_color, 1.0 );
}
