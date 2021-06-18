/*****************************************************************************/
/**
 *  @file   LineTag.h
 *  @author Naohisa Sakamoto
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
 *  @brief  Tag class for <Line>
 */
/*===========================================================================*/
class LineTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_nlines; ///< flag to check whether 'nlines' is specified or not
    size_t m_nlines; ///< number of lines

public:
    LineTag();

    bool hasNLines() const { return m_has_nlines; }
    size_t nlines() const { return m_nlines; }

    void setNLines( const size_t nlines ) { m_has_nlines = true; m_nlines = nlines; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
