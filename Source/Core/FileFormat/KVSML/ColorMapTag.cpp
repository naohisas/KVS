/*****************************************************************************/
/**
 *  @file   ColorMapTag.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ColorMapTag.h"
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
 *  @brief  Constructs a new color tag class.
 */
/*===========================================================================*/
ColorMapTag::ColorMapTag():
    kvs::kvsml::TagBase( "ColorMap" )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ColorMapValueTag class.
 */
/*===========================================================================*/
ColorMapValueTag::ColorMapValueTag():
    kvs::kvsml::TagBase( "ColorMapValue" )
{
}

/*===========================================================================*/
/**
 *  @brief  Read the attribute values.
 *  @param  parent [in] pointer to the parent node
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ColorMapValueTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );
    const auto* element = kvs::XMLNode::ToElement( BaseClass::m_node );
    return this->read( element );
}

/*===========================================================================*/
/**
 *  @brief  Read the attribute values.
 *  @param  element [in] pointer to the element
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ColorMapValueTag::read( const kvs::XMLElement::SuperClass* element )
{
    const auto tag_name = BaseClass::name();

    // s ="xxx"
    const auto s = kvs::XMLElement::AttributeValue( element, "s" );
    if ( s != "" ) { m_scalar = static_cast<float>( atof( s.c_str() ) ); }
    else
    {
        kvsMessageError( "'s' is not specified in <%s>.", tag_name.c_str() );
        return false;
    }

    // r ="xxx"
    kvs::UInt8 red = 0;
    const auto r = kvs::XMLElement::AttributeValue( element, "r" );
    if ( r != "" ) { red = static_cast<kvs::UInt8>( atoi( r.c_str() ) ); }
    else
    {
        kvsMessageError( "'r' is not specified in <%s>.", tag_name.c_str() );
        return false;
    }

    // g ="xxx"
    kvs::UInt8 green = 0;
    const auto g = kvs::XMLElement::AttributeValue( element, "g" );
    if ( g != "" ) { green = static_cast<kvs::UInt8>( atoi( g.c_str() ) ); }
    else
    {
        kvsMessageError( "'g' is not specified in <%s>.", tag_name.c_str() );
        return false;
    }

    // b ="xxx"
    kvs::UInt8 blue = 0;
    const auto b = kvs::XMLElement::AttributeValue( element, "b" );
    if ( b != "" ) { blue = static_cast<kvs::UInt8>( atoi( b.c_str() ) ); }
    else
    {
        kvsMessageError( "'b' is not specified in <%s>.", tag_name.c_str() );
        return false;
    }

    m_color = kvs::RGBColor( red, green, blue );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the attribute values.
 *  @param  parent [in] pointer to the parent node
 *  @return true if the writing process is done successfully
 */
/*===========================================================================*/
bool ColorMapValueTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );
    element.setAttribute( "s", m_scalar );
    element.setAttribute( "r", m_color.r() );
    element.setAttribute( "g", m_color.g() );
    element.setAttribute( "b", m_color.b() );

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
