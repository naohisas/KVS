/*****************************************************************************/
/**
 *  @file   HCLColor.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "HCLColor.h"
#include <kvs/RGBColor>
#include <kvs/LabColor>
#include <kvs/XYZColor>
#include <kvs/Math>
#include <cmath>


namespace
{

inline kvs::Real32 Rad2Deg( const kvs::Real32 rad )
{
    kvs::Real32 deg = 0.0f;
    if ( rad > 0.0f )
    {
        deg = ( rad / kvs::Math::PI() ) * 180.0f;
    }
    else
    {
        deg = 360.0f - ( kvs::Math::Abs( rad ) / kvs::Math::PI() ) * 180.0f;
    }
    return deg;
}

inline kvs::Real32 Deg( const kvs::Real32 deg )
{
    if ( deg < 0.0f ) { return deg + 360.0f; }
    else if ( deg >= 360.0f ) { return deg - 360.0f; }
    else { return deg; }
}

inline kvs::HCLColor Lab2HCL( const kvs::LabColor& lab )
{
    const kvs::Real32 l = lab.l();
    const kvs::Real32 c = std::sqrt( lab.a() * lab.a() + lab.b() * lab.b() );
    const kvs::Real32 h = Deg( Rad2Deg( std::atan2( lab.b(), lab.a() ) ) );
    return kvs::HCLColor( h, c, l );
}

inline kvs::LabColor HCL2Lab( const kvs::HCLColor& hcl )
{
    const kvs::Real32 l = hcl.l();
    const kvs::Real32 theta = hcl.h() * 2.0f * kvs::Math::PI();
    const kvs::Real32 a = std::cos( theta ) * hcl.c();
    const kvs::Real32 b = std::sin( theta ) * hcl.c();
    return kvs::LabColor( l, a, b );
}

inline kvs::HCLColor RGB2HCL( const kvs::RGBColor& rgb )
{
    return rgb.toXYZColor().toLabColor().toHCLColor();
}

inline kvs::RGBColor HCL2RGB( const kvs::HCLColor& hcl )
{
    return hcl.toLabColor().toXYZColor().toRGBColor();
}

}

namespace kvs
{

HCLColor HCLColor::Mix( const HCLColor& hcl1, const HCLColor& hcl2, const kvs::Real32 t )
{
    auto hue_mix = [&] ( float h1, float h2, float a )
    {
        float d = h2 - h1;
        if ( h1 > h2 ) { std::swap( h1, h2 ); d = -d; a = 1 - a; }

        if ( d > 0.5f )
        {
            h1 = h1 + 1.0f;
            const auto h = h1 + a * ( h2 - h1 );
            return h - static_cast<int>( h );
        }
        return h1 + a * d;
    };

    const auto h = hue_mix( hcl1.h(), hcl2.h(), t );
    const auto c = kvs::Math::Mix( hcl1.c(), hcl2.c(), t );
    const auto l = kvs::Math::Mix( hcl1.l(), hcl2.l(), t );
    return { h, c, l };
}

HCLColor::HCLColor( const kvs::LabColor& lab )
{
    *this = ::Lab2HCL( lab );
}

HCLColor::HCLColor( const kvs::RGBColor& rgb )
{
    *this = ::RGB2HCL( rgb );
}

kvs::RGBColor HCLColor::toRGBColor() const
{
    return ::HCL2RGB( *this );
}

kvs::LabColor HCLColor::toLabColor() const
{
    return ::HCL2Lab( *this );
}

kvs::HCLColor& HCLColor::operator += ( const kvs::HCLColor& hcl )
{
    m_h += hcl.h();
    m_c += hcl.c();
    m_l += hcl.l();
    return *this;
}

kvs::HCLColor& HCLColor::operator -= ( const kvs::HCLColor& hcl )
{
    m_h -= hcl.h();
    m_c -= hcl.c();
    m_l -= hcl.l();
    return *this;
}

kvs::HCLColor& HCLColor::operator = ( const kvs::HCLColor& hcl )
{
    m_h = hcl.h();
    m_c = hcl.c();
    m_l = hcl.l();
    return *this;
}

kvs::HCLColor& HCLColor::operator = ( const kvs::RGBColor& rgb )
{
    *this = ::RGB2HCL( rgb );
    return *this;
}

kvs::HCLColor& HCLColor::operator = ( const kvs::LabColor& lab )
{
    *this = ::Lab2HCL( lab );
    return *this;
}

kvs::HCLColor operator + ( const kvs::HCLColor& a, const kvs::HCLColor& b )
{
    kvs::HCLColor ret( a ); ret += b;
    return ret;
}

bool operator == ( const kvs::HCLColor& a, const kvs::HCLColor& b )
{
    return
        kvs::Math::Equal( a.h(), b.h() ) &&
        kvs::Math::Equal( a.c(), b.c() ) &&
        kvs::Math::Equal( a.l(), b.l() );
}

} // end of namespace kvs
