/*****************************************************************************/
/**
 *  @file   LabColor.cpp
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
#include "LabColor.h"
#include "RGBColor.h"
#include "XYZColor.h"
#include "MshColor.h"


namespace
{

//const kvs::Vec3 ReferenceWhite( 1.0, 1.0, 1.0 );
//const kvs::Vec3 ReferenceWhite( 0.964221, 1.0, 0.825211 ); // D50
const kvs::Vec3 ReferenceWhite( 0.95047f, 1.0f, 1.08883f ); // D65

kvs::Real32 f( const kvs::Real32 t )
{
    if ( t > 0.008856f ) { return std::pow( t, 1.0f / 3.0f ); }
    else { return 7.787037f * t + 16.0f / 116.0f; }
}

kvs::Real32 finv( const kvs::Real32 t )
{
    if ( t > 0.008856f ) { return std::pow( t, 3.0f ); }
    else { return ( t - 16.0f / 116.0f ) / 7.787037f; }
}

kvs::LabColor XYZ2Lab( const kvs::XYZColor& xyz )
{
    const kvs::Real32 Xn = ReferenceWhite.x();
    const kvs::Real32 Yn = ReferenceWhite.y();
    const kvs::Real32 Zn = ReferenceWhite.z();

    const kvs::Real32 X = xyz.x();
    const kvs::Real32 Y = xyz.y();
    const kvs::Real32 Z = xyz.z();

    const kvs::Real32 l = 116.0f * ( f( Y / Yn ) - 16.0f / 116.0f );
    const kvs::Real32 a = 500.0f * ( f( X / Xn ) - f( Y / Yn ) );
    const kvs::Real32 b = 200.0f * ( f( Y / Yn ) - f( Z / Zn ) );
    return kvs::LabColor( l, a, b );
}

kvs::XYZColor Lab2XYZ( const kvs::LabColor& lab )
{
    const kvs::Real32 Xn = ReferenceWhite.x();
    const kvs::Real32 Yn = ReferenceWhite.y();
    const kvs::Real32 Zn = ReferenceWhite.z();

    const kvs::Real32 l = lab.l();
    const kvs::Real32 a = lab.a();
    const kvs::Real32 b = lab.b();

    const kvs::Real32 y = ( l + 16.0f ) / 116.0f;
    const kvs::Real32 x = a / 500.0f + y;
    const kvs::Real32 z = y - b / 200.0f;

    const kvs::Real32 X = Xn * finv( x );
    const kvs::Real32 Y = Yn * finv( y );
    const kvs::Real32 Z = Zn * finv( z );
    return kvs::XYZColor( X, Y, Z );
}

/*===========================================================================*/
/**
 *  @brief  Converts color space from RGB to Lab.
 *  @param  rgb [in] RGB color
 *  @return Lab color
 */
/*===========================================================================*/
inline kvs::LabColor RGB2Lab( const kvs::RGBColor& rgb )
{
    return rgb.toXYZColor().toLabColor();
}

/*===========================================================================*/
/**
 *  @brief  Converts color space from Lab to RGB.
 *  @param  lab [in] Lab color
 *  @return RGB color
 */
/*===========================================================================*/
inline kvs::RGBColor Lab2RGB( const kvs::LabColor& lab )
{
    return lab.toXYZColor().toRGBColor();
}

}

namespace kvs
{

kvs::LabColor LabColor::Mix( const kvs::LabColor& lab1, const kvs::LabColor& lab2, const kvs::Real32 t )
{
    const kvs::Real32 l = kvs::Math::Mix( lab1.l(), lab2.l(), t );
    const kvs::Real32 a = kvs::Math::Mix( lab1.a(), lab2.a(), t );
    const kvs::Real32 b = kvs::Math::Mix( lab1.b(), lab2.b(), t );
    return kvs::LabColor( l, a, b );
}

LabColor::LabColor( kvs::Real32 l, kvs::Real32 a, kvs::Real32 b ):
    m_l( l ),
    m_a( a ),
    m_b( b )
{
}

LabColor::LabColor( const kvs::Vec3& lab ):
    m_l( lab[0] ),
    m_a( lab[1] ),
    m_b( lab[2] )
{
}

LabColor::LabColor( const kvs::LabColor& lab ):
    m_l( lab.l() ),
    m_a( lab.a() ),
    m_b( lab.b() )
{
}

LabColor::LabColor( const kvs::RGBColor& rgb )
{
    *this = ::RGB2Lab( rgb );
}

LabColor::LabColor( const kvs::XYZColor& xyz )
{
    *this = ::XYZ2Lab( xyz );
}

kvs::RGBColor LabColor::toRGBColor() const
{
    return ::Lab2RGB( *this );
}

kvs::XYZColor LabColor::toXYZColor() const
{
    return ::Lab2XYZ( *this );
}

kvs::MshColor LabColor::toMshColor() const
{
    return kvs::MshColor( *this );
}

kvs::LabColor& LabColor::operator += ( const kvs::LabColor& lab )
{
    m_l += lab.l();
    m_a += lab.a();
    m_b += lab.b();
    return *this;
}

kvs::LabColor& LabColor::operator -= ( const kvs::LabColor& lab )
{
    m_l -= lab.l();
    m_a -= lab.a();
    m_b -= lab.b();
    return *this;
}

kvs::LabColor& LabColor::operator = ( const kvs::LabColor& lab )
{
    m_l = lab.l();
    m_a = lab.a();
    m_b = lab.b();
    return *this;
}

kvs::LabColor& LabColor::operator = ( const kvs::XYZColor& xyz )
{
    *this = ::XYZ2Lab( xyz );
    return *this;
}

kvs::LabColor& LabColor::operator = ( const kvs::RGBColor& rgb )
{
    *this = ::RGB2Lab( rgb );
    return *this;
}

kvs::LabColor operator + ( const kvs::LabColor& a, const kvs::LabColor& b )
{
    kvs::LabColor ret( a ); ret += b;
    return ret;
}

bool operator == ( const kvs::LabColor& a, const kvs::LabColor& b )
{
    return
        kvs::Math::Equal( a.l(), b.l() ) &&
        kvs::Math::Equal( a.a(), b.a() ) &&
        kvs::Math::Equal( a.b(), b.b() );
}

} // end of namespace kvs
