/*****************************************************************************/
/**
 *  @file   LineObjectTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineObjectTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "LineObjectTag.h"
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
 *  @brief  Constructs a new line object tag class.
 */
/*===========================================================================*/
LineObjectTag::LineObjectTag():
    kvs::kvsml::TagBase( "LineObject" ),
    m_has_line_type( false ),
    m_has_color_type( false ),
    m_line_type( "" ),
    m_color_type( "" )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads the line object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool LineObjectTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // line_type="xxx"
    const std::string line_type = kvs::XMLElement::AttributeValue( element, "line_type" );
    if ( line_type != "" )
    {
        m_has_line_type = true;
        m_line_type = line_type;
    }

    // color_type="xxx"
    const std::string color_type = kvs::XMLElement::AttributeValue( element, "color_type" );
    if ( color_type != "" )
    {
        m_has_color_type = true;
        m_color_type = color_type;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the line object tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool LineObjectTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_line_type )
    {
        element.setAttribute( "line_type", m_line_type );
    }
    else
    {
        kvsMessageError( "'line_type' is not specified in <%s>.", BaseClass::name().c_str() );
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

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
