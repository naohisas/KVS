/*****************************************************************************/
/**
 *  @file   Point.h
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
#ifndef KVS__GIS__POINT_H_INCLUDE
#define KVS__GIS__POINT_H_INCLUDE


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Grid point class.
 */
/*===========================================================================*/
class Point
{
private:

    float m_latitude; ///< latitude
    float m_longitude; ///< longitude
    float m_height; ///< height
    bool m_is_ocean; ///< check flag for the ocean point

public:

    Point();
    Point( const float latitude, const float longitude, const float height, const bool is_ocean = false );

public:

    float latitude() const;
    float longitude() const;
    float height() const;
    bool isOcean() const;

    void setLatitude( const float latitude );
    void setLongitude( const float longitude );
    void setHeight( const float height );
    void setOcean( const bool is_ocean = true );
};

} // end of namespace gis

} // end of namespace kvs

#endif // KVS__GIS__POINT_H_INCLUDE
