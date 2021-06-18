/*****************************************************************************/
/**
 *  @file   NodeTag.h
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
 *  @brief  Tag class for <Node>
 */
/*===========================================================================*/
class NodeTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    kvs::kvsml::TagAttribute<size_t> m_nnodes; ///< number of nodes

public:
    NodeTag();

    bool hasNNodes() const { return m_nnodes.hasValue(); }
    size_t nnodes() const { return m_nnodes; }
    void setNNodes( const size_t nnodes ) { m_nnodes = nnodes; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
