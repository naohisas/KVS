/*****************************************************************************/
/**
 *  @file   ScaleUnitTag.h
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
#include <string>
#include <iostream>
#include <kvs/Indent>


namespace kvs
{

namespace bdml
{

/*===========================================================================*/
/**
 *  @brief  Scale unit tag class.
 */
/*===========================================================================*/
class ScaleUnitTag : public Tag
{
private:
    float m_x_scale; ///< scale in x dimention
    float m_y_scale; ///< scale in y dimention
    float m_z_scale; ///< scale in z dimention
    std::string m_xyz_unit; ///< measurement unit
    float m_t_scale; ///< time scale
    std::string m_t_unit; ///< measurement unit of time

public:
    ScaleUnitTag();

    float xScale() const { return m_x_scale; }
    float yScale() const { return m_y_scale; }
    float zScale() const { return m_z_scale; }
    const std::string& xyzScale() const { return m_xyz_unit; }
    float tScale() const { return m_t_scale; }
    const std::string& tUnit() const { return m_t_unit; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const Node* parent );
};

} // end of namespace bdml

} // end of namespace kvs
