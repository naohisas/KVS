/*****************************************************************************/
/**
 *  @file   USGSMesh.cpp
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
#include "USGSMesh.h"
#include "OceanValue.h"
#include <kvs/Math>
#include <kvs/Endian>
#include <cstring>
#include <cstdlib>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new USGS mesh class.
 */
/*===========================================================================*/
USGSMesh::USGSMesh():
    m_nbands( 0 ),
    m_nbits( 0 ),
    m_band_row_bytes( 0 ),
    m_total_row_bytes( 0 ),
    m_band_gap_bytes( 0 ),
    m_ocean_value( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new USGS mesh class.
 *  @param  header_filename [in] filename of header
 *  @param  data_filename [in] filename of data
 *  @param  area [in] read area
 */
/*===========================================================================*/
USGSMesh::USGSMesh(
    const std::string& header_filename,
    const std::string& data_filename,
    const kvs::gis::Area& area ):
    m_nbands( 0 ),
    m_nbits( 0 ),
    m_band_row_bytes( 0 ),
    m_total_row_bytes( 0 ),
    m_band_gap_bytes( 0 ),
    m_ocean_value( 0 )
{
    if ( !this->read( header_filename, data_filename, area ) )
    {
        kvsMessageError("Cannot read the USGSMesh data.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Destroys the USGS mesh class.
 */
/*===========================================================================*/
USGSMesh::~USGSMesh()
{
}

/*===========================================================================*/
/**
 *  @brief  Read USGS mesh file.
 *  @param  header_filename [in] filename of header
 *  @param  data_filename [in] filename of data
 *  @param  area [in] read area
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool USGSMesh::read(
    const std::string& header_filename,
    const std::string& data_filename,
    const kvs::gis::Area& area )
{
    FILE* header_fp = fopen( header_filename.c_str(), "r" );
    if ( !header_fp )
    {
        kvsMessageError("Cannot open %s.", header_filename.c_str());
        return false;
    }

    FILE* data_fp = fopen( data_filename.c_str(), "r" );
    if ( !data_fp )
    {
        kvsMessageError("Cannot open %s.", data_filename.c_str());
        return false;
    }

    if ( !this->read_header( header_fp ) )
    {
        kvsMessageError("Cannot read the header information of the USGS mesh.");
        return false;
    }

    if ( !this->read_data( data_fp, area ) )
    {
        kvsMessageError("Cannot read the data values of the USGS mesh.");
        return false;
    }

    fclose( header_fp );
    fclose( data_fp );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads header information of the USGS mesh data.
 *  @param  fp [in] file pointer
 *  @return true, if the reading process is done ssuccessfully
 */
/*===========================================================================*/
bool USGSMesh::read_header( FILE* fp )
{
    fseek( fp, 0, SEEK_SET );

    const int buf_size = 512;
    char buf[ buf_size ];

    while ( fgets( buf, buf_size, fp ) != NULL )
    {
        if ( buf[0] == '#' ) continue; // Skip comment line.

        char* p = strstr( buf, "=" );
        if ( p == NULL ) continue; // Read only the line included '='.

        p++;

        if ( !strncmp( buf, "NROWS", 5 ) )
        {
            BaseClass::setLatitudeDimension( atoi( p ) ); continue;
        }
        if ( !strncmp( buf, "NCOLS", 5 ) )
        {
            BaseClass::setLongitudeDimension( atoi( p ) ); continue;
        }

        if ( !strncmp( buf, "NBANDS", 6 ) )
        {
            m_nbands = atoi( p ); continue;
        }
        if ( !strncmp( buf, "NBITS", 5 ) )
        {
            m_nbits = atoi( p ); continue;
        }
        if ( !strncmp( buf, "BANDROWBYTES", 12 ) )
        {
            m_band_row_bytes = atoi( p ); continue;
        }
        if ( !strncmp( buf, "TOTALROWBYTES", 13 ) )
        {
            m_total_row_bytes = atoi( p ); continue;
        }
        if ( !strncmp( buf, "BANDGAPBYTES", 12 ) )
        {
            m_band_gap_bytes = atoi( p ); continue;
        }
        if ( !strncmp( buf, "NODATA", 6 ) )
        {
            m_ocean_value = static_cast<short>( atoi( p ) ); continue;
        }

        kvs::gis::Area area = BaseClass::area();
        if ( !strncmp( buf, "ulxmap", 6 ) )
        {
            area.setMinLongitude( static_cast<float>( atof( p ) ) ); continue;
        }
        if ( !strncmp( buf, "ulymap", 6 ) )
        {
            area.setMaxLatitude( static_cast<float>( atof( p ) ) ); continue;
        }
        BaseClass::setArea( area );

        if ( !strncmp( buf, "xdim", 4 ) )
        {
            BaseClass::setLongitudeInterval( static_cast<float>( atof( p ) ) ); continue;
        }
        if ( !strncmp( buf, "ydim", 4 ) )
        {
            BaseClass::setLatitudeInterval( static_cast<float>( atof( p ) ) ); continue;
        }
    }

    const float max_latitude = BaseClass::area().maxLatitude();
    const float min_longitude = BaseClass::area().minLongitude();
    const float latitude_interval = BaseClass::latitudeInterval();
    const float longitude_interval = BaseClass::longitudeInterval();
    const size_t latitude_dimension = BaseClass::latitudeDimension();
    const size_t longitude_dimension = BaseClass::longitudeDimension();
    kvs::gis::Area area = BaseClass::area();
    area.setMinLatitude( max_latitude - ( latitude_dimension - 1 ) * latitude_interval );
    area.setMaxLongitude( min_longitude + ( longitude_dimension - 1 ) * longitude_interval );
    BaseClass::setArea( area );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads data of the USGS mesh data.
 *  @param  fp [in] file pointer
 *  @param  area [in] read area
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool USGSMesh::read_data( FILE* fp, const kvs::gis::Area& area )
{
    // Read feet data.
    const size_t latitude_dimension = BaseClass::latitudeDimension();
    const size_t longitude_dimension = BaseClass::longitudeDimension();
    short* data = new short [ latitude_dimension * longitude_dimension ];
    fread( data, sizeof(short) * latitude_dimension * longitude_dimension, 1, fp );
    fclose( fp );

    // Set data size.
    const kvs::gis::Area available_area = BaseClass::available_area( area );
    const float range[4] = {
        available_area.minLatitude(),
        available_area.maxLatitude(),
        available_area.minLongitude(),
        available_area.maxLongitude()
    };
    if ( kvs::Math::Equal( range[0], range[1] ) || kvs::Math::Equal( range[2], range[3] ) )
    {
        return true;
    }

    const float latitude_interval = BaseClass::latitudeInterval();
    const float longitude_interval = BaseClass::longitudeInterval();
    const size_t lat_size = kvs::Math::Round( ( range[1] - range[0] ) / latitude_interval  ) + 1;
    const size_t lon_size = kvs::Math::Round( ( range[3] - range[2] ) / longitude_interval ) + 1;
    BaseClass::allocate_data( lat_size, lon_size );

    // Set data.
    size_t row = 0;
    size_t col = 0;
    for ( size_t i = 0; i < latitude_dimension; i++ )
    {
        const float latitude = BaseClass::area().maxLatitude() - latitude_interval * i;
        for ( size_t j = 0; j < longitude_dimension; j++ )
        {
            const float longitude = BaseClass::area().minLongitude() + longitude_interval * j;
            if ( ( kvs::Math::Equal( range[0], latitude  ) || range[0] < latitude  ) &&
                 ( kvs::Math::Equal( range[1], latitude  ) || range[1] > latitude  ) &&
                 ( kvs::Math::Equal( range[2], longitude ) || range[2] < longitude ) &&
                 ( kvs::Math::Equal( range[3], longitude ) || range[3] > longitude ) )
            {
                const size_t index = longitude_dimension * i + j;
                short h = data[index];
                kvs::Endian::Swap( &h );

                kvs::gis::Point data = BaseClass::data( row, col );
                kvs::gis::Area area = BaseClass::area();

                // Ocean surface data.
                data.setLatitude( latitude );
                data.setLongitude( longitude );
                if ( h == short( kvs::gis::OceanValue ) )
                {
                    const float height = kvs::gis::OceanValue / 10000.0f; // km
                    data.setHeight( height );
                    data.setOcean( true );
                }
                else
                {
                    const float height = h * 0.0003048f; // ft -> km
                    data.setHeight( height );
                    data.setOcean( false );

                    area.setMinHeight( kvs::Math::Min( height, area.minHeight() ) );
                    area.setMaxHeight( kvs::Math::Max( height, area.maxHeight() ) );
                }
                BaseClass::setData( row, col, data );
                BaseClass::setArea( area );

                col++;
                if ( col >= lon_size ) { col = 0; row++; }
            }
        }
    }

    delete [] data;

    // Reset some parameters.
    BaseClass::setLatitudeDimension( lat_size );
    BaseClass::setLongitudeDimension( lon_size );
    BaseClass::setArea( available_area );

    return true;
}

} // end of namespace gis

} // end of namespace kvs
