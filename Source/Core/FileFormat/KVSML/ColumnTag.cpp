/*****************************************************************************/
/**
 *  @file   ColumnTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColumnTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ColumnTag.h"
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
 *  @brief  Constructs a new ColumnTag class.
 */
/*===========================================================================*/
ColumnTag::ColumnTag():
    kvs::kvsml::TagBase( "Column" ),
    m_has_label( false ),
    m_has_min_value( false ),
    m_has_max_value( false ),
    m_has_min_range( false ),
    m_has_max_range( false ),
    m_label(""),
    m_min_value(0),
    m_max_value(0),
    m_min_range(0),
    m_max_range(0)
{
}

/*===========================================================================*/
/**
 *  @brief  Reads column data.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ColumnTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    return this->read( element );
}

/*===========================================================================*/
/**
 *  @brief  Reads the data from the element.
 *  @param  element [in] pointer to the element
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ColumnTag::read( const kvs::XMLElement::SuperClass* element )
{
    m_has_label = false;
    m_has_min_value = false;
    m_has_max_value = false;
    m_has_min_range = false;
    m_has_max_range = false;
    m_label = "";
    m_min_value = 0.0;
    m_max_value = 0.0;
    m_min_range = 0.0;
    m_max_range = 0.0;

    // label="xxx"
    const std::string label = kvs::XMLElement::AttributeValue( element, "label" );
    if ( label != "" ) { this->setLabel( label ); }

    // min_value="xxx"
    const std::string min_value = kvs::XMLElement::AttributeValue( element, "min_value" );
    if ( min_value != "" ) { this->setMinValue( atof( min_value.c_str() ) ); }

    // max_value="xxx"
    const std::string max_value = kvs::XMLElement::AttributeValue( element, "max_value" );
    if ( max_value != "" ) { this->setMaxValue( atof( max_value.c_str() ) ); }

    // min_range="xxx"
    const std::string min_range = kvs::XMLElement::AttributeValue( element, "min_range" );
    if ( min_range != "" ) { this->setMinRange( atof( min_range.c_str() ) ); }

    // max_range="xxx"
    const std::string max_range = kvs::XMLElement::AttributeValue( element, "max_range" );
    if ( max_range != "" ) { this->setMaxRange( atof( max_range.c_str() ) ); }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the column data.
 *  @param  parent [in] pointer to the parent node
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool ColumnTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    if ( m_has_label )
    {
        element.setAttribute( "label", m_label );
    }

    if ( m_has_min_value )
    {
        element.setAttribute( "min_value", m_min_value );
    }

    if ( m_has_max_value )
    {
        element.setAttribute( "max_value", m_max_value );
    }

    if ( m_has_min_range )
    {
        element.setAttribute( "min_range", m_min_range );
    }

    if ( m_has_max_range )
    {
        element.setAttribute( "max_range", m_max_range );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
