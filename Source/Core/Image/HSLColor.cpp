#include "HSLColor.h"
#include "RGBColor.h"
#include <kvs/Math>


namespace
{

inline kvs::HSLColor RGB2HSL( const kvs::RGBColor& rgb )
{
    const auto R = kvs::Real32( rgb.r() / 255.0f );
    const auto G = kvs::Real32( rgb.g() / 255.0f );
    const auto B = kvs::Real32( rgb.b() / 255.0f );

    const auto min_rgb = kvs::Math::Min( R, G, B );
    const auto max_rgb = kvs::Math::Max( R, G, B );

    kvs::Real32 l = ( max_rgb + min_rgb ) * 0.5f;
    if ( kvs::Math::Equal( min_rgb, max_rgb ) ) { return { 0.0f, 0.0f, l }; }

    auto delta = max_rgb - min_rgb;
    auto s = ( l > 0.5f ) ?
        delta / ( 2.0f - max_rgb - min_rgb ) :
        delta / ( max_rgb + min_rgb );

    auto h = 0.0f;
    if ( kvs::Math::Equal( max_rgb, R ) )
    {
        h = ( G - B ) / delta + ( G < B ? 6.0f : 0.0f );
    }
    else if ( kvs::Math::Equal( max_rgb, G ) )
    {
        h = ( B - R ) / delta + 2.0f;
    }
    else if ( kvs::Math::Equal( max_rgb, B ) )
    {
        h = ( R - G ) / delta + 4.0f;
    }

    h /= 6.0f;

    return { h, s, l };
}

inline kvs::RGBColor HSL2RGB( const kvs::HSLColor& hsl )
{
    auto hue2rgb = [] ( float p, float q, float t )
    {
        if ( t < 0.0f ) { t += 1.0f; }
        if ( t > 1.0f ) { t -= 1.0f; }
        if ( t < 1.0f / 6.0f ) { return p + ( q - p ) * 6.0f * t; }
        if ( t < 1.0f / 2.0f ) { return q; }
        if ( t < 2.0f / 3.0f ) { return p + ( q - p ) * (2.0f / 3.0f - t ) * 6.0f; }
        return p;
    };

    if ( kvs::Math::IsZero( hsl.s() ) )
    {
        const auto L = kvs::UInt8( hsl.l() * 255.0f );
        return { L, L, L };
    }

    const auto h = hsl.h();
    const auto s = hsl.s();
    const auto l = hsl.l();

    const auto q = l < 0.5f ? l * ( 1.0f + s ) : l + s - l * s;
    const auto p = 2.0f * l - q;
    const auto R = kvs::UInt8( hue2rgb( p, q, h + 1.0f / 3.0f ) * 255.0f );
    const auto G = kvs::UInt8( hue2rgb( p, q, h ) * 255.0f );
    const auto B = kvs::UInt8( hue2rgb( p, q, h - 1.0f / 3.0f ) * 255.0f );
    return { R, G, B };
}

} // end of namespace


namespace kvs
{

HSLColor HSLColor::Mix( const HSLColor& hsl1, const HSLColor& hsl2, const kvs::Real32 t )
{
    const auto h = kvs::Math::Mix( hsl1.h(), hsl2.h(), t );
    const auto s = kvs::Math::Mix( hsl1.s(), hsl2.s(), t );
    const auto l = kvs::Math::Mix( hsl1.l(), hsl2.l(), t );
    return HSLColor( h, s, l );
}

HSLColor::HSLColor( const RGBColor& rgb )
{
    *this = ::RGB2HSL( rgb );
}

kvs::RGBColor HSLColor::toRGBColor() const
{
    return ::HSL2RGB( *this );
}

kvs::HSLColor& HSLColor::operator += ( const kvs::HSLColor& hsl )
{
    m_h += hsl.m_h;
    m_s += hsl.m_s;
    m_l += hsl.m_l;
    return *this;
}

kvs::HSLColor& HSLColor::operator -= ( const kvs::HSLColor& hsl )
{
    m_h -= hsl.m_h;
    m_s -= hsl.m_s;
    m_l -= hsl.m_l;
    return *this;
}

kvs::HSLColor& HSLColor::operator = ( const kvs::HSLColor& hsl )
{
    m_h = hsl.m_h;
    m_s = hsl.m_s;
    m_l = hsl.m_l;
    return *this;
}

kvs::HSLColor& HSLColor::operator = ( const kvs::RGBColor& rgb )
{
    *this = ::RGB2HSL( rgb );
    return *this;
}

kvs::HSLColor operator + ( const kvs::HSLColor& a, const kvs::HSLColor& b )
{
    kvs::HSLColor ret( a ); ret += b;
    return ret;
}

bool operator == ( const kvs::HSLColor& a, const kvs::HSLColor& b )
{
    return
        kvs::Math::Equal( a.h(), b.h() ) &&
        kvs::Math::Equal( a.s(), b.s() ) &&
        kvs::Math::Equal( a.l(), b.l() );
}

} // end of namespace kvs
