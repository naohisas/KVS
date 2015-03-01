/****************************************************************************/
/**
 *  @file   HSVColor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HSVColor.cpp 1316 2012-09-24 10:40:16Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "HSVColor.h"
#include "RGBColor.h"
#include <kvs/Math>


namespace
{

kvs::HSVColor RGB2HSV( const kvs::RGBColor& rgb )
{
    const kvs::Real32 R = kvs::Real32( rgb.r() / 255.0f );
    const kvs::Real32 G = kvs::Real32( rgb.g() / 255.0f );
    const kvs::Real32 B = kvs::Real32( rgb.b() / 255.0f );

    const kvs::Real32 min_rgb = kvs::Math::Min( R, G, B );
    const kvs::Real32 max_rgb = kvs::Math::Max( R, G, B );
    const kvs::Real32 delta = max_rgb - min_rgb;

    kvs::Real32 h = 0.0f;
    kvs::Real32 s = 0.0f;
    kvs::Real32 v = max_rgb;
    if ( !kvs::Math::IsZero( delta ) )
    {
        s = delta / max_rgb;

        kvs::Real32 delta_r = ( ( ( max_rgb - R ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
        kvs::Real32 delta_g = ( ( ( max_rgb - G ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
        kvs::Real32 delta_b = ( ( ( max_rgb - B ) / 6.0f ) + ( delta / 2.0f ) ) / delta;

        if ( kvs::Math::Equal( R, max_rgb ) ) h = delta_b - delta_g;
        else if ( kvs::Math::Equal( G, max_rgb ) ) h = ( 1.0f / 3.0f ) + delta_r - delta_b;
        else if ( kvs::Math::Equal( B, max_rgb ) ) h = ( 2.0f / 3.0f ) + delta_g - delta_r;

        if ( h < 0 ) h += 1;
        if ( h > 1 ) h -= 1;
    }

    return kvs::HSVColor( h, s, v );
}

kvs::RGBColor HSV2RGB( const kvs::HSVColor& hsv )
{
    kvs::UInt8 r, g, b;
    if ( kvs::Math::IsZero( hsv.s() ) )
    {
        r = static_cast<kvs::UInt8>( hsv.v() * 255 );
        g = static_cast<kvs::UInt8>( hsv.v() * 255 );
        b = static_cast<kvs::UInt8>( hsv.v() * 255 );
    }
    else
    {
        kvs::Real32 h = ( hsv.h() < 1.0f ? hsv.h() : hsv.h() - 1.0f ) * 6.0f;
        int i = int( h );

        kvs::Real32 tmp1 = hsv.v() * ( 1 - hsv.s() );
        kvs::Real32 tmp2 = hsv.v() * ( 1 - hsv.s() * ( h - i ) );
        kvs::Real32 tmp3 = hsv.v() * ( 1 - hsv.s() * ( 1 - h + i ) );

        kvs::Real32 tmp_r, tmp_g, tmp_b;
        switch ( i )
        {
        case 0:
        {
            tmp_r = hsv.v();
            tmp_g = tmp3;
            tmp_b = tmp1;
            break;
        }
        case 1:
        {
            tmp_r = tmp2;
            tmp_g = hsv.v();
            tmp_b = tmp1;
            break;
        }
        case 2:
        {
            tmp_r = tmp1;
            tmp_g = hsv.v();
            tmp_b = tmp3;
            break;
        }
        case 3:
        {
            tmp_r = tmp1;
            tmp_g = tmp2;
            tmp_b = hsv.v();
            break;
        }
        case 4:
        {
            tmp_r = tmp3;
            tmp_g = tmp1;
            tmp_b = hsv.v();
            break;
        }
        default:
        {
            tmp_r = hsv.v();
            tmp_g = tmp1;
            tmp_b = tmp2;
            break;
        }
        }

        r = static_cast<kvs::UInt8>( tmp_r * 255.0f + 0.5f );
        g = static_cast<kvs::UInt8>( tmp_g * 255.0f + 0.5f );
        b = static_cast<kvs::UInt8>( tmp_b * 255.0f + 0.5f );
    }

    return kvs::RGBColor( r, g, b );
}

}

namespace kvs
{

HSVColor HSVColor::Mix( const HSVColor& hsv1, const HSVColor& hsv2, const kvs::Real32 t )
{
    const kvs::Real32 h = kvs::Math::Mix( hsv1.h(), hsv2.h(), t );
    const kvs::Real32 s = kvs::Math::Mix( hsv1.s(), hsv2.s(), t );
    const kvs::Real32 v = kvs::Math::Mix( hsv1.v(), hsv2.v(), t );
    return HSVColor( h, s, v );
}

HSVColor::HSVColor( float h, float s, float v ):
    m_h( h ),
    m_s( s ),
    m_v( v )
{
}

HSVColor::HSVColor( const kvs::Vec3& hsv ):
    m_h( hsv[0] ),
    m_s( hsv[1] ),
    m_v( hsv[2] )
{
}

HSVColor::HSVColor( const HSVColor& hsv ):
    m_h( hsv.m_h ),
    m_s( hsv.m_s ),
    m_v( hsv.m_v )
{
}

HSVColor::HSVColor( const RGBColor& rgb )
{
    *this = ::RGB2HSV( rgb );
}

kvs::RGBColor HSVColor::toRGBColor() const
{
    return ::HSV2RGB( *this );
}

kvs::HSVColor& HSVColor::operator += ( const kvs::HSVColor& hsv )
{
    m_h += hsv.m_h;
    m_s += hsv.m_s;
    m_v += hsv.m_v;
    return *this;
}

kvs::HSVColor& HSVColor::operator -= ( const kvs::HSVColor& hsv )
{
    m_h -= hsv.m_h;
    m_s -= hsv.m_s;
    m_v -= hsv.m_v;
    return *this;
}

kvs::HSVColor& HSVColor::operator = ( const kvs::HSVColor& hsv )
{
    m_h = hsv.m_h;
    m_s = hsv.m_s;
    m_v = hsv.m_v;
    return *this;
}

kvs::HSVColor& HSVColor::operator = ( const kvs::RGBColor& rgb )
{
    *this = ::RGB2HSV( rgb );
    return *this;
}

kvs::HSVColor operator + ( const kvs::HSVColor& a, const kvs::HSVColor& b )
{
    kvs::HSVColor ret( a ); ret += b;
    return ret;
}

bool operator == ( const kvs::HSVColor& a, const kvs::HSVColor& b )
{
    return
        kvs::Math::Equal( a.h(), b.h() ) &&
        kvs::Math::Equal( a.s(), b.s() ) &&
        kvs::Math::Equal( a.v(), b.v() );
}

} // end of namespace kvs
