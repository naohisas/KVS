/*****************************************************************************/
/**
 *  @file   GriddedBinaryDataFile.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GriddedBinaryDataFile.cpp 1793 2014-08-01 07:43:50Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "GriddedBinaryDataFile.h"
#include <fstream>
#include <kvs/Endian>


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new GriddedBinaryDataFile class.
 */
/*===========================================================================*/
GriddedBinaryDataFile::GriddedBinaryDataFile():
    m_sequential( false ),
    m_big_endian( false ),
    m_filename("")
{
    m_date.hour = 0;
    m_date.minute = 0;
    m_date.day = 1;
    m_date.month = 1;
    m_date.year = 0;
}

/*===========================================================================*/
/**
 *  @brief  Sets date.
 *  @param  date
 */
/*===========================================================================*/
void GriddedBinaryDataFile::setDate( const Date& date )
{
    m_date.hour = date.hour;
    m_date.minute = date.minute;
    m_date.day = date.day;
    m_date.month = date.month;
    m_date.year = date.year;
}

/*===========================================================================*/
/**
 *  @brief  Sets sequential dat flag.
 *  @param  sequential [in] sequential data flag
 */
/*===========================================================================*/
void GriddedBinaryDataFile::setSequential( const bool sequential )
{
    m_sequential = sequential;
}

/*===========================================================================*/
/**
 *  @brief  Sets big endian flag.
 *  @param  big_endian [in] big endian flag
 */
/*===========================================================================*/
void GriddedBinaryDataFile::setBigEndian( const bool big_endian )
{
    m_big_endian = big_endian;
}

/*===========================================================================*/
/**
 *  @brief  Sets data filename.
 *  @param  filename [in] data filename
 */
/*===========================================================================*/
void GriddedBinaryDataFile::setFilename( const std::string& filename )
{
    m_filename = filename;
}

/*===========================================================================*/
/**
 *  @brief  Returns the date.
 *  @return date
 */
/*===========================================================================*/
const GriddedBinaryDataFile::Date& GriddedBinaryDataFile::date() const
{
    return m_date;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sequential flag.
 *  @return true if the data is a fortran binary format
 */
/*===========================================================================*/
bool GriddedBinaryDataFile::sequential() const
{
    return m_sequential;
}

/*===========================================================================*/
/**
 *  @brief  Returns the big endian flag.
 *  @return true if the data is big endian
 */
/*===========================================================================*/
bool GriddedBinaryDataFile::bigEndian() const
{
    return m_big_endian;
}

/*===========================================================================*/
/**
 *  @brief  Returns data filename.
 *  @return data filename
 */
/*===========================================================================*/
const std::string& GriddedBinaryDataFile::filename() const
{
    return m_filename;
}

/*===========================================================================*/
/**
 *  @brief  Returns data values.
 *  @return data values
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32>& GriddedBinaryDataFile::values() const
{
    return m_values;
}

/*===========================================================================*/
/**
 *  @brief  Returns data values specified by the given index.
 *  @param  vindex [in] value index
 *  @param  dim [in] data dimension
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> GriddedBinaryDataFile::values(
    const size_t vindex,
    const kvs::Vec3ui& dim ) const
{
    const size_t size = dim.x() * dim.y() * dim.z();
    if ( m_values.size() < ( vindex + 1 ) * size ) return kvs::ValueArray<kvs::Real32>();

    kvs::ValueArray<kvs::Real32> dst( size );
    kvs::Real32* src = m_values.data() + vindex * size;
    for ( size_t i = 0; i < size; i++ ) { dst[i] = src[i]; }

    return dst;
}

/*===========================================================================*/
/**
 *  @brief  Loads data values from the specified data file.
 *  @return true, if the loading process is done successfully
 */
/*===========================================================================*/
bool GriddedBinaryDataFile::load() const
{
    if ( m_filename.length() == 0 )
    {
        kvsMessageError("Filename of binary data has not been specified.");
        return false;
    }

    std::ifstream ifs( m_filename.c_str(), std::ios::binary | std::ios::in );
    if( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", m_filename.c_str() );
        return false;
    }

    ifs.seekg( 0, std::ios::end );
    const std::streamoff end = ifs.tellg();

    ifs.seekg( 0, std::ios::beg );
    const std::streamoff begin = ifs.tellg();
    const std::streamoff file_size = end - begin; // [byte]

    if ( m_sequential )
    {
        const size_t element_size = sizeof( kvs::Real32 ) + 4 * sizeof( kvs::Int16 );
        const size_t nelements = static_cast<size_t>( file_size / element_size );
        m_values.allocate( nelements );

        for ( size_t i = 0; i < nelements; i++ )
        {
            kvs::Int16 padding[4];
            kvs::Real32 value;
            ifs.read( (char*)( padding + 0 ), sizeof( kvs::Int16 ) );
            ifs.read( (char*)( padding + 1 ), sizeof( kvs::Int16 ) );
            ifs.read( (char*)( &value ), sizeof( kvs::Real32 ) );
            ifs.read( (char*)( padding + 2 ), sizeof( kvs::Int16 ) );
            ifs.read( (char*)( padding + 3 ), sizeof( kvs::Int16 ) );
            m_values[i] = value;
        }
    }
    else
    {
        const size_t element_size = sizeof( kvs::Real32 );
        const size_t nelements = static_cast<size_t>( file_size / element_size );
        m_values.allocate( nelements );

        ifs.read( (char*)( m_values.data() ), file_size );
    }

    if ( m_big_endian != kvs::Endian::IsBig() )
    {
        kvs::Endian::Swap( m_values.data(), m_values.size() );
    }

    ifs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Free loaded data values.
 */
/*===========================================================================*/
void GriddedBinaryDataFile::free() const
{
    m_values.release();
}

} // end of namespace grads

} // end of namespace kvs
