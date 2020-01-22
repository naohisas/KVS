/****************************************************************************/
/**
 *  @file   RGBColor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RGBColor.cpp 1798 2014-08-04 05:25:10Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include <kvs/Math>
#include <kvs/Type>
#include <kvs/Assert>
#include "RGBColor.h"
#include "HSVColor.h"
#include "MshColor.h"
#include "RGBAColor.h"
#include "XYZColor.h"


namespace kvs
{

kvs::RGBColor RGBColor::Mix( const kvs::RGBColor& rgb1, const kvs::RGBColor& rgb2, const kvs::Real32 t )
{
    const kvs::Vec3 sRGB1 = rgb1.toVec3();
    const kvs::Vec3 sRGB2 = rgb2.toVec3();
    const kvs::Real32 sR = kvs::Math::Mix( sRGB1[0], sRGB2[0], t );
    const kvs::Real32 sG = kvs::Math::Mix( sRGB1[1], sRGB2[1], t );
    const kvs::Real32 sB = kvs::Math::Mix( sRGB1[2], sRGB2[2], t );
    return kvs::RGBColor( kvs::Vec3( sR, sG, sB ) );
}

RGBColor::RGBColor():
    m_r( 0 ),
    m_g( 0 ),
    m_b( 0 )
{
}

RGBColor::RGBColor( kvs::UInt8 r, kvs::UInt8 g, kvs::UInt8 b ):
    m_r( r ),
    m_g( g ),
    m_b( b )
{
}

RGBColor::RGBColor( const kvs::UInt8 rgb[3] ):
    m_r( rgb[0] ),
    m_g( rgb[1] ),
    m_b( rgb[2] )
{
}

RGBColor::RGBColor( const kvs::RGBColor& rgb ):
    m_r( rgb.m_r ),
    m_g( rgb.m_g ),
    m_b( rgb.m_b )
{
}

RGBColor::RGBColor( const kvs::HSVColor& hsv )
{
    *this = hsv.toRGBColor();
}

RGBColor::RGBColor( const kvs::MshColor& msh )
{
    *this = msh.toRGBColor();
}

RGBColor::RGBColor( const kvs::Vec3& rgb )
{
    *this = rgb;
}

RGBColor::RGBColor( const kvs::Vec3i& rgb )
{
    *this = rgb;
}

kvs::RGBColor& RGBColor::operator += ( const kvs::RGBColor& rgb )
{
    m_r += rgb.m_r;
    m_g += rgb.m_g;
    m_b += rgb.m_b;
    return *this;
}

kvs::RGBColor& RGBColor::operator -= ( const kvs::RGBColor& rgb )
{
    m_r -= rgb.m_r;
    m_g -= rgb.m_g;
    m_b -= rgb.m_b;
    return *this;
}

kvs::RGBColor& RGBColor::operator = ( const kvs::RGBColor& rgb )
{
    m_r = rgb.m_r;
    m_g = rgb.m_g;
    m_b = rgb.m_b;
    return *this;
}

kvs::RGBColor& RGBColor::operator = ( const kvs::RGBAColor& rgba )
{
    m_r = rgba.r();
    m_g = rgba.g();
    m_b = rgba.b();
    return *this;
}

RGBColor& RGBColor::operator = ( const HSVColor& hsv )
{
    *this = hsv.toRGBColor();
    return *this;
}

RGBColor& RGBColor::operator = ( const MshColor& msh )
{
    *this = msh.toRGBColor();
    return *this;
}

RGBColor& RGBColor::operator = ( const kvs::Vec3& rgb )
{
    const kvs::Real32 r = kvs::Math::Clamp( rgb.x(), 0.0f, 1.0f );
    const kvs::Real32 g = kvs::Math::Clamp( rgb.y(), 0.0f, 1.0f );
    const kvs::Real32 b = kvs::Math::Clamp( rgb.z(), 0.0f, 1.0f );
    m_r = kvs::Math::Round( r * 255.0f );
    m_g = kvs::Math::Round( g * 255.0f );
    m_b = kvs::Math::Round( b * 255.0f );
    return *this;
}

RGBColor& RGBColor::operator = ( const kvs::Vec3i& rgb )
{
    const int r = kvs::Math::Clamp( rgb.x(), 0, 255 );
    const int g = kvs::Math::Clamp( rgb.y(), 0, 255 );
    const int b = kvs::Math::Clamp( rgb.z(), 0, 255 );
    m_r = static_cast<kvs::UInt8>( r );
    m_g = static_cast<kvs::UInt8>( g );
    m_b = static_cast<kvs::UInt8>( b );
    return *this;
}

bool operator == ( const kvs::RGBColor& a, const kvs::RGBColor& b )
{
    return ( a.m_r == b.m_r ) && ( a.m_g == b.m_g ) && ( a.m_b == b.m_b );
}

kvs::RGBColor operator + ( const kvs::RGBColor& a, const kvs::RGBColor& b )
{
    kvs::RGBColor ret( a ); ret += b;
    return ret;
}

kvs::RGBColor operator - ( const kvs::RGBColor& a, const kvs::RGBColor& b )
{
    kvs::RGBColor ret( a ); ret -= b;
    return ret;
}

std::ostream& operator << ( std::ostream& os, const kvs::RGBColor& rgb )
{
    os << rgb.toVec3i();
    return os;
}

kvs::Vec3 RGBColor::toVec3() const
{
    const kvs::Real32 r = static_cast<kvs::Real32>( m_r ) / 255.0f;
    const kvs::Real32 g = static_cast<kvs::Real32>( m_g ) / 255.0f;
    const kvs::Real32 b = static_cast<kvs::Real32>( m_b ) / 255.0f;
    return kvs::Vec3( r, g, b );
}

kvs::Vec3i RGBColor::toVec3i() const
{
    const int r = static_cast<int>( m_r );
    const int g = static_cast<int>( m_g );
    const int b = static_cast<int>( m_b );
    return kvs::Vec3i( r, g, b );
}

kvs::HSVColor RGBColor::toHSVColor() const
{
    return kvs::HSVColor( *this );
}

kvs::XYZColor RGBColor::toXYZColor() const
{
    return kvs::XYZColor( *this );
}

kvs::MshColor RGBColor::toMshColor() const
{
    return kvs::MshColor( *this );
}

} // end of namespace kvs
