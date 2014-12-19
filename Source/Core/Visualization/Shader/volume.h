/*****************************************************************************/
/**
 *  @file   volume.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: volume.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
struct VolumeParameter
{
    vec3 resolution; // volume resolution
    vec3 resolution_ratio; // ratio of the resolution (256x256x128 => 1:1:0.5)
    vec3 resolution_reciprocal; // reciprocal number of the resolution
    float min_range; // min. range of the value
    float max_range; // max. range of the value
};

/*===========================================================================*/
/**
 *  @brief  Returns gradient vector estimated from six adjacent scalars.
 *  @param  v [in] volume data
 *  @param  p [in] sampling point
 *  @param  o [in] offset
 *  @return gradient vector
 */
/*===========================================================================*/
vec3 VolumeGradient( in sampler3D v, in vec3 p, in vec3 o )
{
    float s0 = texture3D( v, p + vec3( o.x, 0.0, 0.0 ) ).w;
    float s1 = texture3D( v, p + vec3( 0.0, o.y, 0.0 ) ).w;
    float s2 = texture3D( v, p + vec3( 0.0, 0.0, o.z ) ).w;
    float s3 = texture3D( v, p - vec3( o.x, 0.0, 0.0 ) ).w;
    float s4 = texture3D( v, p - vec3( 0.0, o.y, 0.0 ) ).w;
    float s5 = texture3D( v, p - vec3( 0.0, 0.0, o.z ) ).w;

    return vec3( s3 - s0, s4 - s1, s5 - s2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns gradient vector estimated from eight adjacent scalars.
 *  @param  v [in] volume data
 *  @param  p [in] sampling point
 *  @param  o [in] offset
 *  @return gradient vector
 */
/*===========================================================================*/
vec3 VolumeGradient8( in sampler3D v, in vec3 p, in vec3 o )
{
    vec3 g0 = VolumeGradient( v, p, o );
    vec3 g1 = VolumeGradient( v, p + vec3( -o.x, -o.y, -o.z ), o );
    vec3 g2 = VolumeGradient( v, p + vec3(  o.x,  o.y,  o.z ), o );
    vec3 g3 = VolumeGradient( v, p + vec3( -o.x,  o.y, -o.z ), o );
    vec3 g4 = VolumeGradient( v, p + vec3(  o.x, -o.y,  o.z ), o );
    vec3 g5 = VolumeGradient( v, p + vec3( -o.x, -o.y,  o.z ), o );
    vec3 g6 = VolumeGradient( v, p + vec3(  o.x,  o.y, -o.z ), o );
    vec3 g7 = VolumeGradient( v, p + vec3( -o.x,  o.y,  o.z ), o );
    vec3 g8 = VolumeGradient( v, p + vec3(  o.x, -o.y, -o.z ), o );
    vec3 mix0 = mix( mix( g1, g2, 0.5 ), mix( g3, g4, 0.5 ), 0.5 );
    vec3 mix1 = mix( mix( g5, g6, 0.5 ), mix( g7, g8, 0.5 ), 0.5 );

    return mix( g0, mix( mix0, mix1, 0.5 ), 0.75 );
}
