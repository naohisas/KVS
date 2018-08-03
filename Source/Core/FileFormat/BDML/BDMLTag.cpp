/*****************************************************************************/
/**
 *  @file   BDMLTag.cpp
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
#include "BDMLTag.h"
#include "XML.h"


namespace kvs
{

namespace bdml
{

BDMLTag::BDMLTag():
    Tag("bdml")
{
    m_version = 0.0f;
}

void BDMLTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Version: " << m_version << std::endl;
}

bool BDMLTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    const Element* element = ToElement( Tag::node() );
    if ( element )
    {
        m_version = AttributeValue<float>( element, "version" );
    }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
