/*****************************************************************************/
/**
 *  @file   OpacityMapTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpacityMapTag.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "OpacityMapTag.h"
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
 *  @brief  Constructs a new opacity map tag class.
 */
/*===========================================================================*/
OpacityMapTag::OpacityMapTag():
    kvs::kvsml::TagBase( "OpacityMap" )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new OpacityMapValueTag class.
 */
/*===========================================================================*/
OpacityMapValueTag::OpacityMapValueTag():
    kvs::kvsml::TagBase( "OpacityMapValue" )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the scalar value.
 *  @return scalar value
 */
/*===========================================================================*/
float OpacityMapValueTag::scalar() const
{
    return m_scalar;
}

/*===========================================================================*/
/**
 *  @brief  Returns the opacity value.
 *  @return opacity value
 */
/*===========================================================================*/
float OpacityMapValueTag::opacity() const
{
    return m_opacity;
}

/*===========================================================================*/
/**
 *  @brief  Sets a scalar value.
 *  @param  scalar [in] scalar value
 */
/*===========================================================================*/
void OpacityMapValueTag::setScalar( const float scalar )
{
    m_scalar = scalar;
}

/*===========================================================================*/
/**
 *  @brief  Sets an opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void OpacityMapValueTag::setOpacity( const float opacity )
{
    m_opacity = opacity;
}

/*===========================================================================*/
/**
 *  @brief  Reads the attribute value.
 *  @param  parent [in] pointer to the parent node
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool OpacityMapValueTag::read( const kvs::XMLNode::SuperClass* parent )
{
    BaseClass::read( parent );

    // Element
    const kvs::XMLElement::SuperClass* element = kvs::XMLNode::ToElement( BaseClass::m_node );

    return this->read( element );
}

/*===========================================================================*/
/**
 *  @brief  Reads the attribute value.
 *  @param  parent [in] pointer to the parent element
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool OpacityMapValueTag::read( const kvs::XMLElement::SuperClass* element )
{
    const std::string tag_name = BaseClass::name();

    // s ="xxx"
    const std::string s = kvs::XMLElement::AttributeValue( element, "s" );
    if ( s != "" ) m_scalar = static_cast<float>( atof( s.c_str() ) );
    else
    {
        kvsMessageError( "'s' is not specified in <%s>.", tag_name.c_str() );
        return false;
    }

    // a ="xxx"
    const std::string a = kvs::XMLElement::AttributeValue( element, "a" );
    if ( a != "" ) m_opacity = static_cast<float>( atof( a.c_str() ) );
    else
    {
        kvsMessageError( "'a' is not specified in <%s>.", tag_name.c_str() );
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the attribute value.
 *  @param  parent [in] pointer to the parent node
 *  @return true if the writing process is done successfully
 */
/*===========================================================================*/
bool OpacityMapValueTag::write( kvs::XMLNode::SuperClass* parent )
{
    kvs::XMLElement element( BaseClass::name() );

    {
        element.setAttribute( "s", m_scalar );
    }

    {
        element.setAttribute( "a", m_opacity );
    }

    return BaseClass::write_with_element( parent, element );
}

} // end of namespace kvsml

} // end of namespace kvs
