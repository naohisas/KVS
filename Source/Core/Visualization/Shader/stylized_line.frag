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
#include "texture.h"

// Input parameters from vertex shader.
FragIn vec3 position;
FragIn vec3 normal;

FragIn vec4 diffuse;
FragIn vec3 up_vec;
FragIn vec3 side_vec;
FragIn vec3 coord;
FragIn vec2 depth0;
FragIn vec2 depth1;

// Uniform parameters.
uniform ShadingParameter shading;
uniform float line_width;
uniform sampler2D shape_texture;
uniform sampler2D diffuse_texture;

/*===========================================================================*/
/**
 *  @brief  Main function of fragment shader.
 */
/*===========================================================================*/
void main()
{
    vec2 tcd = coord.xy;
    tcd.x = ( coord.x / coord.z ) * 0.5 + 0.5;

    vec3 tex = LookupTexture2D( shape_texture, tcd.xy ).xyz;
    tex.x = tex.x * 2.0 - 1.0;

    if ( tcd.x < 0.0 || tcd.x > 1.0 )
    {
        gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
    }
    else
    {
        // Light position in camera coordinate.
        vec3 light_position = gl_LightSource[0].position.xyz;

        // Light vector (L) and Normal vector (N) in camera coordinate.
        vec3 L = normalize( light_position - position );
        vec3 N = side_vec * tex.x - tex.y * up_vec;

        vec4 color = diffuse * LookupTexture2D( diffuse_texture, tcd.xy );

        // Shading.
#if   defined( ENABLE_LAMBERT_SHADING )
        vec3 shaded_color = ShadingLambert( shading, color.rgb, L, N );

#elif defined( ENABLE_PHONG_SHADING )
        vec3 V = normalize( -position );
        vec3 shaded_color = ShadingPhong( shading, color.rgb, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
        vec3 V = normalize( -position );
        vec3 shaded_color = ShadingBlinnPhong( shading, color.rgb, L, N, V );

#else // DISABLE SHADING
        vec3 shaded_color = ShadingNone( shading, color.rgb );
#endif

        gl_FragColor = vec4( shaded_color, color.a );
    }

    vec2 rdep = tex.y * depth1 + ( 1.0 - tex.y ) * depth0;
    gl_FragDepth = ( rdep.x / rdep.y ) * 0.5 + 0.5;
}
