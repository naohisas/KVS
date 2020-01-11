/*****************************************************************************/
/**
 *  @file   File.h
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
#pragma once
#include <string>
#include <iostream>
#include <kvs/Program>
#include <kvs/CommandLine>
#include <kvs/File>
#include <kvs/Indent>
#include <kvs/Timer>
#include <kvs/AVSField>
#include <kvs/AVSUcd>
#include <kvs/Bmp>
#include <kvs/Csv>
#include <kvs/Dicom>
#include <kvs/Gis>
#include <kvs/GrADS>
#include <kvs/KVSMLImageObject>
#include <kvs/KVSMLPointObject>
#include <kvs/KVSMLLineObject>
#include <kvs/KVSMLPolygonObject>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/KVSMLTableObject>
#include <kvs/KVSMLTransferFunction>
#include <kvs/Ply>
#include <kvs/Pbm>
#include <kvs/Pgm>
#include <kvs/Ppm>
#include <kvs/Stl>
#include <kvs/Tiff>
#include <kvs/IPLab>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  File checker class.
 */
/*===========================================================================*/
class File : public kvs::Program
{
    int exec( int argc, char** argv );
};

#define KVSCHECK_PRINT_FILE( os, fileformat, filename )                 \
    kvs::Timer timer( kvs::Timer::Start );                              \
    fileformat file( filename );                                        \
    timer.stop();                                                       \
    os << "File format class : " << #fileformat << std::endl;           \
    os << "Reading time      : " << timer.sec() << " [sec]" << std::endl; \
    os << "Information       : " << std::endl;                          \
    file.print( os, kvs::Indent(4) );

/*===========================================================================*/
/**
 *  @brief  Executes the checker program.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @return 0 if the process is done sucessfully
 */
/*===========================================================================*/
inline int File::exec( int argc, char** argv )
{
    kvs::CommandLine cl( argc, argv );
    cl.addHelpOption();
    cl.addOption( "file", "Output file information.", 0, false );
    cl.addValue( "filename", true );
    if ( !cl.parse() ) { return 1; }

    const std::string filename = cl.value<std::string>();
    if ( !kvs::File::Exists( filename ) )
    {
        kvsMessageError() << filename << " is not existed." << std::endl;
        return 1;
    }

    if ( kvs::KVSMLPointObject::CheckExtension( filename ) )
    {
        /* NOTE: The KVSML object file have a same extension. Therefore,
         * kvs::KVSMLPointObject is used in order to check the file extension
         * for the KVSML format here.
         */

        if ( kvs::KVSMLImageObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLImageObject, filename );
            return 0;
        }

        if ( kvs::KVSMLPointObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLPointObject, filename );
            return 0;
        }

        if ( kvs::KVSMLLineObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLLineObject, filename );
            return 0;
        }

        if ( kvs::KVSMLPolygonObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLPolygonObject, filename );
            return 0;
        }

        if ( kvs::KVSMLStructuredVolumeObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLStructuredVolumeObject, filename );
            return 0;
        }

        if ( kvs::KVSMLUnstructuredVolumeObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLUnstructuredVolumeObject, filename );
            return 0;
        }

        if ( kvs::KVSMLTableObject::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLTableObject, filename );
            return 0;
        }

        if ( kvs::KVSMLTransferFunction::CheckFormat( filename ) )
        {
            KVSCHECK_PRINT_FILE( std::cout, kvs::KVSMLTransferFunction, filename );
            return 0;
        }
    }

    if ( kvs::AVSField::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::AVSField, filename );
        return 0;
    }

    if ( kvs::AVSUcd::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::AVSUcd, filename );
        return 0;
    }

    if ( kvs::Bmp::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Bmp, filename );
        return 0;
    }

    if ( kvs::Csv::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Csv, filename );
        return 0;
    }

    if ( kvs::Dicom::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Dicom, filename );
        return 0;
    }

    if ( kvs::Gis::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Gis, filename );
        return 0;
    }

    if ( kvs::GrADS::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::GrADS, filename );
        return 0;
    }

    if ( kvs::Ply::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Ply, filename );
        return 0;
    }

    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Pbm, filename );
        return 0;
    }

    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Pgm, filename );
        return 0;
    }

    if ( kvs::Ppm::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Ppm, filename );
        return 0;
    }

    if ( kvs::Stl::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Stl, filename );
        return 0;
    }

    if ( kvs::Tiff::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::Tiff, filename );
        return 0;
    }

    if ( kvs::IPLab::CheckExtension( filename ) )
    {
        KVSCHECK_PRINT_FILE( std::cout, kvs::IPLab, filename );
        return 0;
    }

    kvsMessageError() << "Unknown fileformat: " << filename << "." << std::endl;
    return 1;
}

#undef KVSCHECK_PRINT_FILE

} // end of namespace kvscheck
