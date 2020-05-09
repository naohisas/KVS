/*****************************************************************************/
/**
 *  @file   ObjectTag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ObjectTag.cpp 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ObjectTag.h"
#include <kvs/Tokenizer>
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/String>
#include <sstream>


namespace
{

std::string Vec3ToString( const kvs::Vec3& v )
{
    std::ostringstream s;
    s << v[0] << " " << v[1] << " " << v[2];
    return s.str();
}

}

namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new object tag class.
 */
/*===========================================================================*/
ObjectTag::ObjectTag():
    kvs::kvsml::TagBase( "Object" ),
    m_has_external_coord( false ),
    m_has_object_coord( false ),
    m_type( "" ),
    m_min_external_coord( -3.0f, -3.0f, -3.0f ),
    m_max_external_coord(  3.0f,  3.0f,  3.0f ),
    m_min_object_coord( -3.0f, -3.0f, -3.0f ),
    m_max_object_coord(  3.0f,  3.0f,  3.0f )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns object type as string.
 */
/*===========================================================================*/
const std::string& ObjectTag::type() const
{
    return m_type;
}

/*===========================================================================*/
/**
 *  @brief  Returns a flag to check whether 'external_coord' is specified or not.
 *  @return true, if 'external_coord' is specified
 */
/*===========================================================================*/
bool ObjectTag::hasExternalCoord() const
{
    return m_has_external_coord;
}

/*===========================================================================*/
/**
 *  @brief  Returns a min. external coordinate.
 *  @return min. external coordinate
 */
/*===========================================================================*/
const kvs::Vector3f& ObjectTag::minExternalCoord() const
{
    return m_min_external_coord;
}

/*===========================================================================*/
/**
 *  @brief  Returns a max. external coordinate.
 *  @return max. external coordinate
 */
/*===========================================================================*/
const kvs::Vector3f& ObjectTag::maxExternalCoord() const
{
    return m_max_external_coord;
}

/*===========================================================================*/
/**
 *  @brief  Returns a flag to check whether 'object_coord' is specified or not.
 *  @return true, if 'object_coord' is specified
 */
/*===========================================================================*/
bool ObjectTag::hasObjectCoord() const
{
    return m_has_object_coord;
}

/*===========================================================================*/
/**
 *  @brief  Returns a min. object coordinate.
 *  @return min. object coordinate
 */
/*===========================================================================*/
const kvs::Vector3f& ObjectTag::minObjectCoord() const
{
    return m_min_object_coord;
}

/*===========================================================================*/
/**
 *  @brief  Returns a max. object coordinate.
 *  @return max. object coordinate
 */
/*===========================================================================*/
const kvs::Vector3f& ObjectTag::maxObjectCoord() const
{
    return m_max_object_coord;
}

/*===========================================================================*/
/**
 *  @brief  Sets a object type.
 *  @param  type [in] object type
 */
/*===========================================================================*/
void ObjectTag::setType( const std::string& type )
{
    m_has_type = true;
    m_type = type;
}

/*===========================================================================*/
/**
 *  @brief  Sets min/max external coordinate values.
 *  @param  min_coord [in] min. external coordinate value
 *  @param  max_coord [in] max. external coordinate value
 */
/*===========================================================================*/
void ObjectTag::setMinMaxExternalCoords( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord )
{
    m_has_external_coord = true;
    m_min_external_coord = min_coord;
    m_max_external_coord = max_coord;
}

/*===========================================================================*/
/**
 *  @brief  Sets min/max object coordinate values.
 *  @param  min_coord [in] min. object coordinate value
 *  @param  max_coord [in] max. object coordinate value
 */
/*===========================================================================*/
void ObjectTag::setMinMaxObjectCoords( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord )
{
    m_has_object_coord = true;
    m_min_object_coord = min_coord;
    m_max_object_coord = max_coord;
}

/*===========================================================================*/
/**
 *  @brief  Reads the object tag.
 *  @param  element [in] pointer to the element
 *  @return true, if the reading process is done successfully.
 */
/*===========================================================================*/
bool ObjectTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    // type="xxx"
    const std::string type = kvs::XMLElement::AttributeValue( element, "type" );
    if( type != "" )
    {
        m_has_type = true;
        m_type = type;
    }

    // external_coord="xxx xxx xxx xxx xxx xxx"
    const std::string external_coord = kvs::XMLElement::AttributeValue( element, "external_coord" );
    if ( external_coord != "" )
    {
        const std::string delim(" \n");
        kvs::Tokenizer t( external_coord, delim );

        float values[6];
        for ( size_t i = 0; i < 6; i++ )
        {
            if ( t.isLast() )
            {
                kvsMessageError( "6 components are required for 'external_coord' in <%s>", this->name().c_str() );
                return false;
            }

            values[i] = static_cast<float>( atof( t.token().c_str() ) );
        }

        m_has_external_coord = true;
        m_min_external_coord = kvs::Vector3f( values[0], values[1], values[2] );
        m_max_external_coord = kvs::Vector3f( values[3], values[4], values[5] );
    }

    // object_coord="xxx xxx xxx xxx xxx xxx"
    const std::string object_coord = kvs::XMLElement::AttributeValue( element, "object_coord" );
    if ( object_coord != "" )
    {
        const std::string delim(" \n");
        kvs::Tokenizer t( object_coord, delim );

        float values[6];
        for ( size_t i = 0; i < 6; i++ )
        {
            if ( t.isLast() )
            {
                kvsMessageError( "6 components are required for 'object_coord' in <%s>", this->name().c_str() );
                return false;
            }

            values[i] = static_cast<float>( atof( t.token().c_str() ) );
        }

        m_has_object_coord = true;
        m_min_object_coord = kvs::Vector3f( values[0], values[1], values[2] );
        m_max_object_coord = kvs::Vector3f( values[3], values[4], values[5] );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the object tag.
 *  @param  element [in] pointer to the element
 *  @return true, if the writing process is done successfully.
 */
/*===========================================================================*/
bool ObjectTag::write( kvs::XMLNode::SuperClass* parent )
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

    if ( m_has_external_coord )
    {
        const std::string min_coord = ::Vec3ToString( m_min_external_coord );
        const std::string max_coord = ::Vec3ToString( m_max_external_coord );
        const std::string value = min_coord + " " + max_coord;
        element.setAttribute( "external_coord", value );
    }

    if ( m_has_object_coord )
    {
        const std::string min_coord = ::Vec3ToString( m_min_object_coord );
        const std::string max_coord = ::Vec3ToString( m_max_object_coord );
        const std::string value = min_coord + " " + max_coord;
        element.setAttribute( "object_coord", value );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
