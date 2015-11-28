/****************************************************************************/
/**
 *  @file   PolygonImporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonImporter.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "PolygonImporter.h"
#include <kvs/DebugNew>
#include <kvs/KVSMLPolygonObject>
#include <kvs/Math>
#include <kvs/Vector3>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new PolygonImporter class.
 */
/*==========================================================================*/
PolygonImporter::PolygonImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonImporter class.
 *  @param  filename [in] input filename
 */
/*===========================================================================*/
PolygonImporter::PolygonImporter( const std::string& filename )
{
    if ( kvs::KVSMLPolygonObject::CheckExtension( filename ) )
    {
        BaseClass::setSuccess( SuperClass::read( filename ) );
    }
    else if ( kvs::Stl::CheckExtension( filename ) )
    {
        kvs::Stl* file_format = new kvs::Stl( filename );
        if ( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if ( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else if ( kvs::Ply::CheckExtension( filename ) )
    {
        kvs::Ply* file_format = new kvs::Ply( filename );
        if ( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if ( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Cannot import '%s'.",filename.c_str());
        return;
    }
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new PolygonImporter class.
 *  @param  file_format [in] pointer to the file format
 */
/*==========================================================================*/
PolygonImporter::PolygonImporter( const kvs::FileFormatBase* file_format )
{
    this->exec( file_format );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the PolygonImporter class.
 */
/*===========================================================================*/
PolygonImporter::~PolygonImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the import process.
 *  @param  file_format [in] pointer to the file format class
 *  @return pointer to the imported polygon object
 */
/*===========================================================================*/
PolygonImporter::SuperClass* PolygonImporter::exec( const kvs::FileFormatBase* file_format )
{
    if ( !file_format )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is NULL.");
        return NULL;
    }

    if ( dynamic_cast<const kvs::KVSMLPolygonObject*>( file_format ) )
    {
        BaseClass::setSuccess( SuperClass::read( file_format->filename() ) );
    }
    else if ( const kvs::Stl* polygon = dynamic_cast<const kvs::Stl*>( file_format ) )
    {
        this->import( polygon );
    }
    else if ( const kvs::Ply* polygon = dynamic_cast<const kvs::Ply*>( file_format ) )
    {
        this->import( polygon );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is not supported.");
        return NULL;
    }

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Imports the STL format data.
 *  @param  stl [in] pointer to the STL format file
 */
/*==========================================================================*/
void PolygonImporter::import( const kvs::Stl* stl )
{
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setColorType( kvs::PolygonObject::PolygonColor );
    SuperClass::setNormalType( kvs::PolygonObject::PolygonNormal );
    SuperClass::setCoords( stl->coords() );
    SuperClass::setColor( kvs::RGBColor( 255, 255, 255 ) );
    SuperClass::setNormals( stl->normals() );
    SuperClass::setOpacity( 255 );
    SuperClass::updateMinMaxCoords();
}

/*==========================================================================*/
/**
 *  @brief  Imports the PLY format data.
 *  @param  ply [in] pointer to the PLY format file
 */
/*==========================================================================*/
void PolygonImporter::import( const kvs::Ply* ply )
{
    SuperClass::setPolygonType( kvs::PolygonObject::Triangle );
    SuperClass::setNormalType( kvs::PolygonObject::VertexNormal );

    SuperClass::setCoords( ply->coords() );
    SuperClass::setNormals( ply->normals() );
    SuperClass::setOpacity( 255 );

    if ( ply->hasColors() )
    {
        SuperClass::setColorType( kvs::PolygonObject::VertexColor );
        SuperClass::setColors( ply->colors() );
    }
    else
    {
        SuperClass::setColorType( kvs::PolygonObject::PolygonColor );
        SuperClass::setColor( kvs::RGBColor( 255, 255, 255 ) );
    }

    if ( ply->hasConnections() )
    {
        SuperClass::setConnections( ply->connections() );
    }

    const kvs::Vec3 min_coord( ply->minCoord().x(), ply->minCoord().y(), ply->minCoord().z() );
    const kvs::Vec3 max_coord( ply->maxCoord().x(), ply->maxCoord().y(), ply->maxCoord().z() );
    SuperClass::setMinMaxObjectCoords( min_coord, max_coord );
    SuperClass::setMinMaxExternalCoords( min_coord, max_coord );
}

} // end of namespace kvs
