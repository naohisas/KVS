/*****************************************************************************/
/**
 *  @file   GSIMesh.cpp
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
#include "GSIMesh.h"
#include "Degree.h"
#include "OceanValue.h"
#include <kvs/File>
#include <kvs/Message>
#include <kvs/Math>
#include <kvs/Endian>
#include <vector>
#include <cstring>


namespace kvs
{

namespace gis
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new GSI mesh class.
 */
/*===========================================================================*/
GSIMesh::GSIMesh():
    m_code( 0 ),
    m_scale( 0 ),
    m_survey_year( 0 ),
    m_modified_year( 0 ),
    m_digitalisation_year( 0 ),
    m_foliar_num( 0 ),
    m_first_flg( false ),
    m_second_flg( false ),
    m_third_flg( false ),
    m_forth_flg( false ),
    m_record_num( 0 )
{
    for ( size_t i = 0; i < 320; i++ ) m_record_flg[i] = false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GSI mesh class.
 *  @param  filename [in] filename
 *  @param  area [in] read area
 */
/*===========================================================================*/
GSIMesh::GSIMesh( const std::string& filename, const kvs::gis::Area& area ):
    m_code( 0 ),
    m_scale( 0 ),
    m_survey_year( 0 ),
    m_modified_year( 0 ),
    m_digitalisation_year( 0 ),
    m_foliar_num( 0 ),
    m_first_flg( false ),
    m_second_flg( false ),
    m_third_flg( false ),
    m_forth_flg( false ),
    m_record_num( 0 )
{
    for ( size_t i = 0; i < 320; i++ ) m_record_flg[i] = false;

    if ( !this->read( filename, area ) )
    {
        kvsMessageError("Cannot read the GSI mesh data.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Destroys the GSI mesh class.
 */
/*===========================================================================*/
GSIMesh::~GSIMesh()
{
}

/*===========================================================================*/
/**
 *  @brief  Read GSI mesh data file.
 *  @param  filename [in] filename
 *  @param  area [in] read area
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool GSIMesh::read( const std::string& filename, const kvs::gis::Area& area )
{
    const kvs::File file( filename );
    const std::string extension = file.extension();

    if ( extension == "TEM" || extension == "tem" ) m_type = Mesh1KM;
    else if ( extension == "SEM" || extension == "sem" ) m_type = Mesh250M;
    else if ( extension == "MEM" || extension == "mem" ) m_type = Mesh50M;

    FILE* fp = fopen( filename.c_str(), "r" );
    if ( !fp )
    {
        kvsMessageError("Cannot open %s.", filename.c_str());
        return false;
    }

    if ( !this->read_header( fp ) )
    {
        kvsMessageError("Cannot read the header information of the GSI mesh.");
        return false;
    }

    if ( !this->read_data( fp, area ) )
    {
        kvsMessageError("Cannot read the data value of the GSI mesh.");
        return false;
    }

    fclose( fp );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read header information of the GSI mesh data.
 *  @param  fp [in] file pointer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool GSIMesh::read_header( FILE* fp )
{
    fseek( fp, 0, SEEK_SET );

    const int buf_size = 1012;
    char buf[ buf_size ]; memset( buf, 0, buf_size );

    if ( !fgets( buf, buf_size, fp ) ) return false;

    const std::string tmp( buf );

    char code[7]; memset( code, 0, 6 ); tmp.copy( code, 6, 0 ); code[6] = '\0';
    m_code = atoi( code );

    char scale[6]; memset( scale, 0, 5 ); tmp.copy( scale, 5, 6 ); scale[5] = '\0';
    m_scale = atoi( scale );

    char survey[5]; memset( survey, 0, 4 ); tmp.copy( survey, 4, 11 ); survey[4] = '\0';
    m_survey_year = atoi( survey );

    char modified[5]; memset( modified, 0, 4 ); tmp.copy( modified, 4, 15 ); modified[4] = '\0';
    m_modified_year = atoi( modified );

    char digitalisation[5]; memset( digitalisation, 0, 4 ); tmp.copy( digitalisation, 4, 19 ); digitalisation[4] = '\0';
    m_digitalisation_year = atoi( digitalisation );

    char lon_dim[4]; memset( lon_dim, 0, 3 ); tmp.copy( lon_dim, 3, 23 ); lon_dim[3] = '\0';
    char lat_dim[4]; memset( lat_dim, 0, 3 ); tmp.copy( lat_dim, 3, 26 ); lat_dim[3] = '\0';

    const size_t longitude_dimension = atoi( lon_dim );
    const size_t latitude_dimension = atoi( lat_dim );
    BaseClass::setLongitudeDimension( longitude_dimension );
    BaseClass::setLatitudeDimension( latitude_dimension );

    char min_lat[8]; memset( min_lat, 0, 7 ); tmp.copy( min_lat, 7, 29 ); min_lat[7] = '\0';
    char min_lon[8]; memset( min_lon, 0, 7 ); tmp.copy( min_lon, 7, 36 ); min_lon[7] = '\0';
    char max_lat[8]; memset( max_lat, 0, 7 ); tmp.copy( max_lat, 7, 43 ); max_lat[7] = '\0';
    char max_lon[8]; memset( max_lon, 0, 7 ); tmp.copy( max_lon, 7, 50 ); max_lon[7] = '\0';

    const kvs::gis::Degree min_lat_deg( atoi( min_lat ) );
    const kvs::gis::Degree min_lon_deg( atoi( min_lon ) );
    const kvs::gis::Degree max_lat_deg( atoi( max_lat ) );
    const kvs::gis::Degree max_lon_deg( atoi( max_lon ) );

    const float min_latitude = min_lat_deg.decimal();
    const float max_latitude = max_lat_deg.decimal();
    const float min_longitude = min_lon_deg.decimal();
    const float max_longitude = max_lon_deg.decimal();
    BaseClass::setArea( kvs::gis::Area( min_latitude, max_latitude, min_longitude, max_longitude ) );

    const float diff_latitude = max_latitude - min_latitude;
    const float diff_longitude = max_longitude - min_longitude;
    BaseClass::setLatitudeInterval( diff_latitude / ( latitude_dimension - 1 ) );
    BaseClass::setLongitudeInterval( diff_longitude / ( longitude_dimension - 1 ) );

    char foliar = '0'; tmp.copy( &foliar, 1, 57 );
    m_foliar_num = 1;

    memset( m_first_name, 0, 20 );
    tmp.copy( m_first_name, 20, 58 );

    char first_flg = '0'; tmp.copy( &first_flg, 1, 78 );
    m_first_flg = first_flg == '0' ? false : true;

    memset( m_second_name, 0, 20 );
    tmp.copy( m_second_name, 20, 79 );

    char second_flg = '0'; tmp.copy( &second_flg, 1, 99 );
    m_second_flg = second_flg == '0' ? false : true;

    memset( m_third_name, 0, 20 );
    tmp.copy( m_third_name, 20, 100 );

    char third_flg = '0'; tmp.copy( &third_flg, 1, 120 );
    m_third_flg = third_flg == '0' ? false : true;

    memset( m_forth_name, 0, 20 );
    tmp.copy( m_forth_name, 20, 121 );

    char forth_flg = '0'; tmp.copy( &forth_flg, 1, 141 );
    m_forth_flg = forth_flg == '0' ? false : true;

    char record_num[4]; memset( record_num, 0, 3 ); tmp.copy( record_num, 3, 142 ); record_num[3] = '\0';
    m_record_num = atoi( record_num );

    memset( m_comment, 0, 80 );
    tmp.copy( m_comment, 80, 145 );

    char record_flg = '0';
    size_t size = 0;
    switch( m_type )
    {
    case Mesh1KM: size = 80; break;
    case Mesh250M: size = 320; break;
    case Mesh50M: size = 200; break;
    default: break;
    }

    for ( size_t i = 0; i < size; i++ )
    {
        tmp.copy( &record_flg, 1, 225 + i );
        m_record_flg[i] = record_flg == '0' ? false : true;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read GSI mesh data.
 *  @param  fp [in] filen pointer
 *  @param  area [in] read area
 *  @return ture, if the reading process is done successfully
 */
/*===========================================================================*/
bool GSIMesh::read_data( FILE* fp, const kvs::gis::Area& area )
{
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

    const float max_latitude = BaseClass::area().maxLatitude();
    const float min_longitude = BaseClass::area().minLongitude();
    const size_t max_lat_id = kvs::Math::Round( ( max_latitude - range[1] ) / latitude_interval );
    const size_t min_lon_id = kvs::Math::Round( ( range[2] - min_longitude ) / longitude_interval );

    // Set position data.
    std::vector<size_t> available_id; available_id.clear();
    for ( size_t i = 0; i < lat_size; i++ )
    {
        const size_t lat_id = max_lat_id + i;
        const float latitude = max_latitude - latitude_interval * lat_id;
        if ( m_record_flg[lat_id] ) available_id.push_back( lat_id + 1 );

        for ( size_t j = 0; j < lon_size; j++ )
        {
            const size_t lon_id = min_lon_id + j;
            const float longitude = min_longitude + longitude_interval * lon_id;

            kvs::gis::Point data = BaseClass::data( i, j );
            data.setLatitude( latitude );
            data.setLongitude( longitude );
            if ( !m_record_flg[lat_id] )
            {
                data.setOcean( true );
                data.setHeight( kvs::gis::OceanValue );
            }

            BaseClass::setData( i, j, data );
        }
    }

    if ( available_id.size() != 0 )
    {
        size_t read_ctr = 0;
        for ( size_t i = 0; i < BaseClass::latitudeDimension(); i++ )
        {
            if ( m_record_flg[i] ) read_ctr++;
        }

        size_t line = 0;
        switch ( m_type )
        {
        case Mesh1KM: line = 412; break;
        case Mesh250M: line = 1612; break;
        case Mesh50M: line = 1012; break;
        default: break;
        }

        // Read data.
        char* buf = new char [ line ];
        size_t available_ctr = 0;
        for ( size_t i = 0; i < read_ctr; i++ )
        {
            if ( !fgets( buf, line, fp ) ) return false;

            const std::string tmp( buf );
            char code[7]; memset( code, 0, 6 ); tmp.copy( code, 6, 0 ); code[6] = '\0';
            char number[4]; memset( number, 0, 3 ); tmp.copy( number, 3, 6 ); number[3] = '\0';
            const size_t num = atoi( number );
            if ( num != available_id[available_ctr] ) continue;

            for ( size_t j = 0; j < lon_size; j++ )
            {
                const size_t lon_id = min_lon_id + j;

                char hei[6]; memset( hei, 0, 5 ); tmp.copy( hei, 5, 9 + 5 * lon_id ); hei[5] = '\0';
                const int h = atoi( hei );
                const size_t id = num - 1 - max_lat_id;

                kvs::gis::Point data = BaseClass::data( id, j );
                kvs::gis::Area area = BaseClass::area();
                if ( h == int( kvs::gis::OceanValue ) )
                {
                    const float height = kvs::gis::OceanValue / 10000.0f; // km
                    data.setHeight( height );
                    data.setOcean( true );
                }
                else
                {
                    const float height = h / 10000.0f; // km
                    data.setHeight( height );
                    data.setOcean( false );

                    area.setMinHeight( kvs::Math::Min( height, area.minHeight() ) );
                    area.setMaxHeight( kvs::Math::Max( height, area.maxHeight() ) );
                }
                BaseClass::setData( id, j, data );
                BaseClass::setArea( area );
            }

            available_ctr++;
        }

        if ( buf ) { delete [] buf; buf = NULL; }
    }

    // Reset some parameters.
    BaseClass::setLatitudeDimension( lat_size );
    BaseClass::setLongitudeDimension( lon_size );
    BaseClass::setArea( available_area );

    return true;
}

} // end of namespace gis

} // end of namespace kvs
