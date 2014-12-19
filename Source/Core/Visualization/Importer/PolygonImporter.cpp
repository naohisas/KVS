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
#include <kvs/KVSMLObjectPolygon>
#include <kvs/Math>
#include <kvs/Vector3>


namespace
{

/*==========================================================================*/
/**
 *  @breif  Returns the polygon type from the given string.
 *  @param  polygon_type [in] polygon type string
 *  @return polygon type
 */
/*==========================================================================*/
const kvs::PolygonObject::PolygonType StringToPolygonType( const std::string& polygon_type )
{
    if (      polygon_type == "triangle"   ) { return kvs::PolygonObject::Triangle; }
    else if ( polygon_type == "quadrangle" ) { return kvs::PolygonObject::Quadrangle; }
    else
    {
        kvsMessageError( "Unknown polygon type '%s'.", polygon_type.c_str() );
        return kvs::PolygonObject::UnknownPolygonType;
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns the polygon color type from the given string.
 *  @param  color_type [in] polygon color type string
 *  @return polygon color type
 */
/*==========================================================================*/
const kvs::PolygonObject::ColorType StringToColorType( const std::string& color_type )
{
    if (      color_type == "vertex"  ) { return kvs::PolygonObject::VertexColor; }
    else if ( color_type == "polygon" ) { return kvs::PolygonObject::PolygonColor; }
    else
    {
        kvsMessageError( "Unknown polygon color type '%s'.", color_type.c_str() );
        return kvs::PolygonObject::UnknownColorType;
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns the polygon normal type from the given string.
 *  @param  normal_type [in] polygon normal type string
 *  @return polygon normal type
 */
/*==========================================================================*/
const kvs::PolygonObject::NormalType StringToNormalType( const std::string& normal_type )
{
    if (      normal_type == "vertex"  ) { return kvs::PolygonObject::VertexNormal; }
    else if ( normal_type == "polygon" ) { return kvs::PolygonObject::PolygonNormal; }
    else
    {
        kvsMessageError( "Unknown polygon normal type '%s'.", normal_type.c_str() );
        return kvs::PolygonObject::UnknownNormalType;
    }
}

} // end of namespace


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
    if ( kvs::KVSMLObjectPolygon::CheckExtension( filename ) )
    {
        kvs::KVSMLObjectPolygon* file_format = new kvs::KVSMLObjectPolygon( filename );
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else if ( kvs::Stl::CheckExtension( filename ) )
    {
        kvs::Stl* file_format = new kvs::Stl( filename );
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
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
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
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

    if ( const kvs::KVSMLObjectPolygon* polygon = dynamic_cast<const kvs::KVSMLObjectPolygon*>( file_format ) )
    {
        this->import( polygon );
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
 *  @breif  Imports the KVSML format data.
 *  @param  document [in] pointer to the KVSML document
 */
/*==========================================================================*/
void PolygonImporter::import( const kvs::KVSMLObjectPolygon* kvsml )
{
    if ( kvsml->objectTag().hasExternalCoord() )
    {
        const kvs::Vector3f min_coord( kvsml->objectTag().minExternalCoord() );
        const kvs::Vector3f max_coord( kvsml->objectTag().maxExternalCoord() );
        SuperClass::setMinMaxExternalCoords( min_coord, max_coord );
    }

    if ( kvsml->objectTag().hasObjectCoord() )
    {
        const kvs::Vector3f min_coord( kvsml->objectTag().minObjectCoord() );
        const kvs::Vector3f max_coord( kvsml->objectTag().maxObjectCoord() );
        SuperClass::setMinMaxObjectCoords( min_coord, max_coord );
    }

    SuperClass::setPolygonType( ::StringToPolygonType( kvsml->polygonType() ) );
    SuperClass::setColorType( ::StringToColorType( kvsml->colorType() ) );
    SuperClass::setNormalType( ::StringToNormalType( kvsml->normalType() ) );
    SuperClass::setCoords( kvsml->coords() );
    SuperClass::setColors( kvsml->colors() );
    SuperClass::setNormals( kvsml->normals() );
    SuperClass::setConnections( kvsml->connections() );
    SuperClass::setOpacities( kvsml->opacities() );
//    SuperClass::updateMinMaxCoords();
    this->set_min_max_coord();
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
    this->set_min_max_coord();
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

    const kvs::Vector3f min_coord( ply->minCoord().x(), ply->minCoord().y(), ply->minCoord().z() );
    const kvs::Vector3f max_coord( ply->maxCoord().x(), ply->maxCoord().y(), ply->maxCoord().z() );
    SuperClass::setMinMaxObjectCoords( min_coord, max_coord );
    SuperClass::setMinMaxExternalCoords( min_coord, max_coord );
}

/*==========================================================================*/
/**
 *  @brief  Calculates the min/max coordinate values.
 */
/*==========================================================================*/
void PolygonImporter::set_min_max_coord()
{
    const float min_x = SuperClass::coords()[0];
    const float min_y = SuperClass::coords()[1];
    const float min_z = SuperClass::coords()[2];
    kvs::Vector3f min_coord( min_x, min_y, min_z );
    kvs::Vector3f max_coord( min_coord );
    const size_t dimension = 3;
    const size_t nvertices = SuperClass::coords().size() / dimension;
    size_t index3 = 3;
    for ( size_t i = 1; i < nvertices; i++, index3 += 3 )
    {
        const float x = SuperClass::coords()[index3];
        const float y = SuperClass::coords()[index3+1];
        const float z = SuperClass::coords()[index3+2];

        min_coord.x() = kvs::Math::Min( min_coord.x(), x );
        min_coord.y() = kvs::Math::Min( min_coord.y(), y );
        min_coord.z() = kvs::Math::Min( min_coord.z(), z );

        max_coord.x() = kvs::Math::Max( max_coord.x(), x );
        max_coord.y() = kvs::Math::Max( max_coord.y(), y );
        max_coord.z() = kvs::Math::Max( max_coord.z(), z );
    }

    this->setMinMaxObjectCoords( min_coord, max_coord );
    this->setMinMaxExternalCoords( min_coord, max_coord );
}

} // end of namespace kvs
