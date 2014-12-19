/*****************************************************************************/
/**
 *  @file   Data.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Data.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Data.h"
#include <cstring>
#include <kvs/Endian>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  Construct a new Data class.
 */
/*===========================================================================*/
Data::Data()
{
}

/*===========================================================================*/
/**
 *  @brief  Output Data class information.
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const Data& d )
{
    os << "array type header: " << d.m_array_type_header << std::endl;
    os << "keyword: " << d.m_keyword << std::endl;
    os << "comment: " << d.m_comment << std::endl;
    os << "num: " << d.m_num << std::endl;
    os << "num2: " << d.m_num2;

    return os;
}

/*===========================================================================*/
/**
 *  @brief  Return array type header.
 *  @return array type header
 */
/*===========================================================================*/
const std::string& Data::arrayTypeHeader() const
{
    return m_array_type_header;
}

/*===========================================================================*/
/**
 *  @brief  Return keyword.
 *  @return keyword
 */
/*===========================================================================*/
const std::string& Data::keyword() const
{
    return m_keyword;
}

/*===========================================================================*/
/**
 *  @brief  Return comment.
 *  @return comment
 */
/*===========================================================================*/
const std::string& Data::comment() const
{
    return m_comment;
}

/*===========================================================================*/
/**
 *  @brief  Return data size of 2D array (vector length of the data).
 *  @return data size of 2D array
 */
/*===========================================================================*/
kvs::Int32 Data::num() const
{
    return m_num;
}

/*===========================================================================*/
/**
 *  @brief  Return data size of 2D array (number of element of the data).
 *  @return data size of 2D array
 */
/*===========================================================================*/
kvs::Int32 Data::num2() const
{
    return m_num2;
}

/*===========================================================================*/
/**
 *  @brief  Return data array (float type).
 *  @return data array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32>& Data::fltArray() const
{
    return m_flt_array;
}

/*===========================================================================*/
/**
 *  @brief  Return data array (integer type).
 *  @return data array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Int32>& Data::intArray() const
{
    return m_int_array;
}

/*===========================================================================*/
/**
 *  @brief  Deallocate data sets and comments.
 */
/*===========================================================================*/
void Data::deallocate()
{
    m_array_type_header = "";
    m_keyword = "";
    m_comment = "";
    m_num2 = 0;
    m_num = 0;
    m_flt_array.release();
    m_int_array.release();
}

/*===========================================================================*/
/**
 *  @brief  Read ascii type file.
 *  @param  fp [in] file pointer
 *  @param  tag [in] header tag
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Data::readAscii( FILE* fp, const std::string tag )
{
    const size_t line_size = 256;
    char line[line_size];
    memset( line, 0, line_size );

    // Read an array-type-header (#FLT_ARY or #INT_ARY).
    m_array_type_header = tag;
    if ( !( m_array_type_header == "#FLT_ARY" ||
            m_array_type_header == "#INT_ARY" ) )
    {
        kvsMessageError("Unknown array type \"%s\".", m_array_type_header.c_str());
        return false;
    }

    // Read a keyword.
    fgets( line, line_size, fp );
    m_keyword = std::string( line, 8 );

    // Read a commnet (data name).
    fgets( line, line_size, fp );
    m_comment = std::string( line, 30 );

    // Read a num (vector length) and a num2 (number of elements).
    fgets( line, line_size, fp );
    sscanf( line, "%d %d", &m_num, &m_num2 );

    // Read 2D array.
    if ( m_array_type_header == "#FLT_ARY" )
    {
        const size_t size = m_num * m_num2;
        m_flt_array.allocate( size );
        kvs::Real32* data = m_flt_array.data();

        const char* delim = " ,\t\n\r";
        size_t counter = 0;
        while ( counter < size )
        {
            fgets( line, line_size, fp );
            char* value = strtok( line, delim );
            for ( ; ; )
            {
                if ( !value ) break;

                *(data++) = static_cast<kvs::Real32>( atof( value ) );
                value = strtok( 0, delim );
                counter++;
            }
        }
    }
    else if ( m_array_type_header == "#INT_ARY" )
    {
        const size_t size = m_num * m_num2;
        m_int_array.allocate( size );
        kvs::Int32* data = m_int_array.data();

        const char* delim = " ,\t\n\r";
        size_t counter = 0;
        while ( counter < size )
        {
            fgets( line, line_size, fp );
            char* value = strtok( line, delim );
            for ( ; ; )
            {
                if ( !value ) break;

                *(data++) = static_cast<kvs::Int32>( atoi( value ) );
                value = strtok( 0, delim );
                counter++;
            }
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read binary type file (Fortran unformated).
 *  @param  fp [in] file pointer
 *  @param  tag [in] header tag
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Data::readBinary( FILE* fp, const bool swap )
{
    // Read an array-type-header (#FLT_ARY or #INT_ARY).
    char array_type_header[8];
    fseek( fp, 4, SEEK_CUR );
    fread( array_type_header, 1, 8, fp );
    fseek( fp, 4, SEEK_CUR );
    m_array_type_header = std::string( array_type_header, 8 );
    if ( !( m_array_type_header == "#FLT_ARY" ||
            m_array_type_header == "#INT_ARY" ) )
    {
        kvsMessageError("Unknown array type \"%s\".", m_array_type_header.c_str());
        return false;
    }

    // Read a keyword.
    char keyword[8];
    fseek( fp, 4, SEEK_CUR );
    fread( keyword, 1, 8, fp );
    fseek( fp, 4, SEEK_CUR );
    m_keyword = std::string( keyword, 8 );

    // Read a commnet (data name).
    char comment[30];
    for ( size_t i = 0; i < 30; i++ ) comment[i] = '\0';
    fseek( fp, 4, SEEK_CUR );
    fread( comment, 1, 30, fp );
    fseek( fp, 4, SEEK_CUR );
    m_comment = std::string( comment );

    // Read a num (vector length) and a num2 (number of elements).
    fseek( fp, 4, SEEK_CUR );
    fread( &m_num, 4, 1, fp );
    fread( &m_num2, 4, 1, fp );
    fseek( fp, 4, SEEK_CUR );
    if ( swap ) kvs::Endian::Swap( &m_num );
    if ( swap ) kvs::Endian::Swap( &m_num2 );

    // Read 2D array.
    if ( m_array_type_header == "#FLT_ARY" )
    {
        const size_t size = m_num * m_num2;
        m_flt_array.allocate( size );
        kvs::Real32* pointer = m_flt_array.data();
        fseek( fp, 4, SEEK_CUR );
        fread( pointer, sizeof(kvs::Real32), size, fp );
        fseek( fp, 4, SEEK_CUR );
        if ( swap ) kvs::Endian::Swap( pointer, size );
    }
    else if ( m_array_type_header == "#INT_ARY" )
    {
        const size_t size = m_num * m_num2;
        m_int_array.allocate( size );
        kvs::Int32* pointer = m_int_array.data();
        fseek( fp, 4, SEEK_CUR );
        fread( pointer, sizeof(kvs::Int32), size, fp );
        fseek( fp, 4, SEEK_CUR );
        if ( swap ) kvs::Endian::Swap( pointer, size );
    }

    return true;
}

} // end of namespace gf

} // end of namespace kvs
