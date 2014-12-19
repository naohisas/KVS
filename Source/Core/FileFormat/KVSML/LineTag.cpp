/*****************************************************************************/
/**
 *  @file   LineTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "LineTag.h"
#include <kvs/Message>
#include <kvs/String>
#include <kvs/XMLNode>
#include <kvs/XMLElement>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new line tag class.
 */
/*===========================================================================*/
LineTag::LineTag():
    kvs::kvsml::TagBase( "Line" ),
    m_has_nlines( false ),
    m_nlines( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads the line tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool LineTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // nlines="xxx"
    const std::string nlines = kvs::XMLElement::AttributeValue( element, "nlines" );
    if ( nlines != "" )
    {
        m_has_nlines = true;
        m_nlines = static_cast<size_t>( atoi( nlines.c_str() ) );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the line tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool LineTag::write( kvs::XMLNode::SuperClass* parent )
{
    const std::string tag_name = BaseClass::name();
    kvs::XMLElement element( tag_name );

    if ( m_has_nlines )
    {
        element.setAttribute( "nlines", m_nlines );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
