/****************************************************************************/
/**
 *  @file FileChecker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FileChecker.cpp 1433 2013-03-17 01:23:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "FileChecker.h"
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
#include <kvs/File>
#include <kvs/Timer>
#include <iostream>


#define PRINT_INFO( class_name, os, checker ) \
    PrintInformation<class_name>( #class_name, os, checker )

namespace
{

/*===========================================================================*/
/**
 *  @brief  Outputs information of the file cheacker.
 *  @param  os [out] output stream
 *  @param  checker [in] file cheacker
 */
/*===========================================================================*/
template <typename FileFormatClass>
inline void PrintInformation( const char* class_name, std::ostream& os, const kvscheck::FileChecker& checker )
{
    kvs::Timer timer( kvs::Timer::Start );
    FileFormatClass file( checker.filename() );
    timer.stop();
    os << "File format class : " << class_name << std::endl;
    os << "Reading time      : " << timer.sec() << " [sec]" << std::endl;
    os << "Information       : " << std::endl;
    file.print( os, kvs::Indent(2) );
}

/*===========================================================================*/
/**
 *  @brief  Outputs information of the file cheacker for KVSML.
 *  @param  os [out] output stream
 *  @param  checker [in] file cheacker
 */
/*===========================================================================*/
inline void PrintKVSMLInformation( std::ostream& os, const kvscheck::FileChecker& checker )
{
    if ( kvs::KVSMLImageObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLImageObject, os, checker ); return;
    }

    if ( kvs::KVSMLPointObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLPointObject, os, checker ); return;
    }

    if ( kvs::KVSMLLineObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLLineObject, os, checker ); return;
    }

    if ( kvs::KVSMLPolygonObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLPolygonObject, os, checker ); return;
    }

    if ( kvs::KVSMLStructuredVolumeObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLStructuredVolumeObject, os, checker ); return;
    }

    if ( kvs::KVSMLUnstructuredVolumeObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLUnstructuredVolumeObject, os, checker ); return;
    }

    if ( kvs::KVSMLTableObject::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLTableObject, os, checker ); return;
    }

    if ( kvs::KVSMLTransferFunction::CheckFormat( checker.filename() ) )
    {
        PRINT_INFO( kvs::KVSMLTransferFunction, os, checker ); return;
    }
}

} // end of namespace


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new FileCheacker class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
FileChecker::FileChecker( const std::string& filename )
{
    m_filename = filename;
}

/*===========================================================================*/
/**
 *  @brief  Returns a filename.
 *  @return filename
 */
/*===========================================================================*/
const std::string& FileChecker::filename() const
{
    return m_filename;
}

/*===========================================================================*/
/**
 *  @brief  Returns a file format type.
 *  @return file format type
 */
/*===========================================================================*/
FileChecker::FormatType FileChecker::fileFormat() const
{
    if ( kvs::AVSField::CheckExtension( m_filename ) )
    {
        return FileChecker::AVSFieldFormat;
    }

    if ( kvs::AVSUcd::CheckExtension( m_filename ) )
    {
        return FileChecker::AVSUcdFormat;
    }

    if ( kvs::Bmp::CheckExtension( m_filename ) )
    {
        return FileChecker::BitmapFormat;
    }

    if ( kvs::Csv::CheckExtension( m_filename ) )
    {
        return FileChecker::CSVFormat;
    }

    if ( kvs::Dicom::CheckExtension( m_filename ) )
    {
        return FileChecker::DICOMFormat;
    }

    if ( kvs::Gis::CheckExtension( m_filename ) )
    {
        return FileChecker::GISFormat;
    }

    if ( kvs::GrADS::CheckExtension( m_filename ) )
    {
        return FileChecker::GrADSFormat;
    }

    if ( kvs::KVSMLPointObject::CheckExtension( m_filename ) )
    {
        /* NOTE: The KVSML object file have a same extension. Therefore,
         * kvs::KVSMLPointObject is used in order to check the file extension
         * for the KVSML format here.
         */
        return FileChecker::KVSMLFormat;
    }

    if ( kvs::Ply::CheckExtension( m_filename ) )
    {
        return FileChecker::PLYFormat;
    }

    if ( kvs::Pbm::CheckExtension( m_filename ) )
    {
        return FileChecker::PBMFormat;
    }

    if ( kvs::Pgm::CheckExtension( m_filename ) )
    {
        return FileChecker::PGMFormat;
    }

    if ( kvs::Ppm::CheckExtension( m_filename ) )
    {
        return FileChecker::PPMFormat;
    }

    if ( kvs::Stl::CheckExtension( m_filename ) )
    {
        return FileChecker::STLFormat;
    }

    if ( kvs::Tiff::CheckExtension( m_filename ) )
    {
        return FileChecker::TIFFFormat;
    }

    if ( kvs::IPLab::CheckExtension( m_filename ) )
    {
        return FileChecker::IPLabFormat;
    }

    return FileChecker::UnknownFormat;
}

/*==========================================================================*/
/**
 *  @brief  Output file information.
 *  @param  os [in] output stream
 *  @param  checker [in] file checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const FileChecker& checker )
{
    if ( !kvs::File( checker.filename() ).isExisted() )
    {
        kvsMessageError("%s is not existed.", checker.filename().c_str() );
        return os;
    }

    // KVSML format.
    if ( checker.fileFormat() == FileChecker::KVSMLFormat )
    {
        ::PrintKVSMLInformation( os, checker );
        return os;
    }

    // Other supported format.
    switch ( checker.fileFormat() )
    {
    case FileChecker::AVSFieldFormat:
        PRINT_INFO( kvs::AVSField, os, checker );
        break;

    case FileChecker::AVSUcdFormat:
        PRINT_INFO( kvs::AVSUcd, os, checker );
        break;

    case FileChecker::BitmapFormat:
        PRINT_INFO( kvs::Bmp, os, checker );
        break;

    case FileChecker::CSVFormat:
        PRINT_INFO( kvs::Csv, os, checker );
        break;

    case FileChecker::DICOMFormat:
        PRINT_INFO( kvs::Dicom, os, checker );
        break;

    case FileChecker::GISFormat:
        PRINT_INFO( kvs::Gis, os, checker );
        break;

    case FileChecker::GrADSFormat:
        PRINT_INFO( kvs::GrADS, os, checker );
        break;

    case FileChecker::PLYFormat:
        PRINT_INFO( kvs::Ply, os, checker );
        break;

    case FileChecker::PBMFormat:
        PRINT_INFO( kvs::Pbm, os, checker );
        break;

    case FileChecker::PGMFormat:
        PRINT_INFO( kvs::Pgm, os, checker );
        break;

    case FileChecker::PPMFormat:
        PRINT_INFO( kvs::Ppm, os, checker );
        break;

    case FileChecker::STLFormat:
        PRINT_INFO( kvs::Stl, os, checker );
        break;

    case FileChecker::TIFFFormat:
        PRINT_INFO( kvs::Tiff, os, checker );
        break;

    case FileChecker::IPLabFormat:
        PRINT_INFO( kvs::IPLab, os, checker );
        break;

    default:
        kvsMessageError("Unknown file format.");
        break;
    }

    return os;
}

} // end of namespace kvscheck
