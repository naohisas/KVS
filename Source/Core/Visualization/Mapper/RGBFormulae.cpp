/*****************************************************************************/
/**
 *  @file   RGBFormulae.cpp
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
#include "RGBFormulae.h"
#include <cmath>


namespace
{

double deg2rad( double d ) { return( d * 3.141592 / 180.0 ); }

double f00( double   ) { return( 0 ); }
double f01( double   ) { return( 0.5 ); }
double f02( double   ) { return( 1.0 ); }
double f03( double x ) { return( x ); }
double f04( double x ) { return( x * x ); }
double f05( double x ) { return( x * x * x ); }
double f06( double x ) { return( x * x * x * x ); }
double f07( double x ) { return( std::sqrt(x) ); }
double f08( double x ) { return( std::sqrt( std::sqrt(x) ) ); }
double f09( double x ) { return( std::sin( deg2rad( 90.0 * x ) ) ); }
double f10( double x ) { return( std::cos( deg2rad( 90.0 * x ) ) ); }
double f11( double x ) { return( std::abs( x - 0.5 ) ); }
double f12( double x ) { return( ( 2.0 * x - 1.0 ) * ( 2.0 * x - 1.0 ) ); }
double f13( double x ) { return( std::sin( deg2rad( 180.0 * x ) ) ); }
double f14( double x ) { return( std::abs( std::cos( deg2rad( 180.0 * x ) ) ) ); }
double f15( double x ) { return( std::sin( deg2rad( 360.0 * x ) ) ); }
double f16( double x ) { return( std::cos( deg2rad( 360.0 * x ) ) ); }
double f17( double x ) { return( std::abs( std::sin( deg2rad( 360.0 * x ) ) ) ); }
double f18( double x ) { return( std::abs( std::cos( deg2rad( 360.0 * x ) ) ) ); }
double f19( double x ) { return( std::abs( std::sin( deg2rad( 720.0 * x ) ) ) ); }
double f20( double x ) { return( std::abs( std::cos( deg2rad( 720.0 * x ) ) ) ); }
double f21( double x ) { return( 3.0 * x ); }
double f22( double x ) { return( 3.0 * x - 1.0 ); }
double f23( double x ) { return( 3.0 * x - 2.0 ); }
double f24( double x ) { return( std::abs( 3.0 * x - 1.0 ) ); }
double f25( double x ) { return( std::abs( 3.0 * x - 2.0 ) ); }
double f26( double x ) { return( ( 3.0 * x - 1.0 ) / 2.0 ); }
double f27( double x ) { return( ( 3.0 * x - 2.0 ) / 2.0 ); }
double f28( double x ) { return( std::abs( ( 3.0 * x - 1.0 ) / 2.0 ) ); }
double f29( double x ) { return( std::abs( ( 3.0 * x - 2.0 ) / 2.0 ) ); }
double f30( double x ) { return( x / 0.32 - 0.78125 ); }
double f31( double x ) { return( 2.0 * x - 0.84 ); }
double f32( double x ) { return( x < 0.25 ? 4.0 * x : x < 0.50 ? 1.0 : x < 0.75 ? -2.0 * x + 1.84 : x / 0.08 - 11.5 ); }
double f33( double x ) { return( std::abs( 2.0f * x - 0.5 ) ); }
double f34( double x ) { return( 2.0 * x ); }
double f35( double x ) { return( 2.0 * x - 0.5 ); }
double f36( double x ) { return( 2.0 * x - 1.0 ); }
double f37( double x ) { return( x < 3.0/8 ? 0.0 : x < 5.0/8 ? 4.0 * x - 1.5 : x < 7.0/8 ? 1.0 : -4.0 * x + 4.5 ); }
double f38( double x ) { return( x < 1.0/8 ? 0.0 : x < 3.0/8 ? 4.0 * x - 0.5 : x < 5.0/8 ? 1.0 : x < 7.0/8 ? -4.0 * x + 3.5 : 0.0 ); }
double f39( double x ) { return( x < 1.0/8 ? 4.0 * x + 0.5 : x < 3.0/8 ? 1.0 : x < 5.0/8 ? -4.0 * x + 2.5 : 0.0 ); }
double f40( double x ) { return( x < 3.0/4 ? 7.0/8 * x : 11.0/8 * x - 3.0/8 ); }
double f41( double x ) { return( x < 3.0/8 ? 7.0/8 * x : x < 3.0/4 ? 29.0/24 * x - 1.0/8 : 7.0/8 * x + 1.0/8 ); }
double f42( double x ) { return( x < 3.0/8 ? 29.0/24 * x : 7.0/8 * x + 1.0/8 ); }

double (*f[])(double) = {
    f00, f01, f02, f03, f04, f05, f06, f07, f08, f09,
    f10, f11, f12, f13, f14, f15, f16, f17, f18, f19,
    f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
    f30, f31, f32, f33, f34, f35, f36, f37, f38, f39,
    f40, f41, f42
};

}

namespace kvs
{

namespace RGBFormulae
{

/*===========================================================================*/
/**
 *  @brief  Creates colormap.
 *  @param  index0 [in] index of formula for red component
 *  @param  index1 [in] index of formula for green component
 *  @param  index2 [in] index of formula for blue component
 *  @param  resolution [in] table resolution
 *  @return colormap
 */
/*===========================================================================*/
kvs::ColorMap Create( const int index0, const int index1, const int index2, const size_t resolution )
{
    int rindex = index0;
    int gindex = index1;
    int bindex = index2;

    bool rnegative = false;
    bool gnegative = false;
    bool bnegative = false;
    if ( rindex < 0 ) { rnegative = true; rindex *= -1; }
    if ( gindex < 0 ) { gnegative = true; gindex *= -1; }
    if ( bindex < 0 ) { bnegative = true; bindex *= -1; }

    rindex = kvs::Math::Clamp( rindex, 0, 42 );
    gindex = kvs::Math::Clamp( gindex, 0, 42 );
    bindex = kvs::Math::Clamp( bindex, 0, 42 );

    kvs::ColorMap::Table table( 3 * resolution );
    kvs::UInt8* color = table.data();
    for ( size_t i = 0; i < resolution; ++i )
    {
        const double x = static_cast<double>(i) / ( resolution - 1 );
        const double rx = rnegative ? 1.0 - x : x;
        const double gx = gnegative ? 1.0 - x : x;
        const double bx = bnegative ? 1.0 - x : x;

        const double r = kvs::Math::Clamp( ::f[rindex]( rx ), 0.0, 1.0 );
        const double g = kvs::Math::Clamp( ::f[gindex]( gx ), 0.0, 1.0 );
        const double b = kvs::Math::Clamp( ::f[bindex]( bx ), 0.0, 1.0 );

        *( color++ ) = static_cast<kvs::UInt8>( r * 255.0 + 0.5 );
        *( color++ ) = static_cast<kvs::UInt8>( g * 255.0 + 0.5 );
        *( color++ ) = static_cast<kvs::UInt8>( b * 255.0 + 0.5 );
    }

    kvs::Real32 min_value = 0.0f;
    kvs::Real32 max_value = static_cast<kvs::Real32>( resolution - 1 );
    return kvs::ColorMap( table, min_value, max_value );
}

} // end of namespace RGBFormulae

} // end of namespace kvs
