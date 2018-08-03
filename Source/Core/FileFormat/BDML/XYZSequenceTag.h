/*****************************************************************************/
/**
 *  @file   XYZSequenceTag.h
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
#include <iostream>
#include <kvs/Indent>
#include <kvs/ValueArray>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  XYZ sequence tag class.
 */
/*===========================================================================*/
class XYZSequenceTag : public Tag
{
private:
    kvs::ValueArray<float> m_xyz; ///< x,y,z coordinate values

public:
    XYZSequenceTag();

    const kvs::ValueArray<float>& xyz() const { return m_xyz; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* node );
};

} // end of namespace bdml

} // end of namespace kvs
