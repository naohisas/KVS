/*****************************************************************************/
/**
 *  @file   TableObjectTag.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TableObjectTag.h 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
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
 *  @brief  Tag class for <TableObject>
 */
/*===========================================================================*/
class TableObjectTag : public kvs::kvsml::TagBase
{
public:
    typedef kvs::kvsml::TagBase BaseClass;

private:
    bool m_has_nrows; ///< flag to check whether 'nrows' is specified or not
    size_t m_nrows; ///< number of rows
    bool m_has_ncolumns; ///< flag to check whether 'ncolumns' is specified or not
    size_t m_ncolumns; ///< number of columns

public:
    TableObjectTag();

    bool hasNRows() const;
    size_t nrows() const;
    bool hasNColumns() const;
    size_t ncolumns() const;

    void setNRows( const size_t nrows );
    void setNColumns( const size_t ncolumns );

    bool read( const kvs::XMLNode::SuperClass* parent );
    bool write( kvs::XMLNode::SuperClass* parent );
};

} // end of namesapce kvsml

} // end of namespace kvs
