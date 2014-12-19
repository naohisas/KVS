/****************************************************************************/
/**
 *  @file XMLDeclaration.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLDeclaration.h 1304 2012-09-15 01:23:48Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__XML_DECLARATION_H_INCLUDE
#define KVS__XML_DECLARATION_H_INCLUDE

#include "TinyXML.h"
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  XML declaration class.
 */
/*==========================================================================*/
class XMLDeclaration : public TiXmlDeclaration
{
public:

    typedef TiXmlDeclaration SuperClass;

public:

    XMLDeclaration();
    XMLDeclaration(
        const std::string& version,
        const std::string& encoding = "",
        const std::string& standalone = "" );
    virtual ~XMLDeclaration();
};

} // end of namespace kvs

#endif // KVS__XML_DECLARATION_H_INCLUDE
