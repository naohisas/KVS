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
/*----------------------------------------------------------------------------
 *  Reference:
 *  [1] Kenneth Moreland, "Diverging Color Maps for Scientific Visualization",
 *      In Proceedings of the 5th International Symposium on Visual Computing,
 *      December 2009. DOI 10.1007/978-3-642-10520-3_9.
 */
/*****************************************************************************/
#include "MshColor.h"
#include "LabColor.h"
#include <cmath>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns an adjusted hue when interpolating to un unsaturated color in Msh color space.
 *  @param  msh_st [in] saturated Msh color
 *  @param  M_ust [in] unsaturated M value of Msh color
 *  @return adjusted hue value
 */
/*===========================================================================*/
kvs::Real32 AdjustHue(
    const kvs::MshColor& msh_st,
    const kvs::Real32 M_ust )
{
    const kvs::Real32 M_st = msh_st.m();
    const kvs::Real32 s_st = msh_st.s();
    const kvs::Real32 h_st = msh_st.h();

    if ( M_st >= M_ust )
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

kvs::MshColor Lab2Msh( const kvs::LabColor& lab )
{
    const kvs::Real32 l = lab.l();
    const kvs::Real32 a = lab.a();
    const kvs::Real32 b = lab.b();

    const kvs::Real32 m = std::sqrt( l * l + a * a + b * b );
    const kvs::Real32 s = ( m > 0.00001 ) * std::acos( l / m );
    const kvs::Real32 h = ( s > 0.00001 ) * std::atan2( b, a );
    return kvs::MshColor( m, s, h );
}

kvs::LabColor Msh2Lab( const kvs::MshColor& msh )
{
    const kvs::Real32 m = msh.m();
    const kvs::Real32 s = msh.s();
    const kvs::Real32 h = msh.h();

    const kvs::Real32 l = m * std::cos( s );
    const kvs::Real32 a = m * std::sin( s ) * std::cos( h );
    const kvs::Real32 b = m * std::sin( s ) * std::sin( h );
    return kvs::LabColor( l, a, b );
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
    if ( ( s1 > 0.05 ) && ( s2 > 0.05 ) && ( kvs::Math::Abs( h1 - h2 ) > kvs::Math::PI() / 3.0 ) )
    {
        const kvs::Real32 Mmid = kvs::Math::Max( M1, M2, 88.0f );
        if ( t < 0.5 )
        {
            M2 = Mmid;
            s2 = 0.0;
            h2 = 0.0;
            ratio = 2.0 * ratio;
        }
        else
        {
            M1 = Mmid;
            s1 = 0.0;
            h1 = 0.0;
            ratio = 2.0 * ratio - 1.0;
        }

    }

    if ( ( s1 < 0.05 ) && ( s2 > 0.05 ) )
    {
        h1 = ::AdjustHue( kvs::MshColor( M2, s2, h2 ), M1 );
    }
    else if ( ( s2 < 0.05 ) && ( s1 > 0.05 ) )
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
