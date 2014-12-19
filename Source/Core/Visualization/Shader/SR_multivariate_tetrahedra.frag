/*****************************************************************************/
/**
 *  @file   multivariate_tetrahedra.frag
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
#include "shading.h"

varying vec3 position;
varying vec3 normal;
varying vec2 random_index;
varying vec2 scalar_front;
varying vec2 scalar_back;
varying float segment_length;

uniform sampler3D preintegration_texture0;
uniform sampler3D preintegration_texture1;
uniform sampler2D random_texture;
uniform vec2 screen_scale;
uniform vec2 screen_scale_inv;
uniform vec2 preintegration_scale_offset;
uniform float random_texture_size_inv;
uniform vec2 random_offset;
uniform int start_volume;
uniform int end_volume;
uniform ShadingParameter shading;

void main()
{
    int accept = 0;
    vec3 accum_color = vec3( 0.0, 0.0, 0.0 );

    for ( int i = start_volume; i < end_volume; i++ )
    {
        vec4 lutdata;
        if ( i == 0 )
        {
            vec3 lutcoord = vec3( scalar_front.x, scalar_back.x, segment_length );
            lutdata = texture3D( preintegration_texture0, lutcoord );
        }
        else
        {
            vec3 lutcoord = vec3( scalar_front.y, scalar_back.y, segment_length );
            lutdata = texture3D( preintegration_texture1, lutcoord );
        }

        if ( lutdata.a == 0.0 ) continue;

        vec2 random_position = ( vec2( float( int( random_index.x ) * 73 ), float( int( random_index.y ) * 31 ) ) 
                                 + random_offset + gl_FragCoord.xy ) * random_texture_size_inv;

        float randf = texture2D( random_texture, random_position ).x;
        if ( randf > lutdata.a ) continue;

        vec3 frag_color = lutdata.xyz / lutdata.a;

        // Light position.
        vec3 light_position = gl_LightSource[0].position.xyz;

        // Light vector (L) and Normal vector (N)
        vec3 L = normalize( light_position - position );
        vec3 N = normalize( normal );

#if   defined( ENABLE_LAMBERT_SHADING )
        vec3 shaded_color = ShadingLambert( shading, frag_color, L, N );

#elif defined( ENABLE_PHONG_SHADING )
        vec3 V = normalize( - position );
        vec3 shaded_color = ShadingPhong( shading, frag_color, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
        vec3 V = normalize( - position );
        vec3 shaded_color = ShadingBlinnPhong( shading, frag_color, L, N, V );

#else // DISABLE SHADING
        vec3 shaded_color = ShadingNone( shading, frag_color );
#endif

        accept += 1;
        accum_color += shaded_color;
    }

    if ( accept == 0 ) { discard; return; }
    else if ( accept == 1 )
    {
        gl_FragColor = vec4( accum_color, 1.0 );
    }
    else
    {
        gl_FragColor = vec4( 0.5 * accum_color, 1.0 );
    }
}
