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
#ifndef KVS__KVSML__VALUE_TAG_H_INCLUDE
#define KVS__KVSML__VALUE_TAG_H_INCLUDE

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

    bool m_has_label; ///< flag to check whether 'label' is specified or not
    bool m_has_unit; ///< flag to check whether 'unit' is specified or not
    bool m_has_veclen; ///< flag to check whether 'veclen' is specified or not
    bool m_has_min_value; ///< flag to check whether 'min_value' is specified or not
    bool m_has_max_value; ///< flag to check whether 'max_value' is specified or not
    std::string m_label; ///< data label
    std::string m_unit; ///< data unit
    size_t m_veclen; ///< vector length
    double m_min_value; ///< minimum value
    double m_max_value; ///< maximum value

public:

    ValueTag();

    bool hasLabel() const { return m_has_label; }
    bool hasUnit() const { return m_has_unit; }
    bool hasVeclen() const { return m_has_veclen; }
    bool hasMinValue() const { return m_has_min_value; }
    bool hasMaxValue() const { return m_has_max_value; }
    const std::string& label() const { return m_label; }
    const std::string& unit() const { return m_unit; }
    size_t veclen() const { return m_veclen; }
    double minValue() const { return m_min_value; }
    double maxValue() const { return m_max_value; }

    void setLabel( const std::string& label ) { m_has_label = true; m_label = label; }
    void setUnit( const std::string& unit ) { m_has_unit = true; m_unit = unit; }
    void setVeclen( const size_t veclen ) { m_has_veclen = true; m_veclen = veclen; }
    void setMinValue( const double value ) { m_has_min_value = true; m_min_value = value; }
    void setMaxValue( const double value ) { m_has_max_value = true; m_max_value = value; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__VALUE_TAG_H_INCLUDE
