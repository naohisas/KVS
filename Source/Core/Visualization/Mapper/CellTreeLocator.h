/*****************************************************************************/
/**
 *  @file   CellTreeLocator.h
 *  @author Naohisa Sakamoto, Guo Jiazhen
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] C. Garth and K.I. Joy, "Fast, memory-efficient cell location in unstructured
 *     grids for visualization, IEEE Transactions on Visualization and Computer
 *     Graphics, Vol.16, No.6, pp.1541–1550, 2010.
 * [2] Guo Jiazhen, "An Efficient Hyperstreamline Visualization Method on Large
 *     -Scale Unstructured Datasets," Master Thesis, Kyoto Univeristy, 2012.
 */
/*****************************************************************************/
#pragma once
#include "CellLocator.h"
#include "CellTree.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cell tree locator class.
 */
/*===========================================================================*/
class CellTreeLocator : public CellLocator
{
public:

    typedef CellLocator BaseClass;

private:

    kvs::CellTree* m_cell_tree;
    bool m_enable_mthreading;
    unsigned int m_cache1[32];
    unsigned int* m_cp1;
    unsigned int m_cache2[16];
    unsigned int* m_cp2;

public:

    CellTreeLocator();
    CellTreeLocator( const kvs::UnstructuredVolumeObject* volume, const bool enable_mthreading = false );
    ~CellTreeLocator();

    const kvs::CellTree* cellTree() const { return m_cell_tree; }
    void setEnabledMultiThreading( const bool enable ) { m_enable_mthreading = enable; }
    void enableMultiThreading() { this->setEnabledMultiThreading( true ); }
    void disableMultiThreading() { this->setEnabledMultiThreading( false ); }

    void build();
    int findCell( const kvs::Vec3 p );
    void clearCache();
};

} // end of namespace kvs
