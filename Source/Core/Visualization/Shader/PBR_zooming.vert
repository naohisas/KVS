/*****************************************************************************/
/**
 *  @file   zooming.vert
 *  @author Naohisa Sakamoto
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
uniform float object_scale;
uniform float object_depth;
uniform vec2 screen_scale;
uniform sampler2D random_texture;
uniform float random_texture_size_inv;
attribute vec2 random_index;

const float CIRCLE_THRESHOLD = 3.0;
const float CIRCLE_SCALE = 0.564189583547756; // 1.0 / sqrt(PI)

varying vec3 position;
varying vec3 normal;
varying vec2 center;
varying float radius;

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewMatrix; // model-view matrix
uniform mat4 ProjectionMatrix; // projection matrix


/*===========================================================================*/
/**
 *  @brief  Returns a pixel size of the projected particle.
 *  @param  p [in] position of a particle
 *  @return pixel size of the projected particle
 */
/*===========================================================================*/
float footprint_size( in vec4 p )
{
    if ( ProjectionMatrix[3][3] > 0.0 )
    {
        // Orthogonal projection
        return object_scale * 1.0;
    }
    else
    {
        // Perspective projection
        float D = p.z; // depth value
        if ( D < 1.0 ) D = 1.0; // to avoid front-clip
        return object_depth / D;
    }
}

/*===========================================================================*/
/**
 *  @brief  Return the footprint size of the particle in pixel.
 *  @param  p [in] particle position
 *  @return footprint size [pixel]
 */
/*===========================================================================*/
float zooming( in vec4 p )
{
    float s = footprint_size( p );
    float sf = floor( s ); // round-down value of s
    float sc = ceil( s ); // round-up value of s

    // Calculate a probability 'pc' that the footprint size is 'sc'.
    float fraction = fract( s );
    float pc = fraction * ( 2.0 * sf + fraction ) / ( 2.0 * sf + 1.0 );

    // Random number from the random number texture.
    vec2 random_texture_index = random_index * random_texture_size_inv;
    float R = texture2D( random_texture, random_texture_index ).x;

    if ( CIRCLE_THRESHOLD <= 0.0 || s <= CIRCLE_THRESHOLD )
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

    return s;
}

/*===========================================================================*/
/**
 *  @brief  Calculates a size of the particle in pixel.
 */
/*===========================================================================*/
void main()
{
    gl_FrontColor = gl_Color;
    gl_Position = ProjectionMatrix * ModelViewMatrix * gl_Vertex;

#if defined( ENABLE_PARTICLE_ZOOMING )
    gl_PointSize = zooming( gl_Position );
#else
    gl_PointSize = 1.0;
#endif

    normal = gl_Normal.xyz;
    position = vec3( gl_ModelViewMatrix * gl_Vertex );
}
