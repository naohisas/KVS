/****************************************************************************/
/**
 *  @file   XMLDeclaration.h
 *  @author Naohisa Sakamoto
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
