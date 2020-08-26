/****************************************************************************/
/**
 *  @file   XMLDocument.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__XML_DOCUMENT_H_INCLUDE
#define KVS__XML_DOCUMENT_H_INCLUDE

#include "TinyXML.h"
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  XML document class.
 */
/*==========================================================================*/
class XMLDocument : public TiXmlDocument
{
public:

    typedef TiXmlDocument SuperClass;

protected:

    std::string m_filename; ///< filename

public:

    XMLDocument();
    XMLDocument( const std::string& filename );

public:

    const std::string& filename() const;

public:

    const char* parse( const char* text );
    void print();

public:

    TiXmlNode* insert( const TiXmlNode& node );

public:

    bool read( const std::string& filename );
    bool write( const std::string& filename );

public:

    static TiXmlNode* FindNode( const kvs::XMLDocument* document, const std::string& node_name );
};

} // end of namespace kvs

#endif // KVS__XML_DOCUMENT_H_INCLUDE
