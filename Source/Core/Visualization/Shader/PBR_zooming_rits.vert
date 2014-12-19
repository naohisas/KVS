/*****************************************************************************/
/**
 *  @file   zooming_rits.vert
 *  @author Makoto Uemura, Satoshi Tanaka
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright 2007 Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: zooming.vert 992 2011-10-15 00:24:45Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
uniform float densityFactor;
uniform int circle_threshold;
uniform vec2 screen_scale;
uniform float object_magnification; //ADD_UEMURA
uniform float screen_magnification; //ADD_UEMURA
uniform vec3 object_center; //ADD_UEMURA
uniform int zoom_mode; //ADD_UEMURA
#if defined( ENABLE_RANDOM_TEXTURE )
uniform sampler2D random_texture;
uniform float random_texture_size_inv;
attribute vec2 identifier;
#endif

const float CIRCLE_SCALE = 0.564189583547756; // 1.0 / sqrt(PI)

varying vec3  position;
varying vec3  normal;
varying vec2  center;
varying float radius;

#if defined( ENABLE_RANDOM_TEXTURE )
/*===========================================================================*/
/**
 *  @brief  Return the footprint size of the particle in pixel.
 *  @param  p [in] particle position
 *  @return footprint size [pixel]
 */
/*===========================================================================*/
float zooming( in vec4 p , float p_size) //ADD_UEMURA
{
    // Calculate the footprint size of the particle.
    float s = p_size; // footprint size of the particle in pixel //ADD_UEMURA
    float sf = floor( s );       // round-down value of s
    float sc = ceil( s );        // round-up value of s

    // Calculate a probability 'pc' that the footprint size is 'sc'.
    float fraction = fract( s );
    float pc = fraction * ( 2.0 * sf + fraction ) / ( 2.0 * sf + 1.0 );

    // Random number from the random number texture.
    vec2 random_texture_index = identifier * random_texture_size_inv;
    float R = texture2D( random_texture, random_texture_index ).x;

    if ( circle_threshold <= 0 || s <= float( circle_threshold ) )
    {
        // Draw the particle as square.
        s = ( ( R < pc ) ? sc : sf );
        radius = 0.0;
    }
    else
    {
        // Draw the particle as circle.
        // Convert position to screen coordinates.
        center = screen_scale + ( ( p.xy / p.w ) * screen_scale );
        radius = ( ( R < pc ) ? sc : sf ) * CIRCLE_SCALE;
        s = ceil( s * CIRCLE_SCALE * 2.0 ) + 1.0;
    }

    return( s );
}

#else
/*===========================================================================*/
/**
 *  @brief  Return the footprint size of the particle in pixel.
 *  @param  p [in] particle position
 *  @return footprint size [pixel]
 */
/*===========================================================================*/
float zooming( in vec4 p , float p_size) //ADD_UEMURA
{
    // Calculate the footprint size of the particle.
    float s = p_size; // footprint size of the particle in pixel //ADD_UEMURA
    float sf = floor( s );       // round-down value of s
    float sc = ceil( s );        // round-up value of s

    // Calculate a probability 'pc' that the footprint size is 'sc'.
    float fraction = fract( s );
    float pc = fraction * ( 2.0 * sf + fraction ) / ( 2.0 * sf + 1.0 );

    // Generate a random floating point using the vertex position.
    float myF = p.x + p.z * p.y;
    int Ri = int( myF * float( 0x0000ffff ) ); // pick 4 bits using mask
    Ri = Ri & 0x000000f0;
    Ri >>= 4;
    float Rf = float( Ri ) / 16.0;
    float R = Rf + sf;

    if ( circle_threshold <= 0 || s <= float( circle_threshold ) )
    {
        // Draw the particle as square.
        s = ( ( R < pc ) ? sc : sf );
        radius = 0.0;
    }
    else
    {
        // Draw the particle as circle.
        // Convert position to screen coordinates.
        center = screen_scale + ( ( p.xy / p.w ) * screen_scale );
        radius = ( ( R < pc ) ? sc : sf ) * CIRCLE_SCALE;
        s = ceil( s * CIRCLE_SCALE * 2.0 ) + 1.0;
    }

    return( s );
}
#endif

/*===========================================================================*/
/**
 *  @brief  Calculates a size of the particle in pixel.
 */
/*===========================================================================*/
void main( void )
{
    gl_FrontColor = gl_Color;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // same as 'ftransform()'

    normal = gl_Normal.xyz;
    position = vec3( gl_ModelViewMatrix * gl_Vertex );

    vec3 view_p_pos = (gl_ModelViewMatrix  * gl_Vertex).xyz; //ADD_UEMURA
    vec3 view_default_d_0 = (gl_ModelViewMatrix  * vec4(object_center, 1.0)).xyz; //ADD_UEMURA

    float d_p_current = length(vec3(0,0,view_p_pos.z)); //ADD_UEMURA
    float d_0_current = length(view_default_d_0); //ADD_UEMURA
    float d_0_default = d_0_current * object_magnification * screen_magnification; //ADD_UEMURA
    float d_p_default = d_0_default - (d_0_current - d_p_current); //ADD_UEMURA
    float p_size = 1.0; //ADD_UEMURA
    if(zoom_mode == 1) p_size = (d_0_default/d_p_current); //ADD_UEMURA
    else if(zoom_mode == 3) p_size = (1.0/d_p_current)*sqrt(d_0_default*d_0_current); //ADD_UEMURA
    else if(zoom_mode == 2) p_size = (d_0_current/d_p_current); //ADD_UEMURA
    gl_PointSize = zooming( gl_Position, p_size ); //ADD_UEMURA
}
