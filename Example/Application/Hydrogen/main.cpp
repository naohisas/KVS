/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Hydrogen volume data generator.
 *  @author Naohisa Sakamoto
 *
 *  Compile:\
 *  \code
 *  % kvsmake -G
 *  % kvsmake
 *  \endcode
 *
 *  Usage:\n
 *  \code
 *  % ./Hydrogen [-h|--help] <Output filename>
 *  \endcode
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1403 2012-12-07 07:35:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Argument.h"
#include "Hydrogen.h"
#include <string>
#include <iostream>
#include <kvs/Type>
#include <kvs/File>
#include <kvs/Vector3>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeExporter>
#include <kvs/AVSField>
#include <kvs/KVSMLStructuredVolumeObject>


/*===========================================================================*/
/**
 *  @brief  Main function
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    Argument arg( argc, argv );
    if ( !arg.parse() ) exit( EXIT_FAILURE );

    // Volume resolution.
    kvs::Vector3ui resolution( 64, 64, 64 );
    if ( arg.hasOption("r") )
    {
        const kvs::UInt32 rx = arg.optionValue<kvs::UInt32>("r",0);
        const kvs::UInt32 ry = arg.optionValue<kvs::UInt32>("r",1);
        const kvs::UInt32 rz = arg.optionValue<kvs::UInt32>("r",2);
        resolution = kvs::Vector3ui( rx, ry, rz );
    }

    // Output filename.
    std::string filename("hydrogen.kvsml");
    if ( arg.hasValues() )
    {
        filename = arg.value<std::string>();
    }

    // Generate hydrogen volume.
    kvs::StructuredVolumeObject* volume = new Hydrogen( resolution );
    if ( !volume )
    {
        kvsMessageError( "Cannot create a hydrogen volume." );
        return( false );
    }

    if ( arg.hasOption("verbose") )
    {
        std::cout << "> Structured volume object" << std::endl;
        volume->print( std::cout );
    }

    // Output the hydrogen volume to the file.
    const kvs::File file( filename );
    const std::string extension( file.extension() );
    if ( extension == "fld" )
    {
        kvs::AVSField* field = new kvs::StructuredVolumeExporter<kvs::AVSField>( volume );
        if ( !field )
        {
            kvsMessageError( "Cannot export hydrogen volume." );
            delete volume;
            return( false );
        }

        if ( !field->write( filename ) )
        {
            kvsMessageError( "Cannot write to the file as AVS field format." );
            delete volume;
            return( false );
        }

        if ( arg.hasOption("verbose") )
        {
            std::cout << "AVS field data format" << std::endl;
            field->print( std::cout );
        }

        delete field;
    }
    else if ( extension == "kvsml" )
    {
        kvs::KVSMLStructuredVolumeObject* kvsml =
            new kvs::StructuredVolumeExporter<kvs::KVSMLStructuredVolumeObject>( volume );
        if ( !kvsml )
        {
            kvsMessageError( "Cannot export hydrogen volume." );
            delete volume;
            return( false );
        }

        if ( arg.hasOption("a") )
        {
            kvsml->setWritingDataType( kvs::KVSMLStructuredVolumeObject::ExternalAscii );
        }

        if ( arg.hasOption("b") )
        {
            kvsml->setWritingDataType( kvs::KVSMLStructuredVolumeObject::ExternalBinary );
        }

        if ( !kvsml->write( filename ) )
        {
            kvsMessageError( "Cannot write to the file as KVSML format." );
            delete volume;
            return( false );
        }

        if ( arg.hasOption("verbose") )
        {
            std::cout << "KVS XML data format" << std::endl;
            kvsml->print( std::cout );
        }

        delete kvsml;
    }

    std::cout << "Output >> " << filename << std::endl;

    delete volume;

    return( 0 );
}
