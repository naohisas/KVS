/*****************************************************************************/
/**
 *  @file   XYZColor.h
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
class LabColor;

/*===========================================================================*/
/**
 *  @brief  CIEXYZ color class.
 */
/*===========================================================================*/
class XYZColor
{
private:
    kvs::Real32 m_x; ///< [0-1]
    kvs::Real32 m_y; ///< [0-1]
    kvs::Real32 m_z; ///< [0-1]

public:
    XYZColor( kvs::Real32 x, kvs::Real32 y, kvs::Real32 z );
    XYZColor( const kvs::Vec3& xyz );
    XYZColor( const kvs::XYZColor& xyz );
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
