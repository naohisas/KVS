/*****************************************************************************/
/**
 *  @file   XMLNode.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLNode.h 1304 2012-09-15 01:23:48Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__XML_NODE_H_INCLUDE
#define KVS__XML_NODE_H_INCLUDE

#include "TinyXML.h"
#include <string>


namespace kvs
{

class XMLElement;

/*==========================================================================*/
/**
 *  XML node class.
 */
/*==========================================================================*/
class XMLNode : public TiXmlNode
{
public:

    typedef TiXmlNode SuperClass;

public:

    XMLNode();

public:

    static TiXmlNode* FindChildNode( const TiXmlNode* node, const std::string& node_name );
    static TiXmlElement* ToElement( const TiXmlNode* node );
    static TiXmlText* ToText( const TiXmlNode* node );
};

} // end of namespace kvs

#endif // KVS__XML_NODE_H_INCLUDE
