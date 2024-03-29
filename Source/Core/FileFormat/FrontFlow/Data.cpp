/*****************************************************************************/
/**
 *  @file   Data.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Data.h"
#include <cstring>
#include <kvs/Endian>
#include <kvs/FileFormatBase>


namespace
{
auto& Read = kvs::FileFormatBase::Read;
auto& Seek = kvs::FileFormatBase::Seek;
}

namespace kvs
{

namespace gf
{

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
    if ( !fgets( line, line_size, fp ) )
    {
        kvsMessageError() << "Cannot read a line for keyword." << std::endl;
    }
    m_keyword = std::string( line, 8 );

    // Read a commnet (data name).
    if ( !fgets( line, line_size, fp ) )
    {
        kvsMessageError() << "Cannot read a line for comment." << std::endl;
    }
    m_comment = std::string( line, 30 );

    // Read a num (vector length) and a num2 (number of elements).
    if ( !fgets( line, line_size, fp ) )
    {
        kvsMessageError() << "Cannot read a line for num and num2." << std::endl;
    }
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
            if ( !fgets( line, line_size, fp ) )
            {
                kvsMessageError() << "Cannot read a line for values." << std::endl;
            }
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
            if ( !fgets( line, line_size, fp ) )
            {
                kvsMessageError() << "Cannot read a line for values." << std::endl;
            }
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
    ::Seek( fp, 4, SEEK_CUR );
    ::Read( array_type_header, 1, 8, fp );
    ::Seek( fp, 4, SEEK_CUR );
    m_array_type_header = std::string( array_type_header, 8 );
    if ( !( m_array_type_header == "#FLT_ARY" ||
            m_array_type_header == "#INT_ARY" ) )
    {
        kvsMessageError("Unknown array type \"%s\".", m_array_type_header.c_str());
        return false;
    }

    // Read a keyword.
    char keyword[8];
    ::Seek( fp, 4, SEEK_CUR );
    ::Read( keyword, 1, 8, fp );
    ::Seek( fp, 4, SEEK_CUR );
    m_keyword = std::string( keyword, 8 );

    // Read a commnet (data name).
    char comment[30];
    for ( size_t i = 0; i < 30; i++ ) comment[i] = '\0';
    ::Seek( fp, 4, SEEK_CUR );
    ::Read( comment, 1, 30, fp );
    ::Seek( fp, 4, SEEK_CUR );
    m_comment = std::string( comment );

    // Read a num (vector length) and a num2 (number of elements).
    ::Seek( fp, 4, SEEK_CUR );
    ::Read( &m_num, 4, 1, fp );
    ::Read( &m_num2, 4, 1, fp );
    ::Seek( fp, 4, SEEK_CUR );
    if ( swap ) kvs::Endian::Swap( &m_num );
    if ( swap ) kvs::Endian::Swap( &m_num2 );

    // Read 2D array.
    if ( m_array_type_header == "#FLT_ARY" )
    {
        const size_t size = m_num * m_num2;
        m_flt_array.allocate( size );
        kvs::Real32* pointer = m_flt_array.data();
        ::Seek( fp, 4, SEEK_CUR );
        ::Read( pointer, sizeof(kvs::Real32), size, fp );
        ::Seek( fp, 4, SEEK_CUR );
        if ( swap ) kvs::Endian::Swap( pointer, size );
    }
    else if ( m_array_type_header == "#INT_ARY" )
    {
        const size_t size = m_num * m_num2;
        m_int_array.allocate( size );
        kvs::Int32* pointer = m_int_array.data();
        ::Seek( fp, 4, SEEK_CUR );
        ::Read( pointer, sizeof(kvs::Int32), size, fp );
        ::Seek( fp, 4, SEEK_CUR );
        if ( swap ) kvs::Endian::Swap( pointer, size );
    }

    return true;
}

} // end of namespace gf

} // end of namespace kvs
