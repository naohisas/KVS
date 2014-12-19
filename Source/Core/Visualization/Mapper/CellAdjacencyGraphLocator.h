/*****************************************************************************/
/**
 *  @file   CellAdjacencyGraphLocator.h
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
 * [1] Guo Jiazhen, "An Efficient Hyperstreamline Visualization Method on Large
 *     -Scale Unstructured Datasets," Master Thesis, Kyoto Univeristy, 2012.
 * [2] Takuma Kawamura, Naohisa Sakamoto, Koji Koyamada, "A Streamline Visualization
 *     Technique for Sub-volume Based CFD Results", Asia Simulation Conference 2009,
 *     CD-ROM, 2009.
 */
/*****************************************************************************/
#pragma once
#include <kvs/CellAdjacencyGraph>
#include "CellLocator.h"


namespace kvs
{

class CellAdjacencyGraphLocator : public CellLocator
{
public:

    typedef CellLocator BaseClass;

private:

    kvs::CellAdjacencyGraph* m_adjacency_graph;
    unsigned int m_nrandtests;
    int m_hint_cellid; // used for cache

public:

    CellAdjacencyGraphLocator();
    CellAdjacencyGraphLocator( const kvs::UnstructuredVolumeObject* volume );
    ~CellAdjacencyGraphLocator();

    const kvs::CellAdjacencyGraph* adjacencyGraph() const { return m_adjacency_graph; }

    void build();
    int findCell( const kvs::Vec3 p );
    void clearCache();

private:

    int find_cell( const kvs::Vec3 p, const int start_cellid );
};

} // end of namespace kvs
