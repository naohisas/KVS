/*****************************************************************************/
/**
 *  @file   XYZColor.h
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
 *  @brief  CIEXYZ color class.
 */
/*===========================================================================*/
class XYZColor
{
private:
    kvs::Real32 m_x = 0.0f; ///< [0-1]
    kvs::Real32 m_y = 0.0f; ///< [0-1]
    kvs::Real32 m_z = 0.0f; ///< [0-1]

public:
    XYZColor() = default;
    XYZColor( kvs::Real32 x, kvs::Real32 y, kvs::Real32 z ): m_x( x ), m_y( y ), m_z( z ) {}
    XYZColor( const kvs::Vec3& xyz ): m_x( xyz[0] ), m_y( xyz[1] ), m_z( xyz[2] ) {}
    XYZColor( const kvs::XYZColor& xyz ): m_x( xyz.x() ), m_y( xyz.y() ), m_z( xyz.z() ) {}
    XYZColor( const kvs::RGBColor& rgb );

    void set( kvs::Real32 x, kvs::Real32 y, kvs::Real32 z ) { m_x = x; m_y = y; m_z = z; }
    kvs::Real32 x() const { return m_x; }
    kvs::Real32 y() const { return m_y; }
    kvs::Real32 z() const { return m_z; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_x, m_y, m_z ); }
    kvs::RGBColor toRGBColor() const;
    kvs::LabColor toLabColor() const;

    kvs::XYZColor& operator += ( const kvs::XYZColor& xyz );
    kvs::XYZColor& operator -= ( const kvs::XYZColor& xyz );
    kvs::XYZColor& operator = ( const kvs::XYZColor& xyz );
    kvs::XYZColor& operator = ( const kvs::RGBColor& rgb );
    friend kvs::XYZColor operator + ( const kvs::XYZColor& a, const kvs::XYZColor& b );
    friend bool operator == ( const kvs::XYZColor& a, const kvs::XYZColor& b );
};

} // end of namespace kvs
