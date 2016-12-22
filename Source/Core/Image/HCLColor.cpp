/*****************************************************************************/
/**
 *  @file   HCLColor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  $Id$
 */
/*****************************************************************************/
#include "HCLColor.h"
#include <kvs/LabColor>
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

}

namespace kvs
{

HCLColor::HCLColor( kvs::Real32 h, kvs::Real32 c, kvs::Real32 l ):
    m_h( h ),
    m_c( c ),
    m_l( l )
{
}

HCLColor::HCLColor( const kvs::Vec3& hcl ):
    m_h( hcl[0] ),
    m_c( hcl[1] ),
    m_l( hcl[2] )
{
}

HCLColor::HCLColor( const kvs::LabColor& lab )
{
    *this = ::Lab2HCL( lab );
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
