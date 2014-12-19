/****************************************************************************/
/**
 *  @file XMLDocument.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: XMLDocument.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "XMLDocument.h"
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
XMLDocument::XMLDocument()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param filename [in] filename
 */
/*==========================================================================*/
XMLDocument::XMLDocument( const std::string& filename )
{
    this->read( filename );
}

/*==========================================================================*/
/**
 *  Get the filename.
 */
/*==========================================================================*/
const std::string& XMLDocument::filename() const
{
    return m_filename;
}

/*==========================================================================*/
/**
 *  Parse the XML document.
 *  @param text [in] text
 *  @return 
 */
/*==========================================================================*/
const char* XMLDocument::parse( const char* text )
{
    return SuperClass::Parse( text );
}

/*==========================================================================*/
/**
 *  Print the XML document.
 */
/*==========================================================================*/
void XMLDocument::print()
{
    SuperClass::Print();
}

/*==========================================================================*/
/**
 *  Insert the node to the XML document.
 *  @param node [in] node
 *  @return pointer to the node
 */
/*==========================================================================*/
TiXmlNode* XMLDocument::insert( const TiXmlNode& node )
{
    return SuperClass::InsertEndChild( node );
}

/*==========================================================================*/
/**
 *  Read file.
 *  @param filename [in] filename
 *  @return true, if the process is done successfully
 */
/*==========================================================================*/
bool XMLDocument::read( const std::string& filename )
{
    m_filename = filename;

    return SuperClass::LoadFile( filename );
}

/*==========================================================================*/
/**
 *  Write file.
 *  @param filename [in] filename
 *  @return true, if the process is done successfully
 */
/*==========================================================================*/
bool XMLDocument::write( const std::string& filename )
{
    return SuperClass::SaveFile( filename );
}

TiXmlNode* XMLDocument::FindNode( const kvs::XMLDocument* document, const std::string& node_name )
{
    TiXmlNode* node = document->FirstChild();
    while( node )
    {
        if( node->Value() == node_name ) return node;

        node = node->NextSibling();
    }

    return NULL;
}

} // end of namespace kvs
