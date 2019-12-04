/*****************************************************************************/
/**
 *  @file   LineObjectTag.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineObjectTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/XMLNode>
#include <kvs/Vector3>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <LineObject>
 */
/*===========================================================================*/
class LineObjectTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_line_type; ///< flag to check whether 'line_type' is specified or not
    bool m_has_color_type; ///< flag to check whether 'color_type' is specified or not
    std::string m_line_type; ///< line type
    std::string m_color_type; ///< color type

public:
    LineObjectTag();

    bool hasLineType() const { return m_has_line_type; }
    bool hasColorType() const { return m_has_color_type; }
    const std::string& lineType() const { return m_line_type; }
    const std::string& colorType() const { return m_color_type; }

    void setLineType( const std::string& type ) { m_has_line_type = true; m_line_type = type; }
    void setColorType( const std::string& type ) { m_has_color_type = true; m_color_type = type; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
