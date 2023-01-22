/*****************************************************************************/
/**
 *  @file   HCLColor.h
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
 *  @brief  HCL color class
 */
/*===========================================================================*/
class HCLColor
{
private:
    kvs::Real32 m_h = 0.0f; ///< Hue angle [0-1]
    kvs::Real32 m_c = 0.0f; ///< Chroma [0-100]
    kvs::Real32 m_l = 0.0f; ///< Luminance [0-100]

public:
    static HCLColor Mix( const HCLColor& hcl1, const HCLColor& hcl2, const kvs::Real32 t );

public:
    HCLColor() = default;
    HCLColor( kvs::Real32 h, kvs::Real32 c, kvs::Real32 l ): m_h( h ), m_c( c ), m_l( l ) {}
    HCLColor( const kvs::Vec3& hcl ): m_h( hcl[0] ), m_c( hcl[1] ), m_l( hcl[2] ) {}
    HCLColor( const kvs::HCLColor& hcl ): m_h( hcl.m_h ), m_c( hcl.m_c ), m_l( hcl.m_l ) {}
    HCLColor( const kvs::RGBColor& rgb );
    HCLColor( const kvs::LabColor& lab );

    void set( kvs::Real32 h, kvs::Real32 c, kvs::Real32 l ) { m_h = h; m_c = c; m_l = l; }
    kvs::Real32 h() const { return m_h; }
    kvs::Real32 c() const { return m_c; }
    kvs::Real32 l() const { return m_l; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_h, m_c, m_l ); }
    kvs::RGBColor toRGBColor() const;
    kvs::LabColor toLabColor() const;

    kvs::HCLColor& operator += ( const kvs::HCLColor& hcl );
    kvs::HCLColor& operator -= ( const kvs::HCLColor& hcl );
    kvs::HCLColor& operator = ( const kvs::HCLColor& hcl );
    kvs::HCLColor& operator = ( const kvs::RGBColor& rgb );
    kvs::HCLColor& operator = ( const kvs::LabColor& lab );
    friend kvs::HCLColor operator + ( const kvs::HCLColor& a, const kvs::HCLColor& b );
    friend bool operator == ( const kvs::HCLColor& a, const kvs::HCLColor& b );
};

} // end of namespace kvs
