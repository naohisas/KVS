/****************************************************************************/
/**
 *  @file   RGBColor.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include <kvs/Math>
#include <kvs/Type>
#include <kvs/Assert>
#include "RGBColor.h"
#include "HCLColor.h"
#include "HSLColor.h"
#include "HSVColor.h"
#include "MshColor.h"
#include "LabColor.h"
#include "RGBAColor.h"
#include "XYZColor.h"


namespace kvs
{

RGBColor RGBColor::Mix( const RGBColor& rgb1, const RGBColor& rgb2, const kvs::Real32 t )
{
    const auto sRGB1 = rgb1.toVec3();
    const auto sRGB2 = rgb2.toVec3();
    const auto sR = kvs::Math::Mix( sRGB1[0], sRGB2[0], t );
    const auto sG = kvs::Math::Mix( sRGB1[1], sRGB2[1], t );
    const auto sB = kvs::Math::Mix( sRGB1[2], sRGB2[2], t );
    return { kvs::Vec3( sR, sG, sB ) };
}

RGBColor::RGBColor( const kvs::HSLColor& hsl )
{
    *this = hsl.toRGBColor();
}

RGBColor::RGBColor( const kvs::HSVColor& hsv )
{
    *this = hsv.toRGBColor();
}

RGBColor::RGBColor( const kvs::MshColor& msh )
{
    *this = msh.toRGBColor();
}

RGBColor::RGBColor( const kvs::HCLColor& hcl )
{
    *this = hcl.toRGBColor();
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

RGBColor& RGBColor::operator = ( const HSLColor& hsl )
{
    *this = hsl.toRGBColor();
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

RGBColor& RGBColor::operator = ( const HCLColor& hcl )
{
    *this = hcl.toRGBColor();
    return *this;
}

RGBColor& RGBColor::operator = ( const LabColor& lab )
{
    *this = lab.toRGBColor();
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
    return { static_cast<kvs::Real32>( m_r ) / 255.0f,
             static_cast<kvs::Real32>( m_g ) / 255.0f,
             static_cast<kvs::Real32>( m_b ) / 255.0f };
}

kvs::Vec3i RGBColor::toVec3i() const
{
    return { static_cast<int>( m_r ),
             static_cast<int>( m_g ),
             static_cast<int>( m_b ) };
}

kvs::HSLColor RGBColor::toHSLColor() const
{
    return kvs::HSLColor( *this );
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

kvs::HCLColor RGBColor::toHCLColor() const
{
    return kvs::HCLColor( *this );
}

} // end of namespace kvs
