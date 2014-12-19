/*****************************************************************************/
/**
 *  @file   ValueTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ValueTag.cpp 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ValueTag.h"
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
 *  @brief  Constructs a new value tag class.
 */
/*===========================================================================*/
ValueTag::ValueTag():
    kvs::kvsml::TagBase( "Value" ),
    m_has_label( false ),
    m_has_unit( false ),
    m_has_veclen( false ),
    m_has_min_value( false ),
    m_has_max_value( false ),
    m_label( "" ),
    m_unit( "" ),
    m_veclen( 0 ),
    m_min_value( 0.0 ),
    m_max_value( 0.0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Reads the value tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ValueTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // label="xxx"
    const std::string label = kvs::XMLElement::AttributeValue( element, "label" );
    if ( label != "" )
    {
        m_has_label = true;
        m_label = label;
    }

    // unit="xxx"
    const std::string unit = kvs::XMLElement::AttributeValue( element, "unit" );
    if ( unit != "" )
    {
        m_has_unit = true;
        m_unit = unit;
    }

    // veclen="xxx"
    const std::string veclen = kvs::XMLElement::AttributeValue( element, "veclen" );
    if ( veclen != "" )
    {
        m_has_veclen = true;
        m_veclen = static_cast<size_t>( atoi( veclen.c_str() ) );
    }

    // min_value="xxx"
    const std::string min_value = kvs::XMLElement::AttributeValue( element, "min_value" );
    if ( min_value != "" )
    {
        m_has_min_value = true;
        m_min_value = atof( min_value.c_str() );
    }

    // max_value="xxx"
    const std::string max_value = kvs::XMLElement::AttributeValue( element, "max_value" );
    if ( max_value != "" )
    {
        m_has_max_value = true;
        m_max_value = atof( max_value.c_str() );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the value tag.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool ValueTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_label )
    {
        element.setAttribute( "label", m_label );
    }

    if ( m_has_unit )
    {
        element.setAttribute( "unit", m_unit );
    }

    if ( m_has_veclen )
    {
        element.setAttribute( "veclen", m_veclen );
    }
    else
    {
        kvsMessageError( "'veclen' is not specified in <%s>.", BaseClass::name().c_str() );
        return false;
    }

    if ( m_has_min_value )
    {
        element.setAttribute( "min_value", m_min_value );
    }

    if ( m_has_max_value )
    {
        element.setAttribute( "max_value", m_max_value );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
