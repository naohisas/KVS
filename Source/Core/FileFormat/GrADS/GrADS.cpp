/*****************************************************************************/
/**
 *  @file   GrADS.cpp
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GrADS.cpp 1758 2014-05-04 13:19:24Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "GrADS.h"
#include <cstdlib>
#include <cstdio>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/File>
#include <kvs/Directory>
#include <kvs/String>
#include <kvs/File>


namespace
{

const char* MonthName[] = {
    "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"
};

//const int DaysInMonth[] = {
//    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
//};

std::string ReplaceYear( const std::string& filename, const int year )
{
    if ( filename.find("%y2",0) != std::string::npos )
    {
        int y = year % 100;
        std::string replace = kvs::String::ToString( y );
        return kvs::String::Replace( filename, "%y2", replace );
    }

    if ( filename.find("%y4",0) != std::string::npos )
    {
        int y = year;
        if ( y < 100 ) y = ( 50 <= y ) ? year + 1900 : year + 2000;
        std::string replace = kvs::String::ToString( y );
        return kvs::String::Replace( filename, "%y4", replace );
    }

    return filename;
}

std::string ReplaceMonth( const std::string& filename, const int month )
{
    if ( filename.find("%m1",0) != std::string::npos )
    {
        std::string replace = kvs::String::ToString( month );
        return kvs::String::Replace( filename, "%m1", replace );
    }

    if ( filename.find("%m2",0) != std::string::npos )
    {
        char replace[3]; sprintf( replace, "%02d", month );
        return kvs::String::Replace( filename, "%m2", std::string( replace ) );
    }

    if ( filename.find("%mc",0) != std::string::npos )
    {
        return kvs::String::Replace( filename, "%mc", MonthName[ month - 1 ] );
    }

    return filename;
}

std::string ReplaceDay( const std::string& filename, const int day )
{
    if ( filename.find("%d1",0) != std::string::npos )
    {
        std::string replace = kvs::String::ToString( day );
        return kvs::String::Replace( filename, "%d1", replace );
    }

    if ( filename.find("%d2",0) != std::string::npos )
    {
        char replace[3]; sprintf( replace, "%02d", day );
        return kvs::String::Replace( filename, "%d2", std::string( replace ) );
    }

    return filename;
}

std::string ReplaceHour( const std::string& filename, const int hour )
{
    if ( filename.find("%h1",0) != std::string::npos )
    {
        std::string replace = kvs::String::ToString( hour );
        return kvs::String::Replace( filename, "%h1", replace );
    }

    if ( filename.find("%h2",0) != std::string::npos )
    {
        char replace[3]; sprintf( replace, "%02d", hour );
        return kvs::String::Replace( filename, "%h2", std::string( replace ) );
    }

    if ( filename.find("%h3",0) != std::string::npos )
    {
        char replace[4]; sprintf( replace, "%03d", hour );
        return kvs::String::Replace( filename, "%h3", std::string( replace ) );
    }

    return filename;
}

std::string ReplaceMinute( const std::string& filename, const int minute )
{
    if ( filename.find("%n2",0) != std::string::npos )
    {
        char replace[3]; sprintf( replace, "%02d", minute );
        return kvs::String::Replace( filename, "%n2", std::string( replace ) );
    }

    return filename;
}

}


namespace kvs
{

bool GrADS::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "ctl" || file.extension() == "CTL" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GrADS class.
 */
/*===========================================================================*/
GrADS::GrADS()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GrADS class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
GrADS::GrADS( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the GrADS class.
 */
/*===========================================================================*/
GrADS::~GrADS()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns data descriptor file.
 *  @return data descriptor file
 */
/*===========================================================================*/
const GrADS::DataDescriptorFile& GrADS::dataDescriptor() const
{
    return m_data_descriptor;
}

/*===========================================================================*/
/**
 *  @brief  Returns gridded binary data list.
 *  @return gridded binary data list
 */
/*===========================================================================*/
const GrADS::GriddedBinaryDataFileList& GrADS::dataList() const
{
    return m_data_list;
}

/*===========================================================================*/
/**
 *  @brief  Returns gridded binary data specified by the index.
 *  @return gridded binary data
 */
/*===========================================================================*/
const GrADS::GriddedBinaryDataFile& GrADS::data( const size_t index ) const
{
    return m_data_list[index];
}

void GrADS::print( std::ostream& os, const kvs::Indent& indent ) const
{
    m_data_descriptor.print( os, indent );
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of data files : " << m_data_list.size() << std::endl;
    GriddedBinaryDataFileList::const_iterator data = m_data_list.begin();
    size_t i = 0;
    while ( data != m_data_list.end() )
    {
        os << indent.nextIndent() << "Data file [" << i << "] : " << data->filename() << std::endl;
        i++;
        data++;
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads GrADS data.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
 bool GrADS::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Open file.
    std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );
    if( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    // Read control file.
    if ( !m_data_descriptor.read( ifs ) )
    {
        kvsMessageError( "Cannot read control file." );
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    const bool sequential = m_data_descriptor.options().find( kvs::grads::Options::Sequential );
    const bool big_endian = m_data_descriptor.options().find( kvs::grads::Options::BigEndian );

    // Read binary file. (The data is not loaded into the memory)
    std::string data_pathname;
    std::string template_data_filename = m_data_descriptor.dset().name;
    const kvs::File template_data_file( template_data_filename );
    if ( template_data_file.pathName() == template_data_file.pathName( true ) ) // absolute path
    {
        data_pathname = template_data_file.pathName();
    }
    else
    {
        data_pathname = kvs::File( filename ).pathName();
        if ( template_data_filename[0] == '^' ) { template_data_filename.erase( 0, 1 ); }
    }

    kvs::Directory directory( data_pathname );
    const kvs::FileList files = directory.fileList();

    kvs::grads::TDef tdef = m_data_descriptor.tdef();
    const size_t tnum = m_data_descriptor.tdef().num;
    for ( size_t i = 0; i < tnum; ++i, ++tdef )
    {
        std::string data_filename = template_data_filename;
        data_filename = ::ReplaceYear( data_filename, tdef.start.year );
        data_filename = ::ReplaceMonth( data_filename, tdef.start.month );
        data_filename = ::ReplaceDay( data_filename, tdef.start.day );
        data_filename = ::ReplaceHour( data_filename, tdef.start.hour );
        data_filename = ::ReplaceMinute( data_filename, tdef.start.minute );

        kvs::FileList::const_iterator file = files.begin();
        kvs::FileList::const_iterator last = files.end();
        while ( file != last )
        {
            if ( file->fileName() == data_filename )
            {
                const std::string sep = kvs::File::Separator();
                const std::string path = directory.path( true );
                GriddedBinaryDataFile::Date date;
                date.year = tdef.start.year;
                date.month = tdef.start.month;
                date.day = tdef.start.day;
                date.hour = tdef.start.hour;
                date.minute = tdef.start.minute;

                GriddedBinaryDataFile data;
                data.setFilename( path + sep + data_filename );
                data.setSequential( sequential );
                data.setBigEndian( big_endian );
                data.setDate( date );
                m_data_list.push_back( data );
                break;
            }

            ++file;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes GrADS data.
 *  @param  filename [in] filename
 *  @return false (not yet implemented)
 */
/*===========================================================================*/
bool GrADS::write( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );

    return false;
}

} // end of namespace kvs
