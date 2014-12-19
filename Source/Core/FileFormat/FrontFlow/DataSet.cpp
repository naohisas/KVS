/*****************************************************************************/
/**
 *  @file   DataSet.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataSet.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "DataSet.h"
#include <cstring>
#include <kvs/Endian>


namespace kvs
{

namespace gf
{

/*===========================================================================*/
/**
 *  @brief  Construct a new DataSet class.
 */
/*===========================================================================*/
DataSet::DataSet()
{
}

/*===========================================================================*/
/**
 *  @brief  Output DataSet class information.
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const DataSet& d )
{
    for ( size_t i = 0; i < d.m_comment_list.size(); i++ )
    {
        os << "comment[" << i << "]: " << d.m_comment_list[i] << std::endl;
    }

    for ( size_t i = 0; i < d.m_data_list.size(); i++ )
    {
        os << "data[" << i << "]:" << std::endl;
        os << d.m_data_list[i];
        if ( i != d.m_data_list.size() - 1 ) os << std::endl;
    }

    return os;
}

/*===========================================================================*/
/**
 *  @brief  Return comment list.
 *  @return comment list
 */
/*===========================================================================*/
const std::vector<std::string>& DataSet::commentList() const
{
    return m_comment_list;
}

/*===========================================================================*/
/**
 *  @brief  Return comment specified by the index.
 *  @param  index [in] index of comment list
 *  @return comment
 */
/*===========================================================================*/
const std::string& DataSet::comment( const size_t index ) const
{
    return m_comment_list.at( index );
}

/*===========================================================================*/
/**
 *  @brief  Return data list.
 *  @return data list
 */
/*===========================================================================*/
const std::vector<kvs::gf::Data>& DataSet::dataList() const
{
    return m_data_list;
}

/*===========================================================================*/
/**
 *  @brief  Return data specified by the index.
 *  @param  index [in] index of data set list
 *  @return data
 */
/*===========================================================================*/
const kvs::gf::Data& DataSet::data( const size_t index ) const
{
    return m_data_list.at( index );
}

/*===========================================================================*/
/**
 *  @brief  Deallocate data and comments.
 */
/*===========================================================================*/
void DataSet::deallocate()
{
    std::vector<std::string>::iterator comment = m_comment_list.begin();
    std::vector<std::string>::const_iterator last_comment = m_comment_list.end();
    while ( comment != last_comment )
    {
        comment->assign("");
        comment++;
    }

    std::vector<kvs::gf::Data>::iterator data = m_data_list.begin();
    std::vector<kvs::gf::Data>::const_iterator last_data = m_data_list.end();
    while ( data != last_data )
    {
        data->deallocate();
        data++;
    }
}

/*===========================================================================*/
/**
 *  @brief  Read ascii type file.
 *  @param  fp [in] file pointer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool DataSet::readAscii( FILE* fp )
{
    const size_t line_size = 256;
    char line[line_size];
    memset( line, 0, line_size );

    // Read a number of comments.
    kvs::Int32 ncomments = 0;
    fgets( line, line_size, fp );
    sscanf( line, "%d", &ncomments );

    // Read commnets.
    for ( size_t i = 0; i < size_t( ncomments ); i++ )
    {
        fgets( line, line_size, fp );
        if ( line[ strlen(line) - 1 ] == '\n' ) line[ strlen(line) - 1 ] = '\0';

        const std::string comment( line );
        m_comment_list.push_back( comment );
    }

    // Read data set.
    for ( ; ; )
    {
        fgets( line, line_size, fp );
        const std::string tag( line, 8 );
        if ( tag == "#ENDFILE" || tag == "#NEW_SET" ) { break; }

        kvs::gf::Data data;
        if ( !data.readAscii( fp, tag ) ) return false;

        m_data_list.push_back( data );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read binary type file (Fortran unformated).
 *  @param  fp [in] file pointer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool DataSet::readBinary( FILE* fp, const bool swap )
{
    // Read a number of comments.
    kvs::Int32 ncomments = 0;
    fseek( fp, 4, SEEK_CUR );
    fread( &ncomments, 4, 1, fp );
    fseek( fp, 4, SEEK_CUR );
    if ( swap ) kvs::Endian::Swap( &ncomments );

    // Read commnets.
    char comment[60];
    char initialize[60]; for ( size_t i = 0; i < 60; i++ ) initialize[i] = '\0';
    for ( size_t i = 0; i < size_t( ncomments ); i++ )
    {
        memcpy( comment, initialize, 60 );
        fseek( fp, 4, SEEK_CUR );
        fread( comment, 1, 60, fp );
        fseek( fp, 4, SEEK_CUR );

        m_comment_list.push_back( std::string( comment, 60 ) );
    }

    // Read data.
    for ( ; ; )
    {
        // Peek 8 bytes.
        char buffer[8];
        fpos_t fpos;
        fgetpos( fp, &fpos );
        fseek( fp, 4, SEEK_CUR );
        fread( buffer, 1, 8, fp );
        fseek( fp, 4, SEEK_CUR );
        fsetpos( fp, &fpos );

        if ( strncmp( buffer, "#ENDFILE", 8 ) == 0 ||
             strncmp( buffer, "#NEW_SET", 8 ) == 0 ) { break; }

        kvs::gf::Data data;
        if ( !data.readBinary( fp ) ) return false;

        m_data_list.push_back( data );
    }

    return true;
}

} // end of namespace gf

} // end of namespace kvs
