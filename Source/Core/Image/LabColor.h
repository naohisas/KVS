/*****************************************************************************/
/**
 *  @file   LabColor.h
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
#pragma once
#include <kvs/Type>
#include <kvs/Vector3>


namespace kvs
{

class RGBColor;
class XYZColor;
class MshColor;

/*===========================================================================*/
/**
 *  @brief  CIELAB (L*a*b*) color class.
 */
/*===========================================================================*/
class LabColor
{
private:
    kvs::Real32 m_l; ///< L*
    kvs::Real32 m_a; ///< a*
    kvs::Real32 m_b; ///< b*

public:
    static kvs::LabColor Mix( const kvs::LabColor& lab1, const kvs::LabColor& lab2, const kvs::Real32 t );

public:
    LabColor( kvs::Real32 l, kvs::Real32 a, kvs::Real32 b );
    LabColor( const kvs::Vec3& lab );
    LabColor( const kvs::LabColor& lab );
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

    kvs::LabColor& operator += ( const kvs::LabColor& lab );
    kvs::LabColor& operator -= ( const kvs::LabColor& lab );
    kvs::LabColor& operator = ( const kvs::LabColor& lab );
    kvs::LabColor& operator = ( const kvs::RGBColor& rgb );
    kvs::LabColor& operator = ( const kvs::XYZColor& xyz );
    friend kvs::LabColor operator + ( const kvs::LabColor& a, const kvs::LabColor& b );
    friend bool operator == ( const kvs::LabColor& a, const kvs::LabColor& b );
};

} // end of namespace kvs
