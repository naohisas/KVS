/*****************************************************************************/
/**
 *  @file   multiple_tetrahedra2.frag
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
varying float depth_front;
varying float depth_back;

varying float scalar_front;
varying float scalar_back;
varying float segment_length;

uniform sampler3D preintegration_texture0;
uniform sampler3D preintegration_texture1;
uniform sampler2D random_texture;
uniform sampler2D depth_texture;
uniform sampler2D extra_texture;

uniform vec2 screen_scale;
uniform vec2 screen_scale_inv;
uniform vec2 preintegration_scale_offset;

uniform float random_texture_size_inv;
uniform vec2 random_offset;

uniform ShadingParameter shading;

float distance_to_d( const in float dist )
{
    return( ( dist - preintegration_scale_offset.y ) / preintegration_scale_offset.x );
}

int check_overlap_case( void )
{
    vec2 screen_coord = gl_FragCoord.xy * screen_scale_inv;
    vec4 extra = texture2D( extra_texture, screen_coord );

    if ( extra.z == 0.0 ) return( 0 );

    float v1f_d = extra.w;
    float v1b_d = texture2D( depth_texture, screen_coord ).x;
    float v2f_d = depth_front;
    float v2b_d = depth_back;

    if ( v1b_d >= 1.0 ) return( 0 );

    if ( v2b_d < v1b_d )
    {
        if ( v1f_d < v2b_d )
        {
            if      ( v1f_d < v2f_d ) return( 1 );
            else if ( v1f_d > v2f_d ) return( 2 );
        }
    }
    else if ( v2b_d > v1b_d )
    {
        if ( v2f_d < v1b_d )
        {
            if      ( v2f_d < v1f_d ) return( 3 );
            else if ( v2f_d > v1f_d ) return( 4 );
        }
    }

    return( 0 );
}

vec3 calculate_shaded_color( const in vec3 frag_color )
{
    // Light position.
    vec3 light_position = gl_LightSource[0].position.xyz;

#if   defined( ENABLE_LAMBERT_SHADING )
    // Light vector (L) and Normal vector (N)
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( normal );
    vec3 shaded_color = ShadingLambert( shading, frag_color, L, N );

#elif defined( ENABLE_PHONG_SHADING )
    // Light vector (L) and Normal vector (N)
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( normal );
    vec3 V = normalize( - position );
    vec3 shaded_color = ShadingPhong( shading, frag_color, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
    // Light vector (L) and Normal vector (N)
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( normal );
    vec3 V = normalize( - position );
    vec3 shaded_color = ShadingBlinnPhong( shading, frag_color, L, N, V );

#else // DISABLE SHADING
    vec3 shaded_color = ShadingNone( shading, frag_color );
#endif
    return( shaded_color );
}

vec4 color_of_sf_sb_d( const in vec3 lutcoord, const in int index, const in int rand_i )
{
    vec4 lutdata;
    if ( index == 0 ) lutdata = texture3D( preintegration_texture0, lutcoord );
    else              lutdata = texture3D( preintegration_texture1, lutcoord );
    if ( lutdata.a == 0.0 ) return( vec4( 0.0 ) );

    vec2 random_position = ( vec2( float( int( random_index.x ) * ( 73 + rand_i ) ), float( int( random_index.y ) * ( 31 + rand_i ) ) ) 
                + random_offset + gl_FragCoord.xy ) * random_texture_size_inv;
    float randf = texture2D( random_texture, random_position ).x;
    if ( randf > lutdata.a ) return( vec4( 0.0 ) );

    vec3 frag_color = lutdata.xyz / lutdata.a;

    return( vec4( calculate_shaded_color( frag_color ), lutdata.a ) );
}

vec4 two_color_average( const in vec4 color1, const in vec4 color2 )
{
    vec3 accum_color = vec3( 0.0 );
    int count = 0;
    if ( color1.a > 0.0 )
    {
        accum_color += color1.xyz;
        count += 1;
    }
    if ( color2.a > 0.0 )
    {
        accum_color += color2.xyz;
        count += 1;
    }

    if      ( count == 0 ) return( vec4( 0.0 ) );
    else if ( count == 1 ) return( vec4( accum_color, 1.0 ) );
    return( vec4( 0.5 * accum_color, 1.0 ) );
}

// ---------------------------------
//  No overlap case.
//
//  eye -- 1f -- 1b -- 2f -- 2b -->
//  eye -- 2f -- 2b -- 1f -- 1b -->
// ---------------------------------
void calculate_case_0( void )
{
    vec3 lutcoord = vec3( scalar_front, scalar_back, segment_length );
    vec4 shaded_color = color_of_sf_sb_d( lutcoord, 1, 0 );
    if ( shaded_color.a == 0.0 ) { discard; return; }

    gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
}

// ---------------------------------
//  Overlap case 1.
//
//  eye -- 1f -- 2f -- 2b -- 1b -->
// ---------------------------------
void calculate_case_1( void )
{
    vec2 screen_coord = gl_FragCoord.xy * screen_scale_inv;
    vec4 extra = texture2D( extra_texture, screen_coord );

    float v1b_d = texture2D( depth_texture, screen_coord ).x;
    float v1f_d = extra.w;
    float v2f_d = depth_front;
    float v2b_d = depth_back;

    float v1b_to_v1f = abs( v1b_d - v1f_d );
    float v2b_to_v2f = abs( v2b_d - v2f_d );

    // 1f -> 2f : use v1
    {
        float ratio = ( v2f_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord = vec3(
            extra.x,
            ( 1.0 - ratio ) * extra.x + ratio * extra.y,
            ratio * extra.z );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 0, 0 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 2f -> 2b : use v1 + v2
    {
        float ratio1 = ( v2f_d - v1f_d ) / v1b_to_v1f;
        float ratio2 = ( v2b_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord1 = vec3(
            ( 1.0 - ratio1 ) * extra.x + ratio1 * extra.y,
            ( 1.0 - ratio2 ) * extra.x + ratio2 * extra.y,
            segment_length );

        vec3 lutcoord2 = vec3( scalar_front, scalar_back, segment_length );

        vec4 shaded_color1 = color_of_sf_sb_d( lutcoord1, 0, 1 );
        vec4 shaded_color2 = color_of_sf_sb_d( lutcoord2, 1, 2 );
        vec4 shaded_color = two_color_average( shaded_color1, shaded_color2 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 2b -> 1b : use v1
    {
        float ratio = ( v2b_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord = vec3(
            ( 1.0 - ratio ) * extra.x + ratio * extra.y,
            extra.y,
            ( 1.0 - ratio ) * extra.z );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 0, 3 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    discard;
}

// ---------------------------------
//  Overlap case 2.
//
//  eye -- 2f -- 1f -- 2b -- 1b -->
// ---------------------------------
void calculate_case_2( void )
{
    vec2 screen_coord = gl_FragCoord.xy * screen_scale_inv;
    vec4 extra = texture2D( extra_texture, screen_coord );

    float v1b_d = texture2D( depth_texture, screen_coord ).x;
    float v1f_d = extra.w;
    float v2f_d = depth_front;
    float v2b_d = depth_back;

    float v1b_to_v1f = abs( v1b_d - v1f_d );
    float v2b_to_v2f = abs( v2b_d - v2f_d );

    // 2f -> 1f : use v2
    {
        float ratio = ( v1f_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord = vec3(
            scalar_front,
            ( 1.0 - ratio ) * scalar_front + ratio * scalar_back,
            ratio * segment_length );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 1, 0 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 1f -> 2b : use v1 + v2
    {
        float ratio1 = ( v2b_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord1 = vec3(
            extra.x,
            ( 1.0 - ratio1 ) * extra.x + ratio1 * extra.y,
            ratio1 * extra.z );

        float ratio2 = ( v1f_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord2 = vec3(
            ( 1.0 - ratio2 ) * scalar_front + ratio2 * scalar_back,
            scalar_back, ratio1 * extra.z );

        vec4 shaded_color1 = color_of_sf_sb_d( lutcoord1, 0, 1 );
        vec4 shaded_color2 = color_of_sf_sb_d( lutcoord2, 1, 2 );
        vec4 shaded_color = two_color_average( shaded_color1, shaded_color2 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 2b -> 1b : use v1
    {
        float ratio = ( v2b_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord = vec3(
            ( 1.0 - ratio ) * extra.x + ratio * extra.y,
            extra.y,
            ( 1.0 - ratio ) * extra.z );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 0, 3 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    discard;
}

// ---------------------------------
//  Overlap case 3.
//
//  eye -- 2f -- 1f -- 1b -- 2b -->
// ---------------------------------
void calculate_case_3( void )
{
    vec2 screen_coord = gl_FragCoord.xy * screen_scale_inv;
    vec4 extra = texture2D( extra_texture, screen_coord );

    float v1b_d = texture2D( depth_texture, screen_coord ).x;
    float v1f_d = extra.w;
    float v2f_d = depth_front;
    float v2b_d = depth_back;

    float v1b_to_v1f = abs( v1b_d - v1f_d );
    float v2b_to_v2f = abs( v2b_d - v2f_d );

    // 2f -> 1f : use v2
    {
        float ratio = ( v1f_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord = vec3(
            scalar_front,
            ( 1.0 - ratio ) * scalar_front + ratio * scalar_back,
            ratio * segment_length );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 1, 0 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 1f -> 1b : use v1 + v2
    {
        vec3 lutcoord1 = extra.xyz;

        float ratio1 = ( v1f_d - v2f_d ) / v2b_to_v2f;
        float ratio2 = ( v1b_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord2 = vec3(
            ( 1.0 - ratio1 ) * scalar_front + ratio1 * scalar_back,
            ( 1.0 - ratio2 ) * scalar_front + ratio2 * scalar_back,
            extra.z );

        vec4 shaded_color1 = color_of_sf_sb_d( lutcoord1, 0, 1 );
        vec4 shaded_color2 = color_of_sf_sb_d( lutcoord2, 1, 2 );
        vec4 shaded_color = two_color_average( shaded_color1, shaded_color2 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 1b -> 2b : use v2
    {
        float ratio = ( v1b_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord = vec3(
            ( 1.0 - ratio ) * scalar_front + ratio * scalar_back,
            scalar_back,
            ( 1.0 - ratio ) * segment_length );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 1, 3 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    discard;
}

// ---------------------------------
//  Overlap case 4.
//
//  eye -- 1f -- 2f -- 1b -- 2b -->
// ---------------------------------
void calculate_case_4( void )
{
    vec2 screen_coord = gl_FragCoord.xy * screen_scale_inv;
    vec4 extra = texture2D( extra_texture, screen_coord );

    float v1b_d = texture2D( depth_texture, screen_coord ).x;
    float v1f_d = extra.w;
    float v2f_d = depth_front;
    float v2b_d = depth_back;

    float v1b_to_v1f = abs( v1b_d - v1f_d );
    float v2b_to_v2f = abs( v2b_d - v2f_d );

    // 1f -> 2f : use v1
    {
        float ratio = ( v2f_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord = vec3(
            extra.x,
            ( 1.0 - ratio ) * extra.x + ratio * extra.y,
            ratio * extra.z );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 0, 0 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 2f -> 1b : use v1 + v2
    {
        float ratio1 = ( v2f_d - v1f_d ) / v1b_to_v1f;
        vec3 lutcoord1 = vec3(
            ( 1.0 - ratio1 ) * extra.x + ratio1 * extra.y,
            extra.y,
            ( 1.0 - ratio1 ) * extra.z );

        float ratio2 = ( v1b_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord2 = vec3(
            scalar_front,
            ( 1.0 - ratio2 ) * scalar_front + ratio2 * scalar_back,
            ( 1.0 - ratio1 ) * extra.z );

        vec4 shaded_color1 = color_of_sf_sb_d( lutcoord1, 0, 5 );
        vec4 shaded_color2 = color_of_sf_sb_d( lutcoord2, 1, 10 );
        vec4 shaded_color = two_color_average( shaded_color1, shaded_color2 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    // 1b -> 2b : use v2
    {
        float ratio = ( v1b_d - v2f_d ) / v2b_to_v2f;
        vec3 lutcoord = vec3(
            ( 1.0 - ratio ) * scalar_front + ratio * scalar_back,
            scalar_back,
            ( 1.0 - ratio ) * segment_length );
        vec4 shaded_color = color_of_sf_sb_d( lutcoord, 1, 15 );
        if ( shaded_color.a > 0.0 )
        {
            gl_FragData[0] = vec4( shaded_color.xyz, 1.0 );
            return;
        }
    }

    discard;
}

void main( void )
{
    gl_FragDepth = depth_front;
    gl_FragData[1] = vec4( 0.0, 0.0, 0.0, 1.0 );

    int overlap = check_overlap_case();

    if      ( overlap == 0 ) { calculate_case_0(); }
    else if ( overlap == 1 ) { calculate_case_1(); }
    else if ( overlap == 2 ) { calculate_case_2(); }
    //else if ( overlap == 2 ) { gl_FragData[0] = vec4( 0.0, 1.0, 0.0, 1.0 ); }
    else if ( overlap == 3 ) { calculate_case_3(); }
    //else if ( overlap == 3 ) { gl_FragData[0] = vec4( 0.0, 0.0, 1.0, 1.0 ); }
    else if ( overlap == 4 ) { calculate_case_4(); }
    //else if ( overlap == 4 ) { gl_FragData[0] = vec4( 1.0, 1.0, 0.0, 1.0 ); }

    //if      ( overlap == 1 ) { gl_FragData[0] = vec4( 1.0, 0.0, 0.0, 1.0 ); }
    //else if ( overlap == 2 ) { gl_FragData[0] = vec4( 0.0, 1.0, 0.0, 1.0 ); }
    //else if ( overlap == 3 ) { gl_FragData[0] = vec4( 0.0, 0.0, 1.0, 1.0 ); }
    //else if ( overlap == 4 ) { gl_FragData[0] = vec4( 1.0, 1.0, 0.0, 1.0 ); }

}
