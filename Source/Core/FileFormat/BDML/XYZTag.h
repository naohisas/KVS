/*****************************************************************************/
/**
 *  @file   XYZTag.h
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

#include <iostream>
#include <kvs/Indent>
#include "Tag.h"


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  XYZ tag class.
 */
/*===========================================================================*/
struct XYZTag : public Tag
{
    float x; ///< x coordinate value
    float y; ///< y coordinate value
    float z; ///< z coordinate value

    XYZTag();

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* node );
};

} // end of namespace bdml

} // end of namespace kvs
