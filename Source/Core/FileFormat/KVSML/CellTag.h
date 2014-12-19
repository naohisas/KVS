/*****************************************************************************/
/**
 *  @file   CellTag.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellTag.h 1812 2014-09-11 07:34:35Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__KVSML__CELL_TAG_H_INCLUDE
#define KVS__KVSML__CELL_TAG_H_INCLUDE

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

    typedef kvs::kvsml::TagBase BaseClass;

private:

    bool m_has_ncells; ///< flag to check whether 'ncells' is specified or not
    size_t m_ncells; ///< number of cells

public:

    CellTag();

    bool hasNCells() const { return m_has_ncells; }
    size_t ncells() const { return m_ncells; }
    void setNCells( const size_t ncells ) { m_has_ncells = true; m_ncells = ncells; }

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namespace kvsml

} // end of namespace kvs

#endif // KVS__KVSML__CELL_TAG_H_INCLUDE
