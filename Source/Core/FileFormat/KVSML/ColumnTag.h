/*****************************************************************************/
/**
 *  @file   ColumnTag.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColumnTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/XMLNode>
#include <kvs/XMLElement>
#include <Core/FileFormat/KVSML/TagBase.h>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Column tag class.
 */
/*===========================================================================*/
class ColumnTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_label; ///< flag to check whether the label is spcified or not
    bool m_has_min_value; ///< flag to check whether the min_value is spcified or not
    bool m_has_max_value; ///< flag to check whether the max_value is spcified or not
    bool m_has_min_range; ///< flag to check whether the min_range is spcified or not
    bool m_has_max_range; ///< flag to check whether the max_range is spcified or not
    std::string m_label; ///< column label
    double m_min_value; ///< min. value
    double m_max_value; ///< max. value
    double m_min_range; ///< min. range
    double m_max_range; ///< max. range

public:
    ColumnTag();

    bool hasLabel() const { return m_has_label; }
    bool hasMinValue() const { return m_has_min_value; }
    bool hasMaxValue() const { return m_has_max_value; }
    bool hasMinRange() const { return m_has_min_range; }
    bool hasMaxRange() const { return m_has_max_range; }
    const std::string& label() const { return m_label; }
    double minValue() const { return m_min_value; }
    double maxValue() const { return m_max_value; }
    double minRange() const { return m_min_range; }
    double maxRange() const { return m_max_range; }

    void setLabel( const std::string& label ) { m_has_label = true; m_label = label; }
    void setMinValue( const double value ) { m_has_min_value = true; m_min_value = value; }
    void setMaxValue( const double value ) { m_has_max_value = true; m_max_value = value; }
    void setMinRange( const double range ) { m_has_min_range = true; m_min_range = range; }
    void setMaxRange( const double range ) { m_has_max_range = true; m_max_range = range; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool read( const kvs::XMLElement::SuperClass* element );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
