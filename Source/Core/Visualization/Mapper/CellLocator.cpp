/*****************************************************************************/
/**
 *  @file   CellLocator.cpp
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
/*****************************************************************************/
#include "CellLocator.h"
#include <kvs/TetrahedralCell>
#include <kvs/HexahedralCell>
#include <kvs/QuadraticTetrahedralCell>
#include <kvs/QuadraticHexahedralCell>
#include <kvs/PyramidalCell>
#include <kvs/PrismaticCell>


namespace kvs
{

CellLocator::CellLocator():
    m_volume( NULL ),
    m_cell( NULL ),
    m_cache_mode( CellLocator::CacheOff )
{
}

CellLocator::~CellLocator()
{
    if ( m_cell ) { delete m_cell; }
}

void CellLocator::attachVolume( const kvs::UnstructuredVolumeObject* volume )
{
    m_volume = volume;

    if ( m_cell ) { delete m_cell; }

    switch ( m_volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        m_cell = new kvs::TetrahedralCell( m_volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        m_cell = new kvs::HexahedralCell( m_volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        m_cell = new kvs::QuadraticTetrahedralCell( m_volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        m_cell = new kvs::QuadraticHexahedralCell( m_volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::Pyramid:
    {
        m_cell = new kvs::PyramidalCell( m_volume );
        break;
    }
    case kvs::UnstructuredVolumeObject::Prism:
    {
        m_cell = new kvs::PrismaticCell( m_volume );
        break;
    }
    default:
    {
        kvsMessageError("Not supported cell type.");
        break;
    }
    }
}

} // end of namespace kvs
