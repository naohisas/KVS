/*****************************************************************************/
/**
 *  @file   XML.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once

#include <string>
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <kvs/XMLDocument>
#include <kvs/Assert>


namespace kvs
{

namespace bdml
{

typedef kvs::XMLNode::SuperClass Node;
typedef kvs::XMLElement::SuperClass Element;
typedef TiXmlText Text;

inline Node* FindNode( const Node* parent, const std::string& name )
{
    KVS_ASSERT( parent );
    return kvs::XMLNode::FindChildNode( parent, name );
}

inline const Element* ToElement( const Node* node )
{
    KVS_ASSERT( node );
    return kvs::XMLNode::ToElement( node );
}

inline const Text* ToText( const Node* node )
{
    KVS_ASSERT( node );
    return kvs::XMLNode::ToText( node );
}

template <typename T>
inline T AttributeValue( const Element* element, const std::string& name )
{
    KVS_ASSERT( element );
    return kvs::String::To<T>( kvs::XMLElement::AttributeValue( element, name ) );
}

template <>
inline std::string AttributeValue<std::string>( const Element* element, const std::string& name )
{
    KVS_ASSERT( element );
    return kvs::XMLElement::AttributeValue( element, name );
}

template <typename T>
inline T NodeValue( const Node* node )
{
    KVS_ASSERT( node );
    KVS_ASSERT( ToText( node ) );
    return kvs::String::To<T>( ToText( node )->Value() );
}

template <>
inline std::string NodeValue<std::string>( const Node* node )
{
    KVS_ASSERT( node );
    KVS_ASSERT( ToText( node ) );
    return ToText( node )->Value();
}

} // end of namespace bdml

} // end of namespace kvs
