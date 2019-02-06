/*****************************************************************************/
/**
 *  @file   Shading.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: shading.h 1015 2012-02-09 07:42:04Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
struct ShadingParameter
{
    float Ka; // ambient
    float Kd; // diffuse
    float Ks; // specular
    float S;  // shininess
};

vec3 ShadingNone( in ShadingParameter shading, in vec3 color )
{
    return( color );
}

vec3 ShadingLambert( in ShadingParameter shading, in vec3 color, in vec3 L, in vec3 N )
{
#if defined( ENABLE_TWO_SIDE_LIGHTING )
    float dd = abs( dot( N, L ) );
#else
    float dd = max( dot( N, L ), 0.0 );
#endif

    float Ia = shading.Ka;
    float Id = shading.Kd * dd;

    return( color * ( Ia + Id ) );
}

vec3 ShadingPhong( in ShadingParameter shading, in vec3 color, in vec3 L, in vec3 N, in vec3 V )
{
    vec3 R = reflect( -L, N );
#if defined( ENABLE_TWO_SIDE_LIGHTING )
    float dd = abs( dot( N, L ) );
    float ds = pow( abs( dot( R, V ) ), shading.S );
#else
    float dd = max( dot( N, L ), 0.0 );
    float ds = pow( max( dot( R, V ), 0.0 ), shading.S );
#endif
    if ( dd <= 0.0 ) ds = 0.0;

    float Ia = shading.Ka;
    float Id = shading.Kd * dd;
    float Is = shading.Ks * ds;

    return( color * ( Ia + Id ) + Is );
}

vec3 ShadingBlinnPhong( in ShadingParameter shading, in vec3 color, in vec3 L, in vec3 N, in vec3 V )
{
    vec3 H = normalize( L + V );
#if defined( ENABLE_TWO_SIDE_LIGHTING )
    float dd = abs( dot( N, L ) );
    float ds = pow( abs( dot( H, N ) ), shading.S );
#else
    float dd = max( dot( N, L ), 0.0 );
    float ds = pow( max( dot( H, N ), 0.0 ), shading.S );
#endif
    if ( dd <= 0.0 ) ds = 0.0;

    float Ia = shading.Ka;
    float Id = shading.Kd * dd;
    float Is = shading.Ks * ds;

    return( color * ( Ia + Id ) + Is );
}

vec3 SSAOShadingLambert( in ShadingParameter shading, in vec3 color, in vec3 L, in vec3 N, in float occlusion )
{
#if defined( ENABLE_TWO_SIDE_LIGHTING )
    float dd = abs( dot( N, L ) );
#else
    float dd = max( dot( N, L ), 0.0 );
#endif

    float Ia = shading.Ka * occlusion;
    float Id = shading.Kd * dd;

    return( color * ( Ia + Id ) );
}

vec3 SSAOShadingPhong( in ShadingParameter shading, in vec3 color, in vec3 L, in vec3 N, in vec3 V, in float occlusion )
{
    vec3 R = reflect( -L, N );
#if defined( ENABLE_TWO_SIDE_LIGHTING )
    float dd = abs( dot( N, L ) );
    float ds = pow( abs( dot( R, V ) ), shading.S );
#else
    float dd = max( dot( N, L ), 0.0 );
    float ds = pow( max( dot( R, V ), 0.0 ), shading.S );
#endif
    if ( dd <= 0.0 ) ds = 0.0;

    float Ia = shading.Ka * occlusion;
    float Id = shading.Kd * dd;
    float Is = shading.Ks * ds;

    return( color * ( Ia + Id ) + Is );
}

vec3 SSAOShadingBlinnPhong( in ShadingParameter shading, in vec3 color, in vec3 L, in vec3 N, in vec3 V, in float occlusion )
{
    vec3 H = normalize( L + V );
#if defined( ENABLE_TWO_SIDE_LIGHTING )
    float dd = abs( dot( N, L ) );
    float ds = pow( abs( dot( H, N ) ), shading.S );
#else
    float dd = max( dot( N, L ), 0.0 );
    float ds = pow( max( dot( H, N ), 0.0 ), shading.S );
#endif
    if ( dd <= 0.0 ) ds = 0.0;

    float Ia = shading.Ka * occlusion;
    float Id = shading.Kd * dd;
    float Is = shading.Ks * ds;

    return( color * ( Ia + Id ) + Is );
}
