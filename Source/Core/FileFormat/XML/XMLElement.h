/****************************************************************************/
/**
 *  @file XMLElement.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLElement.h 1345 2012-11-07 15:29:14Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__XML_ELEMENT_H_INCLUDE
#define KVS__XML_ELEMENT_H_INCLUDE

#include "TinyXML.h"
#include <string>
#include <kvs/String>


namespace kvs
{

/*==========================================================================*/
/**
 *  XML element class.
 */
/*==========================================================================*/
class XMLElement : public TiXmlElement
{
public:

    typedef TiXmlElement SuperClass;

public:

    XMLElement( const std::string& value );
    virtual ~XMLElement();

public:

    TiXmlNode* insert( const TiXmlNode& node );

    template <typename T>
    void setAttribute( const std::string& name, const T& value )
    {
        SuperClass::SetAttribute( name, kvs::String::ToString( value ) );
    }

public:

    static const std::string AttributeValue( const TiXmlElement* element, const std::string& name );
};

} // end of namespace kvs

#endif // KVS__XML_ELEMENT_H_INCLUDE
