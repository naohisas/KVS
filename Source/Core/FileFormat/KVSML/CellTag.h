/*****************************************************************************/
/**
 *  @file   CellTag.h
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
 *  @brief  Tag class for <Cell>
 */
/*===========================================================================*/
class CellTag : public kvs::kvsml::TagBase
{
public:
    using BaseClass = kvs::kvsml::TagBase;

private:
    kvs::kvsml::TagAttribute<size_t> m_ncells; ///< number of cells

public:
    CellTag();

    bool hasNCells() const { return m_ncells.hasValue(); }
    size_t ncells() const { return m_ncells; }
    void setNCells( const size_t ncells ) { m_ncells = ncells; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs
