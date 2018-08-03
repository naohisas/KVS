/****************************************************************************/
/**
 *  @file ObjectImporter.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectImporter.cpp 1399 2012-12-06 15:27:59Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ObjectImporter.h"
#include <string>
#include <kvs/DebugNew>
#include <kvs/File>
#include <kvs/AVSField>
#include <kvs/AVSUcd>
#include <kvs/Bmp>
#include <kvs/Ppm>
#include <kvs/Pgm>
#include <kvs/Pbm>
#include <kvs/Stl>
#include <kvs/Ply>
#include <kvs/Tiff>
#include <kvs/Dicom>
#include <kvs/IPLab>
#include <kvs/KVSMLImageObject>
#include <kvs/KVSMLPointObject>
#include <kvs/KVSMLLineObject>
#include <kvs/KVSMLPolygonObject>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/XMLDocument>
#include <kvs/DicomList>
#include <kvs/PointImporter>
#include <kvs/LineImporter>
#include <kvs/PolygonImporter>
#include <kvs/StructuredVolumeImporter>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/ImageImporter>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ObjectImporter class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
ObjectImporter::ObjectImporter( const std::string& filename ):
    m_filename( filename ),
    m_importer_type( ObjectImporter::Unknown ),
    m_file_format( NULL ),
    m_importer( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ObjectImporter class.
 */
/*===========================================================================*/
ObjectImporter::~ObjectImporter()
{
    // NOTE: Never delete the m_importer in this class,
    //       since it is shared by kvs::PipelineModule.
    if ( m_file_format ) delete m_file_format;
}

/*===========================================================================*/
/**
 *  @brief  Imports the object data.
 *  @return pointer to the imported object
 */
/*===========================================================================*/
kvs::ObjectBase* ObjectImporter::import()
{
    if ( !this->estimate_file_format() )
    {
        kvsMessageError( "Cannot create a file format class for '%s'.", m_filename.c_str() );
        return NULL;
    }

    if ( !this->estimate_importer() )
    {
        kvsMessageError( "Cannot create a importer class for '%s'.", m_filename.c_str() );
        return NULL;
    }

    if ( !m_file_format->read( m_filename ) )
    {
        kvsMessageError( "Cannot read a '%s'.", m_filename.c_str() );
        return NULL;
    }

    kvs::ObjectBase* object = m_importer->exec( m_file_format );
    if ( !object )
    {
        kvsMessageError( "Cannot import a object." );

        // NOTE: Delete m_importer only when the memory allocation
        //       of m_importer is failed.
        delete m_importer;

        return NULL;
    }

    return object;
}

/*===========================================================================*/
/**
 *  @brief  Estimates file format.
 *  @return true, if the specified file is supported in KVS
 */
/*===========================================================================*/
bool ObjectImporter::estimate_file_format()
{
    kvs::File file( m_filename );
    if ( kvs::AVSField::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::StructuredVolume;
        m_file_format = new kvs::AVSField;
    }

    else if ( kvs::AVSUcd::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::UnstructuredVolume;
        m_file_format = new kvs::AVSUcd;
    }

    else if ( kvs::Bmp::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::Bmp;
    }

    else if ( kvs::Ppm::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::Ppm;
    }

    else if ( kvs::Pgm::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::Pgm;
    }

    else if ( kvs::Pbm::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::Pbm;
    }

    else if ( kvs::Stl::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Polygon;
        m_file_format = new kvs::Stl;
    }

    else if ( kvs::Ply::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Polygon;
        m_file_format = new kvs::Ply;
    }

    else if ( kvs::Tiff::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::Tiff;
    }

    else if ( kvs::Dicom::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::Dicom;
    }

    else if ( kvs::IPLab::CheckExtension( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::Image;
        m_file_format = new kvs::IPLab;
    }

    else if ( file.extension() == "kvsml" ||
              file.extension() == "KVSML" ||
              file.extension() == "xml"   ||
              file.extension() == "XML" )
    {
        if ( kvs::KVSMLImageObject::CheckFormat( file.filePath() ) )
        {
            m_importer_type = ObjectImporter::Image;
            m_file_format = new kvs::KVSMLImageObject;
        }

        else if ( kvs::KVSMLPointObject::CheckFormat( file.filePath() ) )
        {
            m_importer_type = ObjectImporter::Point;
            m_file_format = new kvs::KVSMLPointObject;
        }

        else if ( kvs::KVSMLLineObject::CheckFormat( file.filePath() ) )
        {
            m_importer_type = ObjectImporter::Line;
            m_file_format = new kvs::KVSMLLineObject;
        }

        else if ( kvs::KVSMLPolygonObject::CheckFormat( file.filePath() ) )
        {
            m_importer_type = ObjectImporter::Polygon;
            m_file_format = new kvs::KVSMLPolygonObject;
        }

        else if ( kvs::KVSMLStructuredVolumeObject::CheckFormat( file.filePath() ) )
        {
            m_importer_type = ObjectImporter::StructuredVolume;
            m_file_format = new kvs::KVSMLStructuredVolumeObject;
        }

        else if ( kvs::KVSMLUnstructuredVolumeObject::CheckFormat( file.filePath() ) )
        {
            m_importer_type = ObjectImporter::UnstructuredVolume;
            m_file_format = new kvs::KVSMLUnstructuredVolumeObject;
        }
    }

    else if ( kvs::DicomList::CheckDirectory( file.filePath() ) )
    {
        m_importer_type = ObjectImporter::StructuredVolume;
        m_file_format = new kvs::DicomList;
    }

    return m_file_format != NULL;
}

/*===========================================================================*/
/**
 *  @brief  Estimates a importer for the specified file.
 *  @return true, if the file is supported in KVS
 */
/*===========================================================================*/
bool ObjectImporter::estimate_importer()
{
    switch( m_importer_type )
    {
    case ObjectImporter::Point:
    {
        m_importer = new kvs::PointImporter;
        break;
    }
    case ObjectImporter::Line:
    {
        m_importer = new kvs::LineImporter;
        break;
    }
    case ObjectImporter::Polygon:
    {
        m_importer = new kvs::PolygonImporter;
        break;
    }
    case ObjectImporter::StructuredVolume:
    {
        m_importer = new kvs::StructuredVolumeImporter;
        break;
    }
    case ObjectImporter::UnstructuredVolume:
    {
        m_importer = new kvs::UnstructuredVolumeImporter;
        break;
    }
    case ObjectImporter::Image:
    {
        m_importer = new kvs::ImageImporter;
        break;
    }
    default: break;
    }

    return m_importer != NULL;
}

} // end of namespace kvs
