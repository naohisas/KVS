/*****************************************************************************/
/**
 *  @file   MshColor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Type>
#include <kvs/Vector3>


namespace kvs
{

class RGBColor;
class LabColor;

/*===========================================================================*/
/**
 *  @brief  Msh (polar version of CIELAB) color class.
 */
/*===========================================================================*/
class MshColor
{
private:
    kvs::Real32 m_m = 0.0f; ///< magnitude of (L*, a*, b*)
    kvs::Real32 m_s = 0.0f; ///< saturation
    kvs::Real32 m_h = 0.0f; ///< hue

public:
    static MshColor Mix( const MshColor& msh1, const MshColor& msh2, const kvs::Real32 t );

public:
    MshColor() = default;
    MshColor( kvs::Real32 m, kvs::Real32 s, kvs::Real32 h ): m_m( m ), m_s( s ), m_h( h ) {}
    MshColor( const kvs::Vec3& msh ): m_m( msh[0] ), m_s( msh[1] ), m_h( msh[2] ) {}
    MshColor( const kvs::MshColor& msh ): m_m( msh.m_m ), m_s( msh.m_s ), m_h( msh.m_h ) {}
    MshColor( const kvs::RGBColor& rgb );
    MshColor( const kvs::LabColor& lab );

    void set( kvs::Real32 m, kvs::Real32 s, kvs::Real32 h ) { m_m = m; m_s = s; m_h = h; }
    kvs::Real32 m() const { return m_m; }
    kvs::Real32 s() const { return m_s; }
    kvs::Real32 h() const { return m_h; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_m, m_s, m_h ); }
    kvs::RGBColor toRGBColor() const;
    kvs::LabColor toLabColor() const;

    kvs::MshColor& operator += ( const kvs::MshColor& msh );
    kvs::MshColor& operator -= ( const kvs::MshColor& msh );
    kvs::MshColor& operator = ( const kvs::MshColor& msh );
    kvs::MshColor& operator = ( const kvs::RGBColor& rgb );
    kvs::MshColor& operator = ( const kvs::LabColor& lab );
    friend kvs::MshColor operator + ( const kvs::MshColor& a, const kvs::MshColor& b );
    friend bool operator == ( const kvs::MshColor& a, const kvs::MshColor& b );
};

} // end of namespace kvs
