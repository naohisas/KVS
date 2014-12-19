/*****************************************************************************/
/**
 *  @file   PolygonObjectTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonObjectTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "PolygonObjectTag.h"
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/Tokenizer>
#include <kvs/String>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new polygon object tag class.
 */
/*===========================================================================*/
PolygonObjectTag::PolygonObjectTag():
    kvs::kvsml::TagBase( "PolygonObject" ),
    m_has_polygon_type( false ),
    m_polygon_type( "" ),
    m_has_color_type( false ),
    m_color_type( "" ),
    m_has_normal_type( false ),
    m_normal_type( "" )
{
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the 'polygon_type' is specified or not.
 *  @return true, if the 'polygon_type' is specified
 */
/*===========================================================================*/
bool PolygonObjectTag::hasPolygonType() const
{
    return m_has_polygon_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a polygon type as string.
 *  @return polygon type
 */
/*===========================================================================*/
const std::string& PolygonObjectTag::polygonType() const
{
    return m_polygon_type;
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the 'color_type' is specified or not.
 *  @return true, if the 'color_type' is specified
 */
/*===========================================================================*/
bool PolygonObjectTag::hasColorType() const
{
    return m_has_color_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a color type as string.
 *  @return polygon type
 */
/*===========================================================================*/
const std::string& PolygonObjectTag::colorType() const
{
    return m_color_type;
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the 'normal_type' is specified or not.
 *  @return true, if the 'normal_type' is specified
 */
/*===========================================================================*/
bool PolygonObjectTag::hasNormalType() const
{
    return m_has_normal_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a normal type as string.
 *  @return normal type
 */
/*===========================================================================*/
const std::string& PolygonObjectTag::normalType() const
{
    return m_normal_type;
}

/*===========================================================================*/
/**
 *  @brief  Sets a polygon type as string.
 *  @param  polygon_type [in] polygon type
 */
/*===========================================================================*/
void PolygonObjectTag::setPolygonType( const std::string& polygon_type )
{
    m_has_polygon_type = true;
    m_polygon_type = polygon_type;
}

/*===========================================================================*/
/**
 *  @brief  Sets a color type as string.
 *  @param  color_type [in] color type
 */
/*===========================================================================*/
void PolygonObjectTag::setColorType( const std::string& color_type )
{
    m_has_color_type = true;
    m_color_type = color_type;
}

/*===========================================================================*/
/**
 *  @brief  Sets a normal type as string.
 *  @param  normal_type [in] normal type
 */
/*===========================================================================*/
void PolygonObjectTag::setNormalType( const std::string& normal_type )
{
    m_has_normal_type = true;
    m_normal_type = normal_type;
}

/*===========================================================================*/
/**
 *  @brief  Reads the polygon object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool PolygonObjectTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // polygon_type="xxx"
    const std::string polygon_type = kvs::XMLElement::AttributeValue( element, "polygon_type" );
    if ( polygon_type != "" )
    {
        m_has_polygon_type = true;
        m_polygon_type = polygon_type;
    }

    // color_type="xxx"
    const std::string color_type = kvs::XMLElement::AttributeValue( element, "color_type" );
    if ( color_type != "" )
    {
        m_has_color_type = true;
        m_color_type = color_type;
    }

    // normal_type="xxx"
    const std::string normal_type = kvs::XMLElement::AttributeValue( element, "normal_type" );
    if ( normal_type != "" )
    {
        m_has_normal_type = true;
        m_normal_type = normal_type;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the polygon object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool PolygonObjectTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_polygon_type )
    {
        element.setAttribute( "polygon_type", m_polygon_type );
    }
    else
    {
        kvsMessageError( "'polygon_type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    if ( m_has_color_type )
    {
        element.setAttribute( "color_type", m_color_type );
    }
    else
    {
        kvsMessageError( "'color_type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    if ( m_has_normal_type )
    {
        element.setAttribute( "normal_type", m_normal_type );
    }
    else
    {
        kvsMessageError( "'normal_type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
