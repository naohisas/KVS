/*****************************************************************************/
/**
 *  @file   MshColor.h
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
 *  @brief  Msh (polar version of CIELAB) color class.
 */
/*===========================================================================*/
class MshColor
{
private:
    kvs::Real32 m_m; ///< magnitude of (L*, a*, b*)
    kvs::Real32 m_s; ///< saturation
    kvs::Real32 m_h; ///< hue

public:
    static kvs::MshColor Mix( const kvs::MshColor& msh1, const kvs::MshColor& msh2, const kvs::Real32 t );

public:
    MshColor( kvs::Real32 m, kvs::Real32 s, kvs::Real32 h );
    MshColor( const kvs::Vec3& msh );
    MshColor( const kvs::MshColor& msh );
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
