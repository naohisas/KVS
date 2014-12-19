/*****************************************************************************/
/**
 *  @file   PixelTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PixelTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "PixelTag.h"
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
 *  @brief  Constructs a new pixel tag class.
 */
/*===========================================================================*/
PixelTag::PixelTag():
    kvs::kvsml::TagBase( "Pixel" ),
    m_has_type( false ),
    m_type( "" )
{
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the 'type' is specified or not.
 *  @return true, if the 'type' is specified
 */
/*===========================================================================*/
bool PixelTag::hasType() const
{
    return m_has_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a pixel type as string.
 *  @return pixel type
 */
/*===========================================================================*/
const std::string& PixelTag::type() const
{
    return m_type;
}

/*===========================================================================*/
/**
 *  @brief  Sets a pixel type as string.
 *  @param  type [in] pixel type
 */
/*===========================================================================*/
void PixelTag::setType( const std::string& type )
{
    m_has_type = true;
    m_type = type;
}

/*===========================================================================*/
/**
 *  @brief  Reads the pixel tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool PixelTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // type="xxx"
    const std::string type = kvs::XMLElement::AttributeValue( element, "type" );
    if ( type != "" )
    {
        m_has_type = true;
        m_type = type;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the pixel tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool PixelTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_type )
    {
        element.setAttribute( "type", m_type );
    }
    else
    {
        kvsMessageError( "'type' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
