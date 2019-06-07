/*****************************************************************************/
/**
 *  @file   CellTree.cpp
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
#include "CellTree.h"
#include <kvs/Thread>
#include <kvs/BitArray>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Bucket class.
 */
/*===========================================================================*/
struct Bucket
{
    float min;
    float max;
    unsigned int cnt;

    Bucket()
    {
        // only stores current max/min and time of add()
        cnt = 0;
        min = std::numeric_limits<float>::max();
        max = -min;
    }

    void add( const float _min, const float _max )
    {
        ++cnt;
        if ( _min < min ) { min = _min; }
        if ( _max > max ) { max = _max; }
    }
};

/*===========================================================================*/
/**
 *  @brief  PerCell class.
 */
/*===========================================================================*/
struct PerCell
{
    float min[3];
    float max[3];
    unsigned int ind;
};

/*===========================================================================*/
/**
 *  @brief  CenterOrder class.
 */
/*===========================================================================*/
struct CenterOrder
{
    unsigned int d;
    CenterOrder( unsigned int _d ): d(_d) {}
    bool operator()( const PerCell& pc0, const PerCell& pc1 ) { return ( pc0.min[d] + pc0.max[d] ) < ( pc1.min[d] + pc1.max[d] ); }
};

/*===========================================================================*/
/**
 *  @brief  LeftPredicate class.
 */
/*===========================================================================*/
struct LeftPredicate
{
    unsigned int d;
    float p;
    LeftPredicate() { d = 0; p = 0; }
    LeftPredicate( unsigned int _d, float _p ): d( _d ), p( 2.0f * _p ) {}
    bool operator()( const PerCell& pc ) { return ( pc.min[d] + pc.max[d] ) < p; }
};

/*===========================================================================*/
/**
 *  @brief  Finds the min. and max. values for each dimension
 *  @param  begin [in] pointer to the begin cell
 *  @param  end [in] pointer to the end cell
 *  @param  min [out] min. values of the bounds
 *  @param  max [out] max. values of the bounds
 */
/*===========================================================================*/
void FindMinMax( const PerCell* begin, const PerCell* end, float* min, float* max )
{
    // of all those cells which pointed by begin and end
    // find the overall min max of each dimension, ie, 
    // to find the bounding box defined by those cells
    // and record min & max

    if ( begin == end ) { return; }

    for ( unsigned int d = 0; d < 3; ++d )
    {
        min[d] = begin->min[d];
        max[d] = begin->max[d];
    }

    while ( ++begin != end )
    {
        for ( unsigned int d = 0; d < 3; ++d )
        {
            if ( begin->min[d] < min[d] ) { min[d] = begin->min[d]; }
            if ( begin->max[d] > max[d] ) { max[d] = begin->max[d]; }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Splitter class.
 */
/*===========================================================================*/
class Splitter : public kvs::Thread
{
private:

    unsigned int m_leafsize;
    std::vector<kvs::CellTree::Node>* m_nodes;
    kvs::UInt32 m_index;
    kvs::Real32 m_min[3];
    kvs::Real32 m_max[3];
    PerCell* m_pc;

public:

    Splitter()
    {
        m_index = 0;
        m_min[0] = m_min[1] = m_min[2] = 0.0f;
        m_max[0] = m_max[1] = m_max[2] = 0.0f;
        m_nodes = NULL;
        m_pc = NULL;
    }

    ~Splitter() {}

public:

    void init(
        unsigned int leafsize,
        std::vector<kvs::CellTree::Node>* p_nodes,
        PerCell* pc,
        kvs::UInt32 index,
        kvs::Real32 min[3],
        kvs::Real32 max[3] )
    {
        m_leafsize = leafsize;
        m_index = index;
        for ( unsigned int i = 0; i < 3; i ++ )
        {
            m_min[i] = min[i];
            m_max[i] = max[i];
        }
        m_nodes = p_nodes;
        m_pc = pc;
    }

    //bool check() { return ( m_nodes != NULL ); }
    void run() { this->split( m_index, m_min, m_max ); }

    void split( kvs::UInt32 index, kvs::Real32 min[3], kvs::Real32 max[3] )
    {
        std::vector<kvs::CellTree::Node>& nodes = *m_nodes;
        kvs::UInt32 start = nodes[index].leaf.start;
        kvs::UInt32 size  = nodes[index].leaf.size;

        // if size is less than the maxium bucket size, don't do spliting any more
        if ( size < m_leafsize ) { return; }

        PerCell* begin = m_pc + start;
        PerCell* end = m_pc + start + size;
        PerCell* mid = begin;

        // bounding box calculated by the max, min passed to the function
        const unsigned int nbuckets = 6;
        const float ext[3] = { max[0]-min[0], max[1]-min[1], max[2]-min[2] };
        const float iext[3] = { nbuckets/ext[0], nbuckets/ext[1], nbuckets/ext[2] };

        Bucket b[3][nbuckets]; //3 dimensions, 6 buckets each

        // This part loops though all the cells in range [ begin, end )
        // in x, y, z dimension for only once
        // then counts the number of cells in each buckets, and also,
        // the max and min values of them
        for ( const PerCell* pc = begin; pc!=end; ++pc )
        {
            for ( unsigned int d = 0; d < 3; ++d )
            {
                float cen = (pc->min[d] + pc->max[d])/2.0f; // center of a certain dimension

                // (distance from center to min) / (distance from max to min) * nbuckets, of a certain dimension
                int ind = (int)( (cen-min[d])*iext[d] );
                if ( ind < 0 ) { ind = 0; } // how can ind < 0 ??? 
                if ( ind >= int( nbuckets ) ) { ind = nbuckets-1; } // how can ind > nbuckets??

                b[d][ind].add( pc->min[d], pc->max[d] );
            }
        }

        float cost = std::numeric_limits<float>::max();
        float plane = 0.0f;
        unsigned int dim = 0;

        // This part loops through every buckets(6) in all the dimension(3)
        // to determine the best spliting plane
        // bucket1   bucket2   bucket3   bucket4   bucket5   bucket6
        //         |        |         |         |         |
        // each plane is evaluated for cost which is determined by minimizing 
        // left volume * left nodes + right volume * right nnodes

        // for the fist split, a proper dimension is choosed for the least cost
        for ( size_t d = 0; d < 3; ++d )
        {
            unsigned int sum = 0;
            for ( size_t n = 0; n < nbuckets -1; ++n )
            {
                float lmax = -std::numeric_limits<float>::max();
                float rmin =  std::numeric_limits<float>::max();
                for ( size_t m = 0; m <= n; ++m ) { if ( b[d][m].max > lmax ) { lmax = b[d][m].max; } }
                for ( size_t m = n + 1; m < nbuckets; ++m ) { if ( b[d][m].min < rmin ) { rmin = b[d][m].min; } }

                sum += b[d][n].cnt;

                float lvol = (lmax-min[d])/ext[d]; // left volume
                float rvol = (max[d]-rmin)/ext[d]; // right volume
                float c = lvol*sum + rvol*(size-sum);
                if ( sum > 0 && sum < size && c < cost )
                {
                    cost = c;
                    dim = d;
                    plane = min[d] + (n+1)/iext[d];
                }
            }
        }

        // by using STL algorithm partition,
        // all the cell( containing bounding box, cell id)  is divided to left and right,
        // according to the seperating plane and dimension

        if ( cost != std::numeric_limits<float>::max() ) // how can that be ...
        {
            mid = std::partition( begin, end, LeftPredicate( dim, plane ) );
        }

        // fallback
        // in the extreme case, use split-median to determin the mid
        if ( mid == begin || mid == end )
        {
            // the lagest element between ext ext+1 and ext+2
            // note: ext is float type pointer -> 4bytes
            //       dim is uint16 type        -> 4bytes
            dim = std::max_element( ext, ext+3 ) - ext;
            mid = begin + (end-begin)/2;
            std::nth_element( begin, mid, end, CenterOrder( dim ) );
        }

        float lmin[3], lmax[3], rmin[3], rmax[3];

        // find each part's left right bounds
        FindMinMax( begin, mid, lmin, lmax );
        FindMinMax( mid,   end, rmin, rmax );

        // and choose the one along the splitting dimension
        float clip[2] = { lmax[dim], rmin[dim] };

        // making two child leaf nodes, which is consecutive in memory address
        kvs::CellTree::Node child[2];
        child[0].makeLeaf( begin - m_pc, mid-begin );
        child[1].makeLeaf( mid   - m_pc, end-mid );

        // then, make the originally leaf node nodes[index] an ordinary node
        // index is node.size() + 00 | dim
        // insert the two child nodes at the end of current container
        nodes[index].makeNode( nodes.size(), dim, clip );
        nodes.insert( nodes.end(), child, child+2 );

        // traverse to the left brunch
        this->split( nodes[index].left(), lmin, lmax );
        // traverse to the right brunch
        this->split( nodes[index].right(), rmin, rmax );
    }
};

/*===========================================================================*/
/**
 *  @brief  Bounding box class.
 */
/*===========================================================================*/
class BoundingBox
{
private:

    float m_bounds[6];

public:

    BoundingBox() {}

    BoundingBox( const kvs::UnstructuredVolumeObject* object, size_t cindex )
    {
        // create a bounding box based on a specific object and cell index
        float max = std::numeric_limits<float>::max();
        float min = - std::numeric_limits<float>::max();

        float xmax(min), xmin(max), ymax(min), ymin(max), zmax(min), zmin(max);

        size_t length = object->numberOfCellNodes();
        const kvs::UnstructuredVolumeObject::Connections& connections = object->connections();
        const kvs::UnstructuredVolumeObject::Coords& coords = object->coords();

        for ( size_t i = 0; i < length; i ++ )
        {
            // connections is a array of a period of Celltype
            // for Tet1 for example:
            // connections:                            0 1 2 3 4 5 6 7 3 8 10 2 ...
            //                                        [0 1 2 3] [4 5 6 7] [2 8 10 3] ...
            // the connection for the 3rd(cellindex==2) cell is:        [2 8 10 3] ...
            // the first point is the 3th point in coords arry, which lietrally should be:
            // pos = cell index * Celltype (kvs cellindex starts at 0 )
            // coords array is like:                1.0 1.0 1.0 2.1 3.2 4.2 4.5 4.6 4.7 ...
            //                                      [1.0 1.0 1.0] [2.1 3.2 4.2] [4.5 4.6 4.7]
            // the 3rds point starts at the 6sixth position(4.5), which literally should be:
            // coords[ 3 * pos ]
            // coords[ 3 * pos + 1 ]
            // coords[ 3 * pos + 2 ]

            size_t pos = connections[ length*cindex+i ];

            // for x
            if ( coords[ 3*pos ] < xmin ) { xmin = coords[ 3*pos ]; }
            if ( coords[ 3*pos ] > xmax ) { xmax = coords[ 3*pos ]; }

            // for y
            if ( coords[ 3*pos+1 ] < ymin ) { ymin = coords[ 3*pos+1 ]; }
            if ( coords[ 3*pos+1 ] > ymax ) { ymax = coords[ 3*pos+1 ]; }

            // for zs
            if ( coords[ 3*pos+2 ] < zmin ) { zmin = coords[ 3*pos+2 ]; }
            if ( coords[ 3*pos+2 ] > zmax ) { zmax = coords[ 3*pos+2 ]; }
        }

        m_bounds[0] = xmin;
        m_bounds[1] = xmax;
        m_bounds[2] = ymin;
        m_bounds[3] = ymax;
        m_bounds[4] = zmin;
        m_bounds[5] = zmax;
    }

    ~BoundingBox() {}

    const float* bounds() const { return m_bounds; }
};

/*===========================================================================*/
/**
 *  @brief  Builder class.
 */
/*===========================================================================*/
class Builder
{
private:

    bool m_parallel;
    unsigned int m_leafsize;
    std::vector<kvs::CellTree::Node> m_nodes;
    std::vector<kvs::CellTree::Node> m_nodes1;
    std::vector<kvs::CellTree::Node> m_nodes2;
    Splitter m_thread[2];
    PerCell* m_pc;
    PerCell* m_pc1;
    PerCell* m_pc2;

public:

    Builder( bool enable_mthreading = false )
    {
        m_parallel = enable_mthreading;
        m_leafsize = 8;
    }

    ~Builder()
    {
        m_nodes.clear();
        m_nodes1.clear();
        m_nodes2.clear();
        delete [] m_pc;
    }

    //void setParallel() { m_parallel = true; }

    void build( kvs::CellTree& ct, const kvs::UnstructuredVolumeObject* volume )
    {
        const size_t ncells = volume->numberOfCells();
        m_pc = new PerCell[ ncells ];

        // max[3], min[3] are the bounding box of the whole data
        float min[3] = {
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()
        };

        float max[3] = {
            -std::numeric_limits<float>::max(),
            -std::numeric_limits<float>::max(),
            -std::numeric_limits<float>::max(),
        };

        // m_pc coressponds to each cell
        for ( size_t i = 0; i < ncells; ++i )
        {
            m_pc[i].ind = i;

            //double bounds[6];
            //ds.GetCellBounds( i, bounds );
            //bounds[6] is like: xmin, xmax, ymin, ymax, zmin, zmax
            BoundingBox bd( volume, i );
            const float* bounds = bd.bounds();
            for ( int d = 0; d < 3; ++d )
            {
                m_pc[i].min[d] = bounds[2*d+0];
                m_pc[i].max[d] = bounds[2*d+1];

                if ( m_pc[i].min[d] < min[d] ) { min[d] = m_pc[i].min[d]; }
                if ( m_pc[i].min[d] > max[d] ) { max[d] = m_pc[i].max[d]; }
            }
        }

        if ( !m_parallel )
        {
            kvs::CellTree::Node root;
            root.makeLeaf( 0, ncells ); //set index = 3 start = 0 size = ncells
            m_nodes.push_back( root );
            this->split( 0, min, max ); //do the split for all the nodes

            // rearrange the node's order and copy it to cell tree
            // since the split algorithm do in a pre-order way, a tree such as
            //            0
            //       |----|-----|
            //      1           2
            //   |--|--|     |--|--|
            //   3     4     5     6
            //       |-|-|       |-|-|
            //       7   8       9   10
            // will be stored as an arry like
            // 0 1 2 3 4 7 8 5 6 9 10
            // rearrange the elements such that the ones copied into celltree is like
            // 0 1 2 3 4 5 6 7 8 9 10
            // i.e, in a beautiful up-to-down order
            // and of course, reset their (the node's) indices so they always point to the correct left child
            ct.nodes.resize( m_nodes.size() );
            ct.nodes[0] = m_nodes[0];

            std::vector<kvs::CellTree::Node>::iterator ni = ct.nodes.begin();
            std::vector<kvs::CellTree::Node>::iterator nn = ct.nodes.begin() + 1;

            for( ; ni!=ct.nodes.end(); ++ni )
            {
                if ( ni->isLeaf() ) { continue; }

                *(nn++) = m_nodes[ ni->left() ];
                *(nn++) = m_nodes[ ni->right() ];

                ni->setChildren( nn-ct.nodes.begin()-2 );
            }

            ct.leaves.resize( ncells );

            for ( size_t i = 0; i < ncells; ++i ) { ct.leaves[i] = m_pc[i].ind; }
        }
        else
        {
            kvs::CellTree::Node root;
            root.makeLeaf( 0, ncells ); //set index = 3 start = 0 size = ncells

            // seperate m_pc into 2 branches
            unsigned int start = 0;
            unsigned int size = ncells;

            PerCell* begin = m_pc + start;
            PerCell* end = m_pc + start + size;
            PerCell* mid = begin;

            // bounding box calculated by the max, min passed to the function
            const unsigned int nbuckets = 6;
            const float ext[3] = { max[0]-min[0], max[1]-min[1], max[2]-min[2] }; 
            const float iext[3] = { nbuckets/ext[0], nbuckets/ext[1], nbuckets/ext[2] };

            Bucket b[3][nbuckets];     //3 dimensions, 6 buckets each

            // This part loops though all the cells in range [ begin, end )
            // in x, y, z dimension for only once
            // then counts the number of cells in each buckets, and also,
            // the max and min values of them
            for ( const PerCell* pc=begin; pc!=end; ++pc )
            {
                for ( size_t d = 0; d < 3; ++d )
                {
                    float cen = (pc->min[d] + pc->max[d])/2.0f; //center of a certain dimension
                    // (distance from center to min) / (distance from max to min) * nbuckets, of a certain dimension
                    int ind = (int)( (cen-min[d])*iext[d] );
                    if ( ind < 0 ) { ind = 0; } // how can ind < 0 ??? 
                    if ( ind >= int( nbuckets ) ) { ind = nbuckets - 1; } // how can ind > nbuckets??
                    b[d][ind].add( pc->min[d], pc->max[d] );
                }
            }

            float cost = std::numeric_limits<float>::max();
            float plane = 0.0f;
            unsigned int dim = 0;

            // This part loops through every buckets(6) in all the dimension(3)
            // to determine the best spliting plane
            // bucket1   bucket2   bucket3   bucket4   bucket5   bucket6
            //         |        |         |         |         |
            // each plane is evaluated for cost which is determined by minimizing 
            // left volume * left nnodes + right volume * right nnodes

            // for the fist split, a proper dimension is chosen for the least cost
            for( size_t d = 0; d < 3; ++d )
            {
                unsigned int sum = 0;
                for ( size_t n = 0; n < nbuckets-1; ++n )
                {
                    float lmax = -std::numeric_limits<float>::max();
                    float rmin =  std::numeric_limits<float>::max();

                    for ( size_t m = 0; m <= n; ++m ) { if ( b[d][m].max > lmax ) { lmax = b[d][m].max; } }
                    for ( size_t m = n + 1; m < nbuckets; ++m ) { if ( b[d][m].min < rmin ) { rmin = b[d][m].min; } }
                    sum += b[d][n].cnt;
                    float lvol = (lmax-min[d])/ext[d]; // left volume
                    float rvol = (max[d]-rmin)/ext[d]; // right volume
                    float c = lvol*sum + rvol*(size-sum);
                    if ( sum > 0 && sum < size && c < cost )
                    {
                        cost = c;
                        dim = d;
                        plane = min[d] + (n+1)/iext[d];
                    }
                }
            }

            // by using STL algorithm partition,
            // all the cell( containing bounding box, cell id)  is divided to left and right,
            // according to the seperating plane and dimension

            if ( cost != std::numeric_limits<float>::max() )
            {
                // how can that be ...
                mid = std::partition( begin, end, LeftPredicate( dim, plane ) );
            }

            // fallback
            // in the extreme case, use split-median to determin the mid
            if ( mid == begin || mid == end )
            {
                // the lagest element between ext ext+1 and ext+2
                // note: ext is float type pointer -> 4bytes
                //       dim is uint16 type        -> 4bytes
                dim = std::max_element( ext, ext+3 ) - ext;

                mid = begin + (end-begin)/2;
                std::nth_element( begin, mid, end, CenterOrder( dim ) );
            }

            float lmin[3] = {0.0f, 0.0f, 0.0f};
            float lmax[3] = {0.0f, 0.0f, 0.0f};
            float rmin[3] = {0.0f, 0.0f, 0.0f};
            float rmax[3] = {0.0f, 0.0f, 0.0f};

            // find each part's left right bounds
            FindMinMax( begin, mid, lmin, lmax );
            FindMinMax( mid,   end, rmin, rmax );

            // and choose the one along the splitting dimension
            float clip[2] = { lmax[dim], rmin[dim] };

            // assign each to either thread
            unsigned int size1 = mid - m_pc;
            unsigned int start1 = 0;
            unsigned int size2 = end - mid;
            unsigned int start2 = 0;

            kvs::CellTree::Node root1;
            root1.makeLeaf( start1, size1 ); //set index = 3 start = 0 size = ncells
            m_nodes1.push_back( root1 );

            kvs::CellTree::Node root2;
            root2.makeLeaf( start2, size2 ); //set index = 3 start = 0 size = ncells
            m_nodes2.push_back( root2 );

            m_pc1 = m_pc;
            m_pc2 = mid;

            m_thread[0].init( m_leafsize, &m_nodes1, m_pc1, 0, lmin, lmax );
            m_thread[1].init( m_leafsize, &m_nodes2, m_pc2, 0, rmin, rmax );

            m_thread[0].start();
            m_thread[1].start();

            m_thread[0].wait();
            m_thread[1].wait();

            // merge data into celltree
            // size = size_of_tree1 + size_of_tree2 + root
            const unsigned int node_size = m_nodes1.size() + m_nodes2.size() + 1;
            ct.nodes.resize( node_size );

            ct.nodes[0] = root;
            ct.nodes[0].makeNode( 1, dim, clip );

            ct.nodes[1] = m_nodes1[0];
            ct.nodes[2] = m_nodes2[0];

            kvs::BitArray mask;
            mask.allocate( node_size );
            mask.reset(1);
            mask.set(2);

            // ni points to root's left child initially
            std::vector<kvs::CellTree::Node>::iterator ni = ct.nodes.begin()+1;
            // nn points to toot's left child's left child initially
            std::vector<kvs::CellTree::Node>::iterator nn = ct.nodes.begin()+3;

            for ( size_t i = 1; ni != ct.nodes.end(); ++ni, i++ )
            {
                if ( ni->isLeaf() ) { continue; }
                if( !mask[i] )
                {
                    mask.reset( nn-ct.nodes.begin() );
                    *(nn++) = m_nodes1[ ni->left() ];

                    mask.reset( nn-ct.nodes.begin() );
                    *(nn++) = m_nodes1[ ni->right() ];
                }
                else
                {
                    mask.set( nn-ct.nodes.begin() );
                    unsigned int left = ni->left();
                    if ( m_nodes2[left].isLeaf() ) { m_nodes2[left].leaf.start += size1; }
                    *(nn++) = m_nodes2[left];

                    mask.set( nn-ct.nodes.begin() );
                    if ( m_nodes2[left+1].isLeaf() ) { m_nodes2[left+1].leaf.start += size1; }
                    *(nn++) = m_nodes2[left+1];
                }
                ni->setChildren( nn-ct.nodes.begin()-2 );
            }

            ct.leaves.resize( ncells );

            for ( size_t i = 0; i < ncells; ++i ) { ct.leaves[i] = m_pc[i].ind; }
        }
    }

    void split( unsigned int index, float min[3], float max[3] )
    {
        unsigned int start = m_nodes[index].leaf.start;
        unsigned int size = m_nodes[index].leaf.size;

        if ( size < m_leafsize ) { return; } // if size is less than the maxium bucket size, don't do spliting any more

        PerCell* begin = m_pc + start;
        PerCell* end = m_pc + start + size;
        PerCell* mid = begin;

        // bounding box calculated by the max, min passed to the function
        const unsigned int nbuckets  = 6;
        const float ext[3] = { max[0]-min[0], max[1]-min[1], max[2]-min[2] }; 
        const float iext[3] = { nbuckets/ext[0], nbuckets/ext[1], nbuckets/ext[2] };

        Bucket b[3][nbuckets]; //3 dimensions, 6 buckets each

        // This part loops though all the cells in range [ begin, end )
        // in x, y, z dimension for only once
        // then counts the number of cells in each buckets, and also,
        // the max and min values of them
        for ( const PerCell* pc = begin; pc != end; ++pc )
        {
            for ( size_t d = 0; d < 3; ++d )
            {
                float cen = (pc->min[d] + pc->max[d])/2.0f; //center of a certain dimension
                // (distance from center to min) / (distance from max to min) * nbuckets, of a certain dimension
                int ind = (int)( (cen-min[d])*iext[d] );

                if ( ind < 0 ) { ind = 0; } // how can ind < 0 ???
                if ( ind >= int( nbuckets ) ) { ind = nbuckets-1; } // how can ind > nbuckets??
                b[d][ind].add( pc->min[d], pc->max[d] );
            }
        }

        float cost = std::numeric_limits<float>::max();
        float plane = 0.0f;
        unsigned int dim = 0;

        // This part loops through every buckets(6) in all the dimension(3)
        // to determine the best spliting plane
        // bucket1   bucket2   bucket3   bucket4   bucket5   bucket6
        //         |        |         |         |         |
        // each plane is evaluated for cost which is determined by minimizing 
        // left volume * left nnodes + right volume * right nnodes

        // for the fist split, a proper dimension is chosen for the least cost
        for ( size_t d = 0; d < 3; ++d )
        {
            unsigned int sum = 0;
            for ( size_t n=0; n<nbuckets-1; ++n )
            {
                float lmax = -std::numeric_limits<float>::max();
                float rmin =  std::numeric_limits<float>::max();

                for ( size_t m = 0; m <= n; ++m ) { if ( b[d][m].max > lmax ) { lmax = b[d][m].max; } }
                for ( size_t m = n+1; m < nbuckets; ++m ) { if ( b[d][m].min < rmin ) { rmin = b[d][m].min; } }

                sum += b[d][n].cnt;

                float lvol = (lmax-min[d])/ext[d]; // left volume
                float rvol = (max[d]-rmin)/ext[d]; // right volume
                float c = lvol*sum + rvol*(size-sum);
                if ( sum > 0 && sum < size && c < cost )
                {
                    cost = c;
                    dim = d;
                    plane = min[d] + (n+1)/iext[d];
                }
            }
        }

        // by using STL algorithm partition,
        // all the cell( containing bounding box, cell id)  is divided to left and right,
        // according to the seperating plane and dimension

        if ( cost != std::numeric_limits<float>::max() )
        {
            // how can that be ...
            mid = std::partition( begin, end, LeftPredicate( dim, plane ) );
        }

        // fallback
        // in the extreme case, use split-median to determin the mid
        if ( mid == begin || mid == end )
        {
            // the lagest element between ext ext+1 and ext+2
            // note: ext is float type pointer -> 4bytes
            //       dim is uint16 type        -> 4bytes
            dim = std::max_element( ext, ext+3 ) - ext;

            mid = begin + (end-begin)/2;
            std::nth_element( begin, mid, end, CenterOrder( dim ) );
        }

        float lmin[3], lmax[3], rmin[3], rmax[3];

        // find each part's left right bounds
        FindMinMax( begin, mid, lmin, lmax );
        FindMinMax( mid,   end, rmin, rmax );

        // and choose the one along the splitting dimension
        float clip[2] = { lmax[dim], rmin[dim] };

        // making two child leaf nodes, which is consecutive in memory address
        kvs::CellTree::Node child[2];
        child[0].makeLeaf( begin - m_pc, mid-begin );
        child[1].makeLeaf( mid   - m_pc, end-mid );

        // then, make the originally leaf node m_nodes[index] an ordinary node
        // index is node.size() + 00 | dim
        // insert the two child nodes at the end of current container 
        m_nodes[index].makeNode( m_nodes.size(), dim, clip );
        m_nodes.insert( m_nodes.end(), child, child+2 );

        // traverse to the left brunch
        this->split( m_nodes[index].left(), lmin, lmax );
        // traverse to the right brunch
        this->split( m_nodes[index].right(), rmin, rmax );
    }
};

} // end of namespace


namespace kvs
{

CellTree::CellTree()
{
}

CellTree::CellTree( const kvs::UnstructuredVolumeObject* volume, bool enable_mthreading )
{
    this->build( volume, enable_mthreading );
}

CellTree::~CellTree()
{
    nodes.clear();
    leaves.clear();
}

kvs::UInt32 CellTree::height( CellTree::Node& node )
{
    if ( node.isLeaf() ) { return 0; }
    return 1 + std::max( height( nodes[ node.left() ] ), height( nodes[ node.right() ] ) );
}

void CellTree::build( const kvs::UnstructuredVolumeObject* volume, bool enable_mthreading )
{
    Builder builder( enable_mthreading );
    builder.build( *this, volume );
}

} // end of namespace kvs
