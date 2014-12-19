/*****************************************************************************/
/**
 *  @file   shader.frag
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: shading.frag 485 2010-03-09 05:44:44Z naohisa.sakamoto $
 */
/*****************************************************************************/
#version 120
#include "shading.h"
#include "qualifire.h"

// Input parameters from vertex shader.
FragIn vec3 position;
FragIn vec3 normal;
FragIn vec2 center;

// Uniform parameters.
uniform ShadingParameter shading;

uniform float line_width;
uniform float outline_width;
uniform vec3 outline_color;


/*===========================================================================*/
/**
 *  @brief  Main function of fragment shader.
 */
/*===========================================================================*/
void main()
{
    // Outline.
    float center_line_width = line_width * 0.5 - outline_width;
    if ( distance( gl_FragCoord.xy, center ) >= center_line_width )
    {
        // The opacity value is set to 0.9 for diminishing aliasing.
        float opacity = 0.9;
        gl_FragColor = vec4( outline_color, opacity );
        return;
    }

    vec3 color = gl_Color.rgb;

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
