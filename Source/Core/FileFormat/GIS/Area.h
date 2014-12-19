/*****************************************************************************/
/**
 *  @file   Area.h
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
#ifndef KVS__GIS__AREA_H_INCLUDE
#define KVS__GIS__AREA_H_INCLUDE

#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Mesh area class.
 */
/*===========================================================================*/
class Area
{
private:

    float m_min_latitude; ///< min. latitude
    float m_max_latitude; ///< max. latitude
    float m_min_longitude; ///< min. longitude
    float m_max_longitude; ///< max. longitude
    float m_min_height; ///< min. height
    float m_max_height; ///< max. height

public:

    Area();
    Area(
        const float min_latitude,
        const float max_latitude,
        const float min_longitude,
        const float max_longitude,
        const float min_height = 0.0f,
        const float max_height = 0.0f );

    float minLatitude() const;
    float maxLatitude() const;
    float minLongitude() const;
    float maxLongitude() const;
    float minHeight() const;
    float maxHeight() const;

    void setMinLatitude( const float min_latitude );
    void setMaxLatitude( const float max_latitude );
    void setMinLongitude( const float min_longitude );
    void setMaxLongitude( const float max_longitude );
    void setMinHeight( const float min_height );
    void setMaxHeight( const float max_height );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
};

} // end of namespace gis

} // end of namespace kvs

#endif // KVS__GIS__AREA_H_INCLUDE
