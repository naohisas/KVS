/*****************************************************************************/
/**
 *  @file   HCLColor.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <kvs/Type>
#include <kvs/Vector3>


namespace kvs
{
class LabColor;

/*===========================================================================*/
/**
 *  @brief  HCL color class
 */
/*===========================================================================*/
class HCLColor
{
private:
    kvs::Real32 m_h; ///< Hue angle [0-1]
    kvs::Real32 m_c; ///< Chroma [0-100]
    kvs::Real32 m_l; ///< Luminance [0-100]

public:
    HCLColor( kvs::Real32 h, kvs::Real32 c, kvs::Real32 l );
    HCLColor( const kvs::Vec3& hcl );
    HCLColor( const kvs::LabColor& lab );

    void set( kvs::Real32 h, kvs::Real32 c, kvs::Real32 l ) { m_h = h; m_c = c; m_l = l; }
    kvs::Real32 h() const { return m_h; }
    kvs::Real32 c() const { return m_c; }
    kvs::Real32 l() const { return m_l; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_h, m_c, m_l ); }
    kvs::LabColor toLabColor() const;

    kvs::HCLColor& operator += ( const kvs::HCLColor& hcl );
    kvs::HCLColor& operator -= ( const kvs::HCLColor& hcl );
    kvs::HCLColor& operator = ( const kvs::HCLColor& hcl );
    kvs::HCLColor& operator = ( const kvs::LabColor& lab );
    friend kvs::HCLColor operator + ( const kvs::HCLColor& a, const kvs::HCLColor& b );
    friend bool operator == ( const kvs::HCLColor& a, const kvs::HCLColor& b );
};

} // end of namespace kvs
