/****************************************************************************/
/**
 *  @file   PolygonObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonObject.cpp 1759 2014-05-05 06:14:18Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "PolygonObject.h"
#include <string>
#include <kvs/KVSMLPolygonObject>
#include <kvs/Assert>
#include <kvs/Type>


namespace
{

/*==========================================================================*/
/**
 *  @breif  Returns the polygon type from the given string.
 *  @param  polygon_type [in] polygon type string
 *  @return polygon type
 */
/*==========================================================================*/
kvs::PolygonObject::PolygonType GetPolygonType( const std::string& polygon_type )
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
kvs::PolygonObject::ColorType GetColorType( const std::string& color_type )
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
kvs::PolygonObject::NormalType GetNormalType( const std::string& normal_type )
{
    if (      normal_type == "vertex"  ) { return kvs::PolygonObject::VertexNormal; }
    else if ( normal_type == "polygon" ) { return kvs::PolygonObject::PolygonNormal; }
    else
    {
        kvsMessageError( "Unknown polygon normal type '%s'.", normal_type.c_str() );
        return kvs::PolygonObject::UnknownNormalType;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the name of the polygon type as string.
 *  @param  type [in] polygon type
 *  @return name of the polygon type
 */
/*===========================================================================*/
const std::string GetPolygonTypeName( const kvs::PolygonObject::PolygonType type )
{
    switch( type )
    {
    case kvs::PolygonObject::Triangle: return "triangle";
    case kvs::PolygonObject::Quadrangle: return "quadrangle";
    default: return "unknown";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the name of the color type as string.
 *  @param  type [in] color type
 *  @return name of the color type
 */
/*===========================================================================*/
const std::string GetColorTypeName( const kvs::PolygonObject::ColorType type )
{
    switch( type )
    {
    case kvs::PolygonObject::VertexColor: return "vertex";
    case kvs::PolygonObject::PolygonColor: return "polygon";
    default: return "unknown";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the name of the normal type as string.
 *  @param  type [in] normal type
 *  @return name of the normal type
 */
/*===========================================================================*/
const std::string GetNormalTypeName( const kvs::PolygonObject::NormalType type )
{
    switch( type )
    {
    case kvs::PolygonObject::VertexNormal: return "vertex";
    case kvs::PolygonObject::PolygonNormal: return "polygon";
    default: return "unknown";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type.
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLPolygonObject::WritingDataType GetWritingDataType( const bool ascii, const bool external )
{
    if ( ascii )
    {
        if ( external ) { return kvs::KVSMLPolygonObject::ExternalAscii; }
        else { return kvs::KVSMLPolygonObject::Ascii; }
    }
    else
    {
        return kvs::KVSMLPolygonObject::ExternalBinary;
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonObject class.
 */
/*===========================================================================*/
PolygonObject::PolygonObject()
{
    BaseClass::setGeometryType( Polygon );
    this->setOpacity( 255 );
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys.
 *  @param  object [in] polygon object
 */
/*===========================================================================*/
void PolygonObject::shallowCopy( const PolygonObject& object )
{
    BaseClass::shallowCopy( object );
    m_polygon_type = object.polygonType();
    m_color_type = object.colorType();
    m_normal_type = object.normalType();
    m_connections = object.connections();
    m_opacities = object.opacities();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys.
 *  @param  object [in] polygon object
 */
/*===========================================================================*/
void PolygonObject::deepCopy( const PolygonObject& object )
{
    BaseClass::deepCopy( object );
    m_polygon_type = object.polygonType();
    m_color_type = object.colorType();
    m_normal_type = object.normalType();
    m_connections = object.connections().clone();
    m_opacities = object.opacities().clone();
}

/*===========================================================================*/
/**
 *  @brief  Clears the data of the polygon object.
 */
/*===========================================================================*/
void PolygonObject::clear()
{
    BaseClass::clear();
    m_connections.release();
    m_opacities.release();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the polygon object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void PolygonObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Object type : " << "polygon object" << std::endl;
    BaseClass::print( os, indent );
    os << indent << "Number of connections : " << this->numberOfConnections() << std::endl;
    os << indent << "Number of opacities : " << this->numberOfOpacities() << std::endl;
    os << indent << "Polygon type : " << ::GetPolygonTypeName( this->polygonType() ) << std::endl;
    os << indent << "Color type : " << ::GetColorTypeName( this->colorType() ) << std::endl;
    os << indent << "Normal type : " << ::GetNormalTypeName( this->normalType() ) << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read a polygon object from the specified file in KVSML.
 *  @param  filename [in] input filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool PolygonObject::read( const std::string& filename )
{
    if ( !kvs::KVSMLPolygonObject::CheckExtension( filename ) )
    {
        kvsMessageError("%s is not a polygon object file in KVSML.", filename.c_str());
        return false;
    }

    kvs::KVSMLPolygonObject kvsml;
    if ( !kvsml.read( filename ) ) { return false; }

    this->setPolygonType( ::GetPolygonType( kvsml.polygonType() ) );
    this->setColorType( ::GetColorType( kvsml.colorType() ) );
    this->setNormalType( ::GetNormalType( kvsml.normalType() ) );
    this->setCoords( kvsml.coords() );
    this->setColors( kvsml.colors() );
    this->setNormals( kvsml.normals() );
    this->setConnections( kvsml.connections() );
    this->setOpacities( kvsml.opacities() );

    if ( kvsml.hasExternalCoord() )
    {
        const kvs::Vec3 min_coord( kvsml.minExternalCoord() );
        const kvs::Vec3 max_coord( kvsml.maxExternalCoord() );
        this->setMinMaxExternalCoords( min_coord, max_coord );
    }

    if ( kvsml.hasObjectCoord() )
    {
        const kvs::Vec3 min_coord( kvsml.minObjectCoord() );
        const kvs::Vec3 max_coord( kvsml.maxObjectCoord() );
        this->setMinMaxObjectCoords( min_coord, max_coord );
    }
    else
    {
        this->updateMinMaxCoords();
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the polygon object to the specfied file in KVSML.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool PolygonObject::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::KVSMLPolygonObject kvsml;
    kvsml.setWritingDataType( ::GetWritingDataType( ascii, external ) );
    kvsml.setPolygonType( ::GetPolygonTypeName( this->polygonType() ) );
    kvsml.setColorType( ::GetColorTypeName( this->colorType() ) );
    kvsml.setNormalType( ::GetNormalTypeName( this->normalType() ) );
    kvsml.setCoords( this->coords() );
    kvsml.setColors( this->colors() );
    kvsml.setConnections( this->connections() );
    kvsml.setNormals( this->normals() );
    kvsml.setOpacities( this->opacities() );

    if ( this->hasMinMaxObjectCoords() )
    {
        kvsml.setMinMaxObjectCoords( this->minObjectCoord(), this->maxObjectCoord() );
    }

    if ( this->hasMinMaxExternalCoords() )
    {
        kvsml.setMinMaxExternalCoords( this->minExternalCoord(), this->maxExternalCoord() );
    }

    return kvsml.write( filename );
}

/*===========================================================================*/
/**
 *  @brief  Sets a color value.
 *  @param  color [in] color value
 */
/*===========================================================================*/
void PolygonObject::setColor( const kvs::RGBColor& color )
{
    BaseClass::setColor( color );
    m_color_type = PolygonObject::PolygonColor;
}

/*===========================================================================*/
/**
 *  @brief  Sets an opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void PolygonObject::setOpacity( const kvs::UInt8 opacity )
{
    m_opacities.allocate( 1 );
    m_opacities[0] = opacity;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of connections.
 *  @return number of connections
 */
/*===========================================================================*/
size_t PolygonObject::numberOfConnections() const
{
    const size_t nvertices_per_face = m_polygon_type;
    return nvertices_per_face == 0 ? 0 : m_connections.size() / nvertices_per_face;
}

std::ostream& operator << ( std::ostream& os, const PolygonObject& object )
{
    os << "Object type:  " << "polygon object" << std::endl;
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
//    os << static_cast<const kvs::GeometryObjectBase&>( object ) << std::endl;
    static_cast<const kvs::GeometryObjectBase&>( object ).print( os );
#endif
    os << "Number of connections:  " << object.numberOfConnections() << std::endl;
    os << "Number of opacities:  " << object.numberOfOpacities() << std::endl;
    os << "Polygon type:  " << ::GetPolygonTypeName( object.polygonType() ) << std::endl;
    os << "Color type:  " << ::GetColorTypeName( object.colorType() ) << std::endl;
    os << "Normal type:  " << ::GetNormalTypeName( object.normalType() );

    return os;
}

} // end of namespace kvs
