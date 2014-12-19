/*****************************************************************************/
/**
 *  @file   Mesh.cpp
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
#include "Mesh.h"
#include <kvs/Math>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Mesh class.
 */
/*===========================================================================*/
Mesh::Mesh()
{
    m_data = NULL;
    m_row = 0;
    m_column = 0;
    m_latitude_dimension = 0;
    m_longitude_dimension = 0;
    m_latitude_interval = 0.0f;
    m_longitude_interval = 0.0f;
    m_area = kvs::gis::Area( 0.0f, 0.0f, 0.0f, 0.0f, 9999.0f, 0.0f );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Mesh class.
 */
/*===========================================================================*/
Mesh::~Mesh()
{
    this->deallocate_data();
}

/*===========================================================================*/
/**
 *  @brief  Returns mesh data.
 *  @param  latitude [in] latitude
 *  @param  longitude [in] longitude
 */
/*===========================================================================*/
const kvs::gis::Point& Mesh::data( const size_t latitude, const size_t longitude ) const
{
    return m_data[ latitude ][ longitude ];
}

/*===========================================================================*/
/**
 *  @brief  Returns number of rows.
 *  @return number of rows
 */
/*===========================================================================*/
size_t Mesh::row() const
{
    return m_row;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of columns.
 *  @return number of columns
 */
/*===========================================================================*/
size_t Mesh::column() const
{
    return m_column;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of dimensions for latitude.
 *  @return number of dimensions for latitude
 */
/*===========================================================================*/
size_t Mesh::latitudeDimension() const
{
    return m_latitude_dimension;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of dimensions for longitude.
 *  @return number of dimensions for longitude
 */
/*===========================================================================*/
size_t Mesh::longitudeDimension() const
{
    return m_longitude_dimension;
}

/*===========================================================================*/
/**
 *  @brief  Returns interval for latitude.
 *  @return interval for latitude
 */
/*===========================================================================*/
float Mesh::latitudeInterval() const
{
    return m_latitude_interval;
}

/*===========================================================================*/
/**
 *  @brief  Returns interval for longitude.
 *  @return interval for longitude
 */
/*===========================================================================*/
float Mesh::longitudeInterval() const
{
    return m_longitude_interval;
}

/*===========================================================================*/
/**
 *  @brief  Returns area of the mesh.
 *  @return area of the mesh
 */
/*===========================================================================*/
const kvs::gis::Area& Mesh::area() const
{
    return m_area;
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the mesh has data values.
 *  @return true if the mesh has data values
 */
/*===========================================================================*/
bool Mesh::hasData() const
{
    return m_data != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Sets a data at the specified point.
 *  @param  latitude [in] latitude
 *  @param  longitude [in] longitude
 *  @param  data [in] data
 */
/*===========================================================================*/
void Mesh::setData( const size_t latitude, const size_t longitude, const kvs::gis::Point& data )
{
    m_data[ latitude ][ longitude ] = data;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of rows.
 *  @param  row [in] number of rows
 */
/*===========================================================================*/
void Mesh::setRow( const size_t row )
{
    m_row = row;
}

/*===========================================================================*/
/**
 *  @brief  Sets a number of columns.
 *  @param  column [in] number of columns
 */
/*===========================================================================*/
void Mesh::setColumn( const size_t column )
{
    m_column = column;
}

/*===========================================================================*/
/**
 *  @brief  Sets a latitude dimension.
 *  @param  latitude_dimension [in] latitude dimension
 */
/*===========================================================================*/
void Mesh::setLatitudeDimension( const size_t latitude_dimension )
{
    m_latitude_dimension = latitude_dimension;
}

/*===========================================================================*/
/**
 *  @brief  Sets a longitude dimension.
 *  @param  longitude_dimension [in] longitude dimension
 */
/*===========================================================================*/
void Mesh::setLongitudeDimension( const size_t longitude_dimension )
{
    m_longitude_dimension = longitude_dimension;
}

/*===========================================================================*/
/**
 *  @brief  Sets a latitude interval.
 *  @param  latitude_interval [in] latitude interval
 */
/*===========================================================================*/
void Mesh::setLatitudeInterval( const float latitude_interval )
{
    m_latitude_interval = latitude_interval;
}

/*===========================================================================*/
/**
 *  @brief  Sets a longitude interval.
 *  @param  longitude_interval [in] longitude interval
 */
/*===========================================================================*/
void Mesh::setLongitudeInterval( const float longitude_interval )
{
    m_longitude_interval = longitude_interval;
}

/*===========================================================================*/
/**
 *  @brief  Sets an area.
 *  @param  area [in] area
 */
/*===========================================================================*/
void Mesh::setArea( const kvs::gis::Area& area )
{
    m_area = area;
}

void Mesh::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Number of rows : " << m_row << std::endl;
    os << indent << "Number of columns : " << m_column << std::endl;
    os << indent << "Number of dimensions for latitude : " << m_latitude_dimension << std::endl;
    os << indent << "Number of dimensions for longitude : " << m_longitude_dimension << std::endl;
    os << indent << "Interval for latitude : " << m_latitude_interval << std::endl;
    os << indent << "Interval for longitude : " << m_longitude_interval << std::endl;
    os << indent << "Mesh area : " << std::endl;
    m_area.print( os, indent.nextIndent() );
}

/*===========================================================================*/
/**
 *  @brief  Allocates the mesh data.
 *  @param  row [in] number of rows
 *  @param  column [in] number of columns
 */
/*===========================================================================*/
void Mesh::allocate_data( const size_t row, const size_t column )
{
    if ( m_data ) this->deallocate_data();

    m_row = row;
    m_column = column;
    m_data = new kvs::gis::Point* [ row ];
    for ( size_t i = 0; i < row; i++ )
    {
        m_data[i] = new kvs::gis::Point [ column ];
    }
}

/*===========================================================================*/
/**
 *  @brief  Deallocates the mesh data.
 */
/*===========================================================================*/
void Mesh::deallocate_data()
{
    if ( m_data )
    {
        for ( size_t i = 0; i < m_row; i++ )
        {
            if ( m_data[i] ) delete [] m_data[i];
        }

        delete [] m_data;

        m_row = 0;
        m_column = 0;
        m_data = NULL;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns available area.
 *  @param  area [in] area of the mesh
 *  @return available area
 */
/*===========================================================================*/
kvs::gis::Area Mesh::available_area( const kvs::gis::Area& area ) const
{
    // Search minimum latitude.
    float min_latitude = 0.0f;
    if ( area.minLatitude() < m_area.minLatitude() ||
         kvs::Math::Equal( area.minLatitude(), m_area.minLatitude() ) )
    {
        min_latitude = m_area.minLatitude();
    }
    else if ( area.minLatitude() > m_area.maxLatitude() ||
              kvs::Math::Equal( area.minLatitude(), m_area.maxLatitude() ) )
    {
        min_latitude = m_area.maxLatitude();
    }
    else
    {
        for ( size_t i = 0; i < m_latitude_dimension; i++ )
        {
            min_latitude = m_area.minLatitude() + m_latitude_interval * i;
            if ( ( area.minLatitude() < min_latitude ) ||
                 kvs::Math::Equal( area.minLatitude(), min_latitude ) ) break;
        }
    }

    // Search maximum latitude.
    float max_latitude = 0.0f;
    if ( area.maxLatitude() < m_area.minLatitude() ||
         kvs::Math::Equal( area.maxLatitude(), m_area.minLatitude() ) )
    {
        max_latitude = m_area.minLatitude();
    }
    else if ( area.maxLatitude() > m_area.maxLatitude() ||
              kvs::Math::Equal( area.maxLatitude(), m_area.maxLatitude() ) )
    {
        max_latitude = m_area.maxLatitude();
    }
    else
    {
        for ( size_t i = 0; i < m_latitude_dimension; i++ )
        {
            max_latitude = m_area.maxLatitude() - m_latitude_interval * i;
            if ( ( area.maxLatitude() > max_latitude ) ||
                 kvs::Math::Equal( area.maxLatitude(), max_latitude ) ) break;
        }
    }

    // Search minimum longitude.
    float min_longitude = 0.0f;
    if ( ( area.minLongitude() < m_area.minLongitude() ) ||
         kvs::Math::Equal( area.minLongitude(), m_area.minLongitude() ) )
    {
        min_longitude = m_area.minLongitude();
    }
    else if ( ( area.minLongitude() > m_area.maxLongitude() ) ||
              kvs::Math::Equal( area.minLongitude(), m_area.maxLongitude() ) )
    {
        min_longitude = m_area.maxLongitude();
    }
    else
    {
        for ( size_t i = 0; i < m_longitude_dimension; i++ )
        {
            min_longitude = m_area.minLongitude() + m_longitude_interval * i;
            if ( ( area.minLongitude() < min_longitude ) ||
                 kvs::Math::Equal( area.minLongitude(), min_longitude ) ) break;
        }
    }

    // Search maximum longitude.
    float max_longitude = 0.0f;
    if ( ( area.maxLongitude() < m_area.minLongitude() ) ||
         kvs::Math::Equal( area.maxLongitude(), m_area.minLongitude() ) )
    {
        max_longitude = m_area.minLongitude();
    }
    else if ( ( area.maxLongitude() > m_area.maxLongitude() ) ||
              kvs::Math::Equal( area.maxLongitude(), m_area.maxLongitude() ) )
    {
        max_longitude = m_area.maxLongitude();
    }
    else
    {
        for ( size_t i = 0; i < m_longitude_dimension; i++ )
        {
            max_longitude = m_area.maxLongitude() - m_longitude_interval * i;
            if ( ( area.maxLongitude() > max_longitude ) ||
                 kvs::Math::Equal( area.maxLongitude(), max_longitude ) ) break;
        }
    }

    return kvs::gis::Area( min_latitude, max_latitude, min_longitude, max_longitude );
}

} // end of namespace gis

} // end of namespace kvs
