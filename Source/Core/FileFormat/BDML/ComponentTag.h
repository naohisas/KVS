/*****************************************************************************/
/**
 *  @file   ComponentTag.h
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
#include "MeasurementTag.h"
#include <string>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  Component tag class. Collection of the objects.
 */
/*===========================================================================*/
class ComponentTag : public Tag
{
private:
    std::string m_componentID; ///< unique ID of a component
    std::string m_componentName; ///< name of the component
    float m_time; ///< time
    std::string m_prevID; ///< connectivity with another component
    MeasurementTag m_measurement; ///< spatial information

public:
    ComponentTag();

    const std::string& componentID() const { return  m_componentID; }
    const std::string& componentName() const { return m_componentName; }
    float time() const { return m_time; }
    const std::string& prevID() const { return m_prevID; }
    const MeasurementTag& measurement() const { return  m_measurement; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* node );
};

} // end of namespace bdml

} // end of namespace kvs
