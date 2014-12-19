/*****************************************************************************/
/**
 *  @file   Area.cpp
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
#include "Area.h"
#include <string>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Area class.
 */
/*===========================================================================*/
Area::Area():
    m_min_latitude( 0.0f ),
    m_max_latitude( 0.0f ),
    m_min_longitude( 0.0f ),
    m_max_longitude( 0.0f ),
    m_min_height( 0.0f ),
    m_max_height( 0.0f )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Area class.
 *  @param  min_latitude [in] minimum latitude
 *  @param  max_latitude [in] maximum latitude
 *  @param  min_longitude [in] minimum longitude
 *  @param  max_longitude [in] maximum longitude
 *  @param  min_height [in] minimum height
 *  @param  max_height [in] maximum height
 */
/*===========================================================================*/
Area::Area(
    const float min_latitude,
    const float max_latitude,
    const float min_longitude,
    const float max_longitude,
    const float min_height,
    const float max_height ):
    m_min_latitude( min_latitude ),
    m_max_latitude( max_latitude ),
    m_min_longitude( min_longitude ),
    m_max_longitude( max_longitude ),
    m_min_height( min_height ),
    m_max_height( max_height )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the minimum latitude.
 *  @return minimum latitude
 */
/*===========================================================================*/
float Area::minLatitude() const
{
    return m_min_latitude;
}

/*===========================================================================*/
/**
 *  @brief  Returns the maximum latitude.
 *  @return maximum latitude
 */
/*===========================================================================*/
float Area::maxLatitude() const
{
    return m_max_latitude;
}

/*===========================================================================*/
/**
 *  @brief  Returns the minimum longitude.
 *  @return minimum longitude
 */
/*===========================================================================*/
float Area::minLongitude() const
{
    return m_min_longitude;
}

/*===========================================================================*/
/**
 *  @brief  Returns the maximum longitude.
 *  @return maximum longitude
 */
/*===========================================================================*/
float Area::maxLongitude() const
{
    return m_max_longitude;
}

/*===========================================================================*/
/**
 *  @brief  Returns the minimum height.
 *  @return minimum height
 */
/*===========================================================================*/
float Area::minHeight() const
{
    return m_min_height;
}

/*===========================================================================*/
/**
 *  @brief  Returns the maximum height
 *  @return maximum height
 */
/*===========================================================================*/
float Area::maxHeight() const
{
    return m_max_height;
}

/*===========================================================================*/
/**
 *  @brief  Sets a minimum latitude.
 *  @param  min_latitude [in] minimum latitude
 */
/*===========================================================================*/
void Area::setMinLatitude( const float min_latitude )
{
    m_min_latitude = min_latitude;
}

/*===========================================================================*/
/**
 *  @brief  Sets a maximum latitude.
 *  @param  max_latitude [in] maximum latitude
 */
/*===========================================================================*/
void Area::setMaxLatitude( const float max_latitude )
{
    m_max_latitude = max_latitude;
}

/*===========================================================================*/
/**
 *  @brief  Sets a minimum longitude.
 *  @param  min_longitude [in] minimum longitude
 */
/*===========================================================================*/
void Area::setMinLongitude( const float min_longitude )
{
    m_min_longitude = min_longitude;
}

/*===========================================================================*/
/**
 *  @brief  Sets a maximum longitude.
 *  @param  max_longitude [in] maximum longitude
 */
/*===========================================================================*/
void Area::setMaxLongitude( const float max_longitude )
{
    m_max_longitude = max_longitude;
}

/*===========================================================================*/
/**
 *  @brief  Sets a minimum height.
 *  @param  min_height [in] minimum height
 */
/*===========================================================================*/
void Area::setMinHeight( const float min_height )
{
    m_min_height = min_height;
}

/*===========================================================================*/
/**
 *  @brief  Sets a maximum height.
 *  @param  max_height [in] maximum height
 */
/*===========================================================================*/
void Area::setMaxHeight( const float max_height )
{
    m_max_height = max_height;
}

void Area::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Min. latitude : " << m_min_latitude << std::endl;
    os << indent << "Max. latitude : " << m_max_latitude << std::endl;
    os << indent << "Min. longitude : " << m_min_longitude << std::endl;
    os << indent << "Max. longitude : " << m_max_longitude << std::endl;
    os << indent << "Min. height : " << m_min_height << std::endl;
    os << indent << "Max. height : " << m_max_height << std::endl;
}

} // end of namespace gis

} // end of namespace kvs
