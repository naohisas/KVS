/*****************************************************************************/
/**
 *  @file   ValueTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ValueTag.h 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/XMLNode>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <Value>
 */
/*===========================================================================*/
class ValueTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    kvs::kvsml::TagAttribute<std::string> m_label; ///< data label
    kvs::kvsml::TagAttribute<std::string> m_unit; ///< data unit
    kvs::kvsml::TagAttribute<size_t> m_veclen; ///< vector length
    kvs::kvsml::TagAttribute<double> m_min_value; ///< minimum value
    kvs::kvsml::TagAttribute<double> m_max_value; ///< maximum value

public:
    ValueTag();

    bool hasLabel() const { return m_label.hasValue(); }
    bool hasUnit() const { return m_unit.hasValue(); }
    bool hasVeclen() const { return m_veclen.hasValue(); }
    bool hasMinValue() const { return m_min_value.hasValue(); }
    bool hasMaxValue() const { return m_max_value.hasValue(); }
    const std::string& label() const { return m_label; }
    const std::string& unit() const { return m_unit; }
    size_t veclen() const { return m_veclen; }
    double minValue() const { return m_min_value; }
    double maxValue() const { return m_max_value; }

    void setLabel( const std::string& label ) { m_label = label; }
    void setUnit( const std::string& unit ) { m_unit = unit; }
    void setVeclen( const size_t veclen ) { m_veclen = veclen; }
    void setMinValue( const double value ) { m_min_value = value; }
    void setMaxValue( const double value ) { m_max_value = value; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
