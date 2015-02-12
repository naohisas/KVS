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
#include <kvs/Assert>
#include <kvs/Type>


namespace
{

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
    default: return "unknown polygon type";
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
    case kvs::PolygonObject::VertexColor: return "vertex color";
    case kvs::PolygonObject::PolygonColor: return "polygon color";
    default: return "unknown color type";
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
    case kvs::PolygonObject::VertexNormal: return "vertex normal";
    case kvs::PolygonObject::PolygonNormal: return "polygon normal";
    default: return "unknown normal type";
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
    os << static_cast<const kvs::GeometryObjectBase&>( object ) << std::endl;
#endif
    os << "Number of connections:  " << object.numberOfConnections() << std::endl;
    os << "Number of opacities:  " << object.numberOfOpacities() << std::endl;
    os << "Polygon type:  " << ::GetPolygonTypeName( object.polygonType() ) << std::endl;
    os << "Color type:  " << ::GetColorTypeName( object.colorType() ) << std::endl;
    os << "Normal type:  " << ::GetNormalTypeName( object.normalType() );

    return os;
}

} // end of namespace kvs
