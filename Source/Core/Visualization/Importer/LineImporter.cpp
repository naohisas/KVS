/****************************************************************************/
/**
 *  @file   LineImporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineImporter.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "LineImporter.h"
#include <kvs/DebugNew>
#include <kvs/KVSMLLineObject>
#include <kvs/Math>
#include <kvs/Vector3>
#include <string>


namespace
{

/*==========================================================================*/
/**
 *  @brief  Returns the line type from the given string.
 *  @param  line_type [in] line type string
 *  @return line type
 */
/*==========================================================================*/
const kvs::LineObject::LineType StringToLineType( const std::string& line_type )
{
    if (      line_type == "strip"    ) { return kvs::LineObject::Strip; }
    else if ( line_type == "uniline"  ) { return kvs::LineObject::Uniline; }
    else if ( line_type == "polyline" ) { return kvs::LineObject::Polyline; }
    else if ( line_type == "segment"  ) { return kvs::LineObject::Segment; }
    else
    {
        kvsMessageError( "Unknown line type '%s'.", line_type.c_str() );
        return kvs::LineObject::UnknownLineType;
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns the line color type from the given string.
 *  @param  color_type [in] line color type string
 *  @return line color type
 */
/*==========================================================================*/
const kvs::LineObject::ColorType StringToColorType( const std::string& color_type )
{
    if (      color_type == "vertex" ) { return kvs::LineObject::VertexColor; }
    else if ( color_type == "line"   ) { return kvs::LineObject::LineColor; }
    else
    {
        kvsMessageError( "Unknown line color type '%s'.", color_type.c_str() );
        return kvs::LineObject::UnknownColorType;
    }
}

} // end of namespace


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new LineImporter class.
 */
/*==========================================================================*/
LineImporter::LineImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineImporter class.
 *  @param  filename [in] input filename
 */
/*===========================================================================*/
LineImporter::LineImporter( const std::string& filename )
{
    if ( kvs::KVSMLLineObject::CheckExtension( filename ) )
    {
        kvs::KVSMLLineObject* file_format = new kvs::KVSMLLineObject( filename );
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
 *  @brief  Constructs a new LineImporter class.
 *  @param file_format [in] pointer to the file format
 */
/*==========================================================================*/
LineImporter::LineImporter( const kvs::FileFormatBase* file_format )
{
    this->exec( file_format );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the LineImporter class.
 */
/*===========================================================================*/
LineImporter::~LineImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the import process.
 *  @param  file_format [in] pointer to the file format class
 *  @return pointer to the imported line object
 */
/*===========================================================================*/
LineImporter::SuperClass* LineImporter::exec( const kvs::FileFormatBase* file_format )
{
    if ( !file_format )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is NULL.");
        return NULL;
    }

    if ( const kvs::KVSMLLineObject* line = dynamic_cast<const kvs::KVSMLLineObject*>( file_format ) )
    {
        this->import( line );
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
 *  @brief  Imports KVSML format data.
 *  @param  kvsml [in] pointer to the KVSML document
 */
/*==========================================================================*/
void LineImporter::import( const kvs::KVSMLLineObject* kvsml )
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

    SuperClass::setLineType( ::StringToLineType( kvsml->lineType() ) );
    SuperClass::setColorType( ::StringToColorType( kvsml->colorType() ) );
    SuperClass::setCoords( kvsml->coords() );
    SuperClass::setColors( kvsml->colors() );
    SuperClass::setSizes( kvsml->sizes() );
    SuperClass::setConnections( kvsml->connections() );
//    SuperClass::updateMinMaxCoords();
    this->set_min_max_coord();
}

/*==========================================================================*/
/**
 *  @brief  Calculates the min/max coordinate values.
 */
/*==========================================================================*/
void LineImporter::set_min_max_coord()
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
