/*****************************************************************************/
/**
 *  @file   Point.cpp
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
#include "Point.h"


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new mesh point class.
 */
/*===========================================================================*/
Point::Point():
    m_latitude( 0.0f ),
    m_longitude( 0.0f ),
    m_height( 0.0f ),
    m_is_ocean( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new mesh point class.
 *  @param  latitude [in] latitude
 *  @param  longitude [in] longitude
 *  @param  height [in] height
 *  @param  is_ocean [in] true, if the input point is ocean
 */
/*===========================================================================*/
Point::Point( const float latitude, const float longitude, const float height, const bool is_ocean ):
    m_latitude( latitude ),
    m_longitude( longitude ),
    m_height( height ),
    m_is_ocean( is_ocean )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns latitude
 *  @return latitude
 */
/*===========================================================================*/
float Point::latitude() const
{
    return m_latitude;
}

/*===========================================================================*/
/**
 *  @brief  Returns longitude.
 *  @return longitude
 */
/*===========================================================================*/
float Point::longitude() const
{
    return m_longitude;
}

/*===========================================================================*/
/**
 *  @brief  Returns height.
 *  @return height
 */
/*===========================================================================*/
float Point::height() const
{
    return m_height;
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the point is ocean.
 *  @return true, if the point is ocean
 */
/*===========================================================================*/
bool Point::isOcean() const
{
    return m_is_ocean;
}

/*===========================================================================*/
/**
 *  @brief  Sets latitude.
 *  @param  latitude [in] latitude
 */
/*===========================================================================*/
void Point::setLatitude( const float latitude )
{
    m_latitude = latitude;
}

/*===========================================================================*/
/**
 *  @brief  Sets longitude.
 *  @param  longitude [in] longitude
 */
/*===========================================================================*/
void Point::setLongitude( const float longitude )
{
    m_longitude = longitude;
}

/*===========================================================================*/
/**
 *  @brief  Sets height.
 *  @param  height [in] height
 */
/*===========================================================================*/
void Point::setHeight( const float height )
{
    m_height = height;
}

/*===========================================================================*/
/**
 *  @brief  Sets true if the point is ocean.
 *  @param  is_ocean [in] true, if the point is ocean
 */
/*===========================================================================*/
void Point::setOcean( const bool is_ocean )
{
    m_is_ocean = is_ocean;
}

} // end of namespace gis

} // end of namespace kvs
