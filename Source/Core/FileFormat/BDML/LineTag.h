/*****************************************************************************/
/**
 *  @file   LineTag.h
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
#pragma once

#include "Tag.h"
#include "XYZSequenceTag.h"
#include <vector>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  Line tag class.
 */
/*===========================================================================*/
class LineTag : public Tag
{
private:
    std::vector<XYZSequenceTag> m_xyzSequences; ///< continuous sequence points

public:
    LineTag();

    const std::vector<XYZSequenceTag>& xyzSequences() const { return m_xyzSequences; }
    const XYZSequenceTag& xyzSequence( const size_t index ) const { return m_xyzSequences[index]; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* parent );
};

} // end of namespace bdml

} // end of namespace kvs
