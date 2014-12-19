/*****************************************************************************/
/**
 *  @file   CellTreeLocator.cpp
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
#include "CellTreeLocator.h"


namespace
{

bool CellContains( const kvs::CellBase* cell, const kvs::Vec3& p )
{
    kvs::Vec3 min_cell_coord = cell->vertices()[0];
    kvs::Vec3 max_cell_coord = cell->vertices()[0];
    const size_t nnodes = cell->numberOfCellNodes();
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const kvs::Vec3 v = cell->vertices()[i];
        min_cell_coord.x() = kvs::Math::Min( min_cell_coord.x(), v.x() );
        min_cell_coord.y() = kvs::Math::Min( min_cell_coord.y(), v.y() );
        min_cell_coord.z() = kvs::Math::Min( min_cell_coord.z(), v.z() );
        max_cell_coord.x() = kvs::Math::Max( max_cell_coord.x(), v.x() );
        max_cell_coord.y() = kvs::Math::Max( max_cell_coord.y(), v.y() );
        max_cell_coord.z() = kvs::Math::Max( max_cell_coord.z(), v.z() );
    }

    if ( p.x() < min_cell_coord.x() || p.x() > max_cell_coord.x() ) { return false; }
    if ( p.y() < min_cell_coord.y() || p.y() > max_cell_coord.y() ) { return false; }
    if ( p.z() < min_cell_coord.z() || p.z() > max_cell_coord.z() ) { return false; }

    cell->setGlobalPoint( p );
    const kvs::Vec3 local = cell->localPoint();
    switch ( nnodes )
    {
    case  4: // Tetrahedra
    {
        if ( local.x() < 0 || 1 < local.x() ) { return false; }
        if ( local.y() < 0 || 1 < local.y() ) { return false; }
        if ( local.z() < 0 || 1 < local.z() ) { return false; }
        if ( local.x() + local.y() + local.z() > 1 ) { return false; }
        return true;
    }
    case  8: // Hexahedra
    case 10: // Quadratic tetrahedra
    case 20: // Quadratic hexahedra
    {
        if ( local.x() < 0 || 1 < local.x() ) { return false; }
        if ( local.y() < 0 || 1 < local.y() ) { return false; }
        if ( local.z() < 0 || 1 < local.z() ) { return false; }
        return true;
    }
    case  6: // Prism
    {
        if ( local.x() < 0 || 1 < local.x() ) { return false; }
        if ( local.y() < 0 || 1 < local.y() ) { return false; }
        if ( local.z() < 0 || 1 < local.z() ) { return false; }
        if ( local.x() + local.y() > 1 ) { return false; }
        return true;
    }
    default:
    {
        kvsMessageError("Not supported cell type.");
        return false;
    }
    }
}

} // end of namespace


namespace kvs
{

CellTreeLocator::CellTreeLocator()
{
    m_enable_mthreading = false;
    this->clearCache();
}

CellTreeLocator::CellTreeLocator(
    const kvs::UnstructuredVolumeObject* volume,
    const bool enable_mthreading )
{
    BaseClass::attachVolume( volume );
    this->clearCache();
    this->setEnabledMultiThreading( enable_mthreading );
    this->build();
}

CellTreeLocator::~CellTreeLocator()
{
    if ( m_cell_tree ) { delete m_cell_tree; }
}

void CellTreeLocator::build()
{
    KVS_ASSERT( BaseClass::volume() );
    m_cell_tree = new kvs::CellTree( BaseClass::volume(), m_enable_mthreading );
}

int CellTreeLocator::findCell( const kvs::Vec3 p )
{
    switch ( BaseClass::cacheMode() )
    {
    case CacheOff:
    {
        CellTree::PreTraversal pt( *m_cell_tree, p.data() );
        while ( const CellTree::Node* n = pt.next() )
        {
            // pt.next() brings us to a series of leaves that may contain p
            const unsigned int* begin = &(m_cell_tree->leaves[ n->start ]);
            const unsigned int* end = begin + n->size;
            for ( ; begin != end; ++begin )
            {
                BaseClass::cell()->bindCell( *begin );
                if ( ::CellContains( BaseClass::cell(), p ) ) { return *begin; }
            }
        }
        break;
    }
    case CacheHalf:
    {
        CellTree::PreTraversalCached pt( *m_cell_tree, p.data(), m_cache1[0] );
        while ( const CellTree::Node* n = pt.next() )
        {
            // pt.next() brings us to a series of leaves that may contain p
            const unsigned int* begin = &(this->m_cell_tree->leaves[ n->start ]);
            const unsigned int* end = begin + n->size;
            for ( ; begin != end; ++begin )
            {
                BaseClass::cell()->bindCell( *begin );
                if ( ::CellContains( BaseClass::cell(), p ) )
                {
                    const unsigned int* sp = pt.sp();
                    m_cache1[0] = *sp;
                    return *begin;
                }
            }
        }
        break;
    }
    case CacheFull:
    {
        CellTree::PreTraversalCached pt( *m_cell_tree, p.data(), m_cache1[0] );
        while ( const CellTree::Node* n = pt.next() )
        {
            // pt.next() brings us to a series of leaves that may contain p
            const unsigned int* begin = &(this->m_cell_tree->leaves[ n->start ]);
            const unsigned int* end = begin + n->size;
            for ( ; begin != end; ++begin )
            {
                BaseClass::cell()->bindCell( *begin );
                if ( ::CellContains( BaseClass::cell(), p ) )
                {
                    const unsigned int* stack1 = pt.m_stack;
                    const unsigned int* sp1 = pt.m_sp;
                    int n = sp1 - stack1;
                    memcpy( m_cache1 + 1, stack1 + 1, 124 );
                    m_cp1 = m_cache1 + n + 1; // +1 is important!!
                    return *begin;
                }
            }
        }
        break;
    }
    default:
    {
        kvsMessageError("Unknown cache mode.");
        break;
    }
    }

    return -1;
}

void CellTreeLocator::clearCache()
{
    for ( size_t i = 0; i < 32; i++ ) { m_cache1[ i ] = -1; }
    for ( size_t i = 0; i < 16; i++ ) { m_cache2[ i ] = -1; }

    m_cache1[0] = 0;
    m_cache2[0] = 0;

    m_cp1 = m_cache1 + 1;
    m_cp2 = m_cache2;
}

} // end of namespace kvs
