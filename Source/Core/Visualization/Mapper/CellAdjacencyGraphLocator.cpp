/*****************************************************************************/
/**
 *  @file   CellAdjacencyGraphLocator.cpp
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
#include "CellAdjacencyGraphLocator.h"
#include <kvs/MersenneTwister>


namespace
{

size_t RandomCellIndex( const kvs::UnstructuredVolumeObject* volume )
{
    kvs::MersenneTwister r( time(0) );
    return r() * ( volume->numberOfCells() - 1 );
}

struct Line
{
    kvs::Vec3 start;
    kvs::Vec3 end;
    float t;

    Line() {}
    Line( const kvs::Vec3 s, const kvs::Vec3 e, const float a ):
        start(s), end(e), t(a) {}
};

struct Plane
{
    kvs::Vec3 p0;
    kvs::Vec3 p1;
    kvs::Vec3 p2;

    Plane() {}
    Plane( const kvs::Vec3 a, const kvs::Vec3 b, const kvs::Vec3 c ):
        p0(a), p1(b), p2(c) {}
};

struct Weights
{
    float t;
    float u;
    float v;
    bool parallel;
};

const Weights LinePlaneIntersection( Line line, const Plane plane )
{
    Weights w;
    const kvs::Vec3& la = line.start;
    const kvs::Vec3& lb = line.end;

    const kvs::Vec3& p0 = plane.p0;
    const kvs::Vec3& p1 = plane.p1;
    const kvs::Vec3& p2 = plane.p2;

    kvs::Mat3 M(
        la.x()-lb.x(),  p1.x()-p0.x(),  p2.x()-p0.x(),
        la.y()-lb.y(),  p1.y()-p0.y(),  p2.y()-p0.y(),
        la.z()-lb.z(),  p1.z()-p0.z(),  p2.z()-p0.z() );

    if ( M.determinant() == 0 ) // if singular
    {
        w.t = 0;
        w.u = 0;
        w.v = 0;
        w.parallel = 1;
        return w;
    }

    kvs::Vec3 V = la - p0;

    kvs::Vec3 result = M.inverted() * V;
    w.t = result[0];
    w.u = result[1];
    w.v = result[2];
    w.parallel = 0;

    return w;
}

const kvs::UInt32 TetCellFaces[12] =
{
    0, 1, 2, // face 0
    0, 2, 3, // face 1
    0, 3, 1, // face 2
    1, 3, 2  // face 3
};

} // end namespace


namespace kvs
{

CellAdjacencyGraphLocator::CellAdjacencyGraphLocator():
    m_adjacency_graph( NULL ),
    m_nrandtests( 30 ),
    m_hint_cellid( -1 )
{
}

CellAdjacencyGraphLocator::CellAdjacencyGraphLocator( const kvs::UnstructuredVolumeObject* volume ):
    m_adjacency_graph( NULL ),
    m_nrandtests( 30 ),
    m_hint_cellid( -1 )
{
    BaseClass::attachVolume( volume );
    this->build();
}

CellAdjacencyGraphLocator::~CellAdjacencyGraphLocator()
{
    if ( m_adjacency_graph ) { delete m_adjacency_graph; }
}

void CellAdjacencyGraphLocator::build()
{
    KVS_ASSERT( BaseClass::volume() );
    m_adjacency_graph = new kvs::CellAdjacencyGraph( BaseClass::volume() );
}

int CellAdjacencyGraphLocator::findCell( const kvs::Vec3 p )
{
    switch ( BaseClass::cacheMode() )
    {
    case CacheOff:
    {
        // 1 bind some random cell indices, get their center,
        // find the one closest to the target point
        unsigned int startindex = 0, temp_startindex = 0;
        float min = std::numeric_limits<float>::max();
        float distance = 0.0f;
        kvs::Vec3 center;
        for ( size_t i = 0; i < m_nrandtests; i++ )
        {
            temp_startindex = ::RandomCellIndex( BaseClass::volume() );
            BaseClass::cell()->bindCell( temp_startindex );
            center = BaseClass::cell()->center();
            distance = ( center - p ).length();
            if ( distance < min )
            {
                min = distance;
                startindex = temp_startindex;
            }
        }

        return this->find_cell( p, startindex );
    }
    case CacheHalf:
    {
        if ( m_hint_cellid == -1 )
        {
            // 1 bind some random cell indices, get their center,
            // find the one closest to the target point
            unsigned int startindex = 0, temp_startindex = 0;
            float min = std::numeric_limits<float>::max();
            float distance = 0.0f;
            kvs::Vec3 center;
            for ( size_t i = 0; i < m_nrandtests; i ++ )
            {
                temp_startindex = ::RandomCellIndex( BaseClass::volume() );
                BaseClass::cell()->bindCell( temp_startindex );
                center = BaseClass::cell()->center();
                distance = ( center - p ).length();
                if ( distance < min )
                {
                    min = distance;
                    startindex = temp_startindex;
                }
            }

            m_hint_cellid = startindex;
        }

        return this->find_cell( p, m_hint_cellid );
    }
    default:
    {
        kvsMessageError("Not supported cache mode.");
        break;
    }
    }

    return -1;
}

int CellAdjacencyGraphLocator::find_cell( const kvs::Vec3 p, const int start_cellid )
{
    switch ( BaseClass::volume()->cellType() )
    {
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        ::Weights w;
        unsigned int current_faceid = 0;
        float step = 0;
        bool found = false;

        // 1 starting from the center, find the intersection of the line and polygon
        // 2 from adjacency graph, find which cell to go next
        // 3 go to the next cell, find the outgoing intersection
        // repeat from 2 to 3 util reach the pos

        BaseClass::cell()->bindCell( start_cellid );
        kvs::Vec3 center = BaseClass::cell()->center();
        kvs::Vec3 end = p;

        ::Line line( center, end, 0 ); //initialize the line
        int current_cellid = start_cellid;
        while (1)
        {
            if ( found ) { return current_cellid; }

            if ( BaseClass::cell()->contains( p ) )
            {
                found = true;
                m_hint_cellid = current_cellid;
                return current_cellid;
            }

            BaseClass::cell()->bindCell( current_cellid );
            for ( size_t i = 0; i < 4; i ++ )
            {
                ::Plane p(
                    BaseClass::cell()->coords()[TetCellFaces[ 3*i+0 ]],
                    BaseClass::cell()->coords()[TetCellFaces[ 3*i+1 ]],
                    BaseClass::cell()->coords()[TetCellFaces[ 3*i+2 ]]);

                w = ::LinePlaneIntersection( line, p );
                if ( w.u >= 0 && w.v >= 0 && w.u + w.v <= 1 && w.t > step )
                {
                    current_faceid = i;
                    step = w.t;
                    break;
                }

                if ( i == 3 )
                {
                    step = 0;
                    line.start = BaseClass::cell()->randomSampling();
                    i = 0;
                }
            }

            if ( m_adjacency_graph->mask()[ 4*current_cellid+current_faceid ] == 1 )
            {
                current_cellid = m_adjacency_graph->graph()[ 4*current_cellid+current_faceid ];
            }
            else // the ray goes out of the volume object
            {
                // record current step
                // do a brute force search along all the external face
                // find out the step that satisfies
                // step < 1 and step = max of all the step found
                // and then set the current index 
                float step_save = step;
                for ( size_t i = 0; i < m_adjacency_graph->mask().size(); i++ )
                {
                    if ( m_adjacency_graph->mask()[i] == 0 )
                    {
                        current_faceid = i % 4;
                        BaseClass::cell()->bindCell( i / 4 ); //current_cellid = i / 4;
                        ::Plane p(
                            BaseClass::cell()->coords()[TetCellFaces[ 3*current_faceid]],
                            BaseClass::cell()->coords()[TetCellFaces[ 3*current_faceid+1 ] ],
                            BaseClass::cell()->coords()[TetCellFaces[ 3*current_faceid+2 ] ]);
                        w = ::LinePlaneIntersection( line, p );

                        if ( w.u >= 0 && w.v >= 0 && w.u + w.v <= 1 && w.t > step && w.t < 1 )
                        {
                            current_cellid = i / 4;
                            step = w.t;
                        }
                    }
                }

                if ( step_save == step ) { return -1; } //means the point is outside of the volume
            }
        }
        break;
    }
    default:
    {
        kvsMessageError("Not supported cell type.");
        break;
    }
    }

    return -1;
}

void CellAdjacencyGraphLocator::clearCache()
{
    m_hint_cellid = -1;
}

} // end of namespace kvs
