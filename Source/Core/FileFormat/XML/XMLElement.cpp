/****************************************************************************/
/**
 *  @file XMLElement.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLElement.cpp 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "XMLElement.h"
#include "XMLNode.h"
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 *  @param value [in] element value
 */
/*==========================================================================*/
XMLElement::XMLElement( const std::string& value ):
    TiXmlElement( value )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
XMLElement::~XMLElement()
{
}

/*==========================================================================*/
/**
 *  Insert node to the element.
 *  @param node [in] node
 *  @return pointer to the node
 */
/*==========================================================================*/
TiXmlNode* XMLElement::insert( const TiXmlNode& node )
{
    return SuperClass::InsertEndChild( node );
}

const std::string XMLElement::AttributeValue( const TiXmlElement* element, const std::string& name )
{
    const std::string* value = element->Attribute( name );
    if( !value ) return "";

    return *value;
}

} // end of namespace kvs
