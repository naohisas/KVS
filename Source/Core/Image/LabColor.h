/*****************************************************************************/
/**
 *  @file   LabColor.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Type>
#include <kvs/Vector3>


namespace kvs
{

class RGBColor;
class XYZColor;
class MshColor;
class HCLColor;

/*===========================================================================*/
/**
 *  @brief  CIELAB (L*a*b*) color class.
 */
/*===========================================================================*/
class LabColor
{
private:
    kvs::Real32 m_l = 0.0f; ///< L*: Lightness [0, 100]
    kvs::Real32 m_a = 0.0f; ///< a*: Red/Green value
    kvs::Real32 m_b = 0.0f; ///< b*: Blue/Yellow value

public:
    static LabColor Mix( const LabColor& lab1, const LabColor& lab2, const kvs::Real32 t );

public:
    LabColor() = default;
    LabColor( kvs::Real32 l, kvs::Real32 a, kvs::Real32 b ): m_l( l ), m_a( a ), m_b( b ) {}
    LabColor( const kvs::Vec3& lab ): m_l( lab[0] ), m_a( lab[1] ), m_b( lab[2] ) {}
    LabColor( const kvs::LabColor& lab ): m_l( lab.l() ), m_a( lab.a() ), m_b( lab.b() ) {}
    LabColor( const kvs::RGBColor& rgb );
    LabColor( const kvs::XYZColor& xyz );

    void set( kvs::Real32 l, kvs::Real32 a, kvs::Real32 b ) { m_l = l; m_a = a; m_b = b; }
    kvs::Real32 l() const { return m_l; }
    kvs::Real32 a() const { return m_a; }
    kvs::Real32 b() const { return m_b; }
    kvs::Vec3 toVec3() const { return kvs::Vec3( m_l, m_a, m_b ); }
    kvs::RGBColor toRGBColor() const;
    kvs::XYZColor toXYZColor() const;
    kvs::MshColor toMshColor() const;
    kvs::HCLColor toHCLColor() const;

    kvs::LabColor& operator += ( const kvs::LabColor& lab );
    kvs::LabColor& operator -= ( const kvs::LabColor& lab );
    kvs::LabColor& operator = ( const kvs::LabColor& lab );
    kvs::LabColor& operator = ( const kvs::RGBColor& rgb );
    kvs::LabColor& operator = ( const kvs::XYZColor& xyz );
    friend kvs::LabColor operator + ( const kvs::LabColor& a, const kvs::LabColor& b );
    friend bool operator == ( const kvs::LabColor& a, const kvs::LabColor& b );
};

} // end of namespace kvs
