/****************************************************************************/
/**
 *  @file   XMLComment.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__XML_COMMENT_H_INCLUDE
#define KVS__XML_COMMENT_H_INCLUDE

#include "TinyXML.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  XML comment class.
 */
/*==========================================================================*/
class XMLComment : public TiXmlComment
{
public:

    typedef TiXmlComment SuperClass;

public:

    XMLComment();
    XMLComment( const char* comment );
    virtual ~XMLComment();
};

} // end of namespace kvs

#endif // KVS__XML_COMMENT_H_INCLUDE
