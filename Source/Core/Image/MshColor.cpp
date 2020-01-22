/*****************************************************************************/
/**
 *  @file   MshColor.cpp
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
#include "MshColor.h"
#include "RGBColor.h"
#include "LabColor.h"
#include "XYZColor.h"
#include <cmath>
#include <kvs/Vector2>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns angle difference in radian.
 *  @param  rad1 [in] angle 1 [rad]
 *  @param  rad2 [in] angle 2 [rad]
 *  @return angle difference
 */
/*===========================================================================*/
inline kvs::Real32 RadDiff( const kvs::Real32 rad1, const kvs::Real32 rad2 )
{
    const kvs::Vec2 v1( std::cos( rad1 ), std::sin( rad1 ) );
    const kvs::Vec2 v2( std::cos( rad2 ), std::sin( rad2 ) );
    return std::acos( v1.dot( v2 ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns an adjusted hue when interpolating to an unsaturated color in Msh color space.
 *  @param  msh_st [in] saturated Msh color
 *  @param  M_ust [in] unsaturated M value of Msh color
 *  @return adjusted hue value
 */
/*===========================================================================*/
inline kvs::Real32 AdjustHue(
    const kvs::MshColor& msh_st,
    const kvs::Real32 M_ust )
{
    const kvs::Real32 M_st = msh_st.m();
    const kvs::Real32 s_st = msh_st.s();
    const kvs::Real32 h_st = msh_st.h();

    if ( M_st >= M_ust - 0.1f )
    {
        return h_st;
    }
    else
    {
        const kvs::Real32 tmp1 = std::sqrt( M_ust * M_ust - M_st * M_st );
        const kvs::Real32 tmp2 = M_st * std::sin( s_st );
        const kvs::Real32 h_spin = s_st * tmp1 / tmp2;
        if ( h_st > -kvs::Math::PI() / 3.0 )
        {
            return h_st + h_spin;
        }
        else
        {
            return h_st - h_spin;
        }
    }
}

inline kvs::MshColor Lab2Msh( const kvs::LabColor& lab )
{
    const kvs::Real32 l = lab.l();
    const kvs::Real32 a = lab.a();
    const kvs::Real32 b = lab.b();

    const kvs::Real32 m = std::sqrt( l * l + a * a + b * b );
    const kvs::Real32 s = ( m > 0.001f ) * std::acos( l / m );
    const kvs::Real32 h = ( s > 0.001f ) * std::atan2( b, a );
    return kvs::MshColor( m, s, h );
}

inline kvs::LabColor Msh2Lab( const kvs::MshColor& msh )
{
    const kvs::Real32 m = msh.m();
    const kvs::Real32 s = msh.s();
    const kvs::Real32 h = msh.h();

    const kvs::Real32 l = m * std::cos( s );
    const kvs::Real32 a = m * std::sin( s ) * std::cos( h );
    const kvs::Real32 b = m * std::sin( s ) * std::sin( h );
    return kvs::LabColor( l, a, b );
}

/*===========================================================================*/
/**
 *  @brief  Converts color space from RGB to Msh.
 *  @param  rgb [in] RGB color
 *  @return Msh color
 */
/*===========================================================================*/
inline kvs::MshColor RGB2Msh( const kvs::RGBColor& rgb )
{
    return rgb.toXYZColor().toLabColor().toMshColor();
}

/*===========================================================================*/
/**
 *  @brief  Converts color space from Msh to RGB.
 *  @param  msh [in] Msh color
 *  @return RGB color
 */
/*===========================================================================*/
inline kvs::RGBColor Msh2RGB( const kvs::MshColor& msh )
{
    return msh.toLabColor().toXYZColor().toRGBColor();
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Interpolated msh color in Msh color space.
 *  @param  msh1 [in] Msh color #1
 *  @param  msh2 [in] Msh color #2
 *  @param  t [in] ratio
 *  @return interpolated Msh color
 */
/*===========================================================================*/
kvs::MshColor MshColor::Mix(
    const kvs::MshColor& msh1,
    const kvs::MshColor& msh2,
    const kvs::Real32 t )
{
    kvs::Real32 M1 = msh1.m();
    kvs::Real32 s1 = msh1.s();
    kvs::Real32 h1 = msh1.h();

    kvs::Real32 M2 = msh2.m();
    kvs::Real32 s2 = msh2.s();
    kvs::Real32 h2 = msh2.h();

    kvs::Real32 ratio = t;
    if ( ( s1 > 0.05f ) && ( s2 > 0.05f ) && ( ::RadDiff( h1, h2 ) > kvs::Math::PI() / 3.0f ) )
    {
        const kvs::Real32 Mmid = kvs::Math::Max( M1, M2, 88.0f );
        if ( t < 0.5f )
        {
            M2 = Mmid;
            s2 = 0.0;
            h2 = 0.0;
            ratio = 2.0f * ratio;
        }
        else
        {
            M1 = Mmid;
            s1 = 0.0;
            h1 = 0.0;
            ratio = 2.0f * ratio - 1.0f;
        }
    }

    if ( ( s1 < 0.05f ) && ( s2 > 0.05f ) )
    {
        h1 = ::AdjustHue( kvs::MshColor( M2, s2, h2 ), M1 );
    }
    else if ( ( s2 < 0.05f ) && ( s1 > 0.05f ) )
    {
        h2 = ::AdjustHue( kvs::MshColor( M1, s1, h1 ), M2 );
    }

    const kvs::Real32 M = kvs::Math::Mix( M1, M2, ratio );
    const kvs::Real32 s = kvs::Math::Mix( s1, s2, ratio );
    const kvs::Real32 h = kvs::Math::Mix( h1, h2, ratio );
    return kvs::MshColor( M, s, h );
}

MshColor::MshColor( kvs::Real32 m, kvs::Real32 s, kvs::Real32 h ):
    m_m( m ),
    m_s( s ),
    m_h( h )
{
}

MshColor::MshColor( const kvs::Vec3& msh ):
    m_m( msh[0] ),
    m_s( msh[1] ),
    m_h( msh[2] )
{
}

MshColor::MshColor( const kvs::MshColor& msh ):
    m_m( msh.m() ),
    m_s( msh.s() ),
    m_h( msh.h() )
{
}

MshColor::MshColor( const kvs::LabColor& lab )
{
    *this = ::Lab2Msh( lab );
}

MshColor::MshColor( const kvs::RGBColor& rgb )
{
    *this = ::RGB2Msh( rgb );
}

kvs::RGBColor MshColor::toRGBColor() const
{
    return ::Msh2RGB( *this );
}

kvs::LabColor MshColor::toLabColor() const
{
    return ::Msh2Lab( *this );
}

kvs::MshColor& MshColor::operator += ( const kvs::MshColor& msh )
{
    m_m += msh.m();
    m_s += msh.s();
    m_h += msh.h();
    return *this;
}

kvs::MshColor& MshColor::operator -= ( const kvs::MshColor& msh )
{
    m_m -= msh.m();
    m_s -= msh.s();
    m_h -= msh.h();
    return *this;
}

kvs::MshColor& MshColor::operator = ( const kvs::MshColor& msh )
{
    m_m = msh.m();
    m_s = msh.s();
    m_h = msh.h();
    return *this;
}

kvs::MshColor& MshColor::operator = ( const kvs::RGBColor& rgb )
{
    *this = ::RGB2Msh( rgb );
    return *this;
}

kvs::MshColor& MshColor::operator = ( const kvs::LabColor& lab )
{
    *this = ::Lab2Msh( lab );
    return *this;
}

kvs::MshColor operator + ( const kvs::MshColor& a, const kvs::MshColor& b )
{
    kvs::MshColor ret( a ); ret += b;
    return ret;
}

bool operator == ( const kvs::MshColor& a, const kvs::MshColor& b )
{
    return
        kvs::Math::Equal( a.m(), b.m() ) &&
        kvs::Math::Equal( a.s(), b.s() ) &&
        kvs::Math::Equal( a.h(), b.h() );
}

} // end of namespace kvs
