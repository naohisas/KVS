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
#include "XYZColor.h"
#include "MshColor.h"


namespace
{

//const kvs::Vec3 ReferenceWhite( 1.0, 1.0, 1.0 );
//const kvs::Vec3 ReferenceWhite( 0.964221, 1.0, 0.825211 ); // D50
const kvs::Vec3 ReferenceWhite( 0.95047, 1.0, 1.08883 ); // D65

kvs::Real32 f( const kvs::Real32 t )
{
    if ( t > 0.008856 ) { return std::pow( t, 1.0f / 3.0f ); }
    else { return 7.787037 * t + 16.0 / 116.0; }
}

kvs::Real32 finv( const kvs::Real32 t )
{
    if ( t > 0.008856 ) { return std::pow( t, 3.0f ); }
    else { return ( t - 16.0 / 116.0 ) / 7.787037; }
}

kvs::LabColor XYZ2Lab( const kvs::XYZColor& xyz )
{
    const kvs::Real32 Xn = ReferenceWhite.x();
    const kvs::Real32 Yn = ReferenceWhite.y();
    const kvs::Real32 Zn = ReferenceWhite.z();

    const kvs::Real32 X = xyz.x();
    const kvs::Real32 Y = xyz.y();
    const kvs::Real32 Z = xyz.z();

    const kvs::Real32 l = 116.0 * ( f( Y / Yn ) - 16.0 / 116.0 );
    const kvs::Real32 a = 500.0 * ( f( X / Xn ) - f( Y / Yn ) );
    const kvs::Real32 b = 200.0 * ( f( Y / Yn ) - f( Z / Zn ) );
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

    const kvs::Real32 y = ( l + 16.0 ) / 116.0;
    const kvs::Real32 x = a / 500.0 + y;
    const kvs::Real32 z = y - b / 200.0;

    const kvs::Real32 X = Xn * finv( x );
    const kvs::Real32 Y = Yn * finv( y );
    const kvs::Real32 Z = Zn * finv( z );
    return kvs::XYZColor( X, Y, Z );
}

}

namespace kvs
{

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

LabColor::LabColor( const kvs::XYZColor& xyz )
{
    *this = ::XYZ2Lab( xyz );
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
