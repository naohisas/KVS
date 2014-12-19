/*****************************************************************************/
/**
 *  @file   Parameter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Parameter.cpp 1774 2014-05-16 13:48:45Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Parameter.h"
#include <kvs/File>
#include <kvs/Directory>
#include <kvs/Dicom>


/*===========================================================================*/
/**
 *  @brief  Construct a new Parameter structure.
 */
/*===========================================================================*/
Parameter::Parameter( void )
{
    width = 0;
    height = 0;
    index = 0;
    window_level = 0;
    window_width = 0;
    value = 0;
    enable_show_information = true;
}

/*===========================================================================*/
/**
 *  @brief  Reads arguments.
 *  @param  argument [out] command line arguments
 *  @return true, if the reading process is done succesfully
 */
/*===========================================================================*/
const bool Parameter::read( const Argument& argument )
{
    const std::string name  = argument.value<std::string>(0);
    const bool check = !argument.hasOption("f");

    const kvs::File file( name );
    if( file.isFile() )
    {
        return( this->read_from_file( name ) );
    }

    const kvs::Directory dir( name );
    if( dir.isDirectory() )
    {
        return( this->read_from_directory( name, check ) );
    }

    kvsMessageError("Invalid argument.");
    return( false );
};

/*===========================================================================*/
/**
 *  @brief  Reads a DICOM file.
 *  @param  name [in] filename
 *  @return true, if the reading process is done succesfully
 */
/*===========================================================================*/
const bool Parameter::read_from_file( const std::string& name )
{
    // Check file existence.
    kvs::File file( name );
    if( !file.exists() )
    {
        kvsMessageError( "%s is not existed.", name.c_str() );
        return( false );
    }

    // Read a DICOM file.
    kvs::Dicom dicom( name );

    dicom_list.push_back( dicom );
    width = dicom.width();
    height = dicom.height();
    window_level = dicom.windowLevel();
    window_width = dicom.windowWidth();

    return( true );
}

/*===========================================================================*/
/**
 *  @brief  Reads DICOM files in the specified directory.
 *  @param  name [in] directory name
 *  @param  extension_check [in] check flag for the file extension
 *  @return true, if the reading process is done succesfully
 */
/*===========================================================================*/
const bool Parameter::read_from_directory( const std::string& name, const bool extension_check )
{
    // Check directory existence.
    kvs::Directory dir( name );
    if( !dir.exists() )
    {
        kvsMessageError( "%s is not existed.", name.c_str() );
        return( false );
    }

    // Read DICOM files in the specified directory.
    if ( extension_check ) dicom_list.enableExtensionCheck();
    dicom_list.read( dir.directoryPath().c_str() );
    dicom_list.sort();
    width = dicom_list[0]->width();
    height = dicom_list[0]->height();
    window_level = dicom_list[0]->windowLevel();
    window_width = dicom_list[0]->windowWidth();

    return( true );
}
