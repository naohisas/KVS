/*****************************************************************************/
/**
 *  @file   MeasurementTag.h
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
#include "LineTag.h"
#include "PropertyTag.h"
#include <string>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  Measurement tag class.
 */
/*===========================================================================*/
class MeasurementTag : public Tag
{
private:
    std::string m_objectRef; ///< reference to the object
    LineTag m_line; ///< line entity
    PropertyTag m_property; ///< property

public:
    MeasurementTag();

    const std::string& objectRef() const { return m_objectRef; }
    const LineTag& line() const { return m_line; }
    const PropertyTag& property() const { return m_property; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* parent );
};

} // end of namespace bdml

} // end of namespace kvs
