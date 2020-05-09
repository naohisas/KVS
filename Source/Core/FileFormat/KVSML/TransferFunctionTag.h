/*****************************************************************************/
/**
 *  @file   TransferFunctionTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TransferFunctionTag.h 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/XMLNode>
#include "TagBase.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Tag class for <TransferFunction>
 */
/*===========================================================================*/
class TransferFunctionTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_file; ///< flag to check whether 'file' is specified or not
    std::string m_file; ///< name of external file for 'argb' data
    bool m_has_resolution; ///< flag to check whether 'resolution' is specified or not
    size_t m_resolution; ///< resolution
    bool m_has_min_value; ///< flag to check whether 'min_value' is specified or not
    float m_min_value; ///< min. scalar value
    bool m_has_max_value; ///< flag to check whether 'max_value' is specified or not
    float m_max_value; ///< max. scalar value

public:
    TransferFunctionTag();

    bool hasFile() const;
    const std::string& file() const;
    bool hasResolution() const;
    size_t resolution() const;
    bool hasMinValue() const;
    float minValue() const;
    bool hasMaxValue() const;
    float maxValue() const;

    void setFile( const std::string& file );
    void setResolution( const size_t resolution );
    void setMinValue( const float min_value );
    void setMaxValue( const float max_value );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
