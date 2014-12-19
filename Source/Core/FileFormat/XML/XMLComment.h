/****************************************************************************/
/**
 *  @file XMLComment.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLComment.h 1304 2012-09-15 01:23:48Z naohisa.sakamoto@gmail.com $
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
