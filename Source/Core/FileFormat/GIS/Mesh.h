/*****************************************************************************/
/**
 *  @file   Mesh.h
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
#ifndef KVS__GIS__MESH_H_INCLUDE
#define KVS__GIS__MESH_H_INCLUDE

#include "Point.h"
#include "Area.h"
#include <iostream>
#include <kvs/Vector2>
#include <kvs/Indent>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Mesh class.
 */
/*===========================================================================*/
class Mesh
{
private:

    kvs::gis::Point** m_data; ///< set of points
    size_t m_row; ///< number of rows
    size_t m_column; ///< number of columns
    size_t m_latitude_dimension; ///< number of dimensions for latitude
    size_t m_longitude_dimension; ///< number of dimensions for longitude
    float m_latitude_interval; ///< interval for latitude
    float m_longitude_interval; ///< interval for longitude
    kvs::gis::Area m_area; ///< mesh area

public:

    Mesh();
    virtual ~Mesh();

    const kvs::gis::Point& data( const size_t latitude, const size_t longitude ) const;
    size_t row() const;
    size_t column() const;
    size_t latitudeDimension() const;
    size_t longitudeDimension() const;
    float latitudeInterval() const;
    float longitudeInterval() const;
    const kvs::gis::Area& area() const;
    bool hasData() const;

    void setData( const size_t latitude, const size_t longitude, const kvs::gis::Point& data );
    void setRow( const size_t row );
    void setColumn( const size_t column );
    void setLatitudeDimension( const size_t latitude_dimension );
    void setLongitudeDimension( const size_t longitude_dimension );
    void setLatitudeInterval( const float latitude_interval );
    void setLongitudeInterval( const float longitude_interval );
    void setArea( const kvs::gis::Area& area );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;

protected:

    void allocate_data( const size_t row, const size_t column );
    void deallocate_data();
    kvs::gis::Area available_area( const kvs::gis::Area& area ) const;
};

} // end of namespace gis

} // end of namespace kvs

#endif // KVS__GIS__MESH_H_INCLUDE
