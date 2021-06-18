/*****************************************************************************/
/**
 *  @file   VertexTag.h
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
 *  @brief  Tag class for <Vertex>
 */
/*===========================================================================*/
class VertexTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_nvertices; ///< flag to check whether 'nvertices' is specified or not
    size_t m_nvertices; ///< number of vertices

public:
    VertexTag();

    bool hasNVertices() const;
    size_t nvertices() const;
    void setNVertices( const size_t nvertices );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
