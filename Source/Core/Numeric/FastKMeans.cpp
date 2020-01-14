/*****************************************************************************/
/**
 *  @file   FastKMeans.cpp
 *  @author Naohisa Sakamoto
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
 * [1] Greg Hamerly, Making k-means even faster, In proceedings of the 2010 SIAM
 *     international conference on data mining (SDM 2010), April 2010.
 * [2] D. Arthur and S. Vassilvitskii, k-means++ : The Advantages of Careful
 *     Seeding, in Proceedings of the eighteenth annual ACM-SIAM symposium on
 *     Discrete algorithms, 2007, pp. 1027-1035.
 */
/*****************************************************************************/
#include "FastKMeans.h"
#include <kvs/Value>
#include <kvs/Message>
#include <kvs/Math>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns the distance between the given points.
 *  @param  x0 [in] point 0
 *  @param  x1 [in] point 1
 *  @return distance
 */
/*===========================================================================*/
kvs::Real32 GetEuclideanDistance(
    const kvs::ValueArray<kvs::Real32>& x0,
    const kvs::ValueArray<kvs::Real32>& x1 )
{
    kvs::Real32 distance = 0.0f;
    const size_t nrows = x0.size();
    for ( size_t i = 0; i < nrows; i++ )
    {
        const kvs::Real32 diff = x1[i] - x0[i];
        distance += diff * diff;
    }

    return distance;
}

/*===========================================================================*/
/**
 *  @brief  Initializes cluster centers with random seeding.
 *  @param  table [in] table data
 *  @param  nclusters [in] number of clusters
 *  @param  random [in] random number generator
 *  @param  center [out] cluster centers
 */
/*===========================================================================*/
void InitializeCenterWithRandomSeeding(
    const kvs::AnyValueTable& table,
    const size_t nclusters,
    kvs::MersenneTwister& random,
    kvs::ValueArray<kvs::Real32>* center )
{
    const size_t nrows = table.column(0).size();
    for ( size_t i = 0; i < nclusters; i++ )
    {
        const kvs::UInt32 index = static_cast<kvs::UInt32>( nrows * random.rand() );
        center[i] = table.rowAsValueArray<kvs::Real32>( index );
    }
}

/*===========================================================================*/
/**
 *  @brief  Initializes cluster centers with smart seeding.
 *  @param  table [in] table data
 *  @param  nclusters [in] number of clusters
 *  @param  random [in] random number generator
 *  @param  center [out] cluster centers
 */
/*===========================================================================*/
void InitializeCenterWithSmartSeeding(
    const kvs::AnyValueTable& table,
    const size_t nclusters,
    kvs::MersenneTwister& random,
    kvs::ValueArray<kvs::Real32>* center )
{
    const size_t nrows = table.column(0).size();
    const size_t ncolumns = table.columnSize();
    const kvs::UInt32 index = static_cast<kvs::UInt32>( nrows * random.rand() );
    center[0] = table.rowAsValueArray<kvs::Real32>( index );

    for ( size_t i = 1; i < nclusters; i++ )
    {
        kvs::Real32 S = 0.0;
        kvs::Real32* D = new kvs::Real32 [ nrows ];
        for ( size_t j = 0; j < nrows; j++ )
        {
            kvs::Real32 distance = kvs::Value<kvs::Real32>::Max();
            for ( size_t k = 0; k < nclusters; k++ )
            {
                const kvs::Real32 d = GetEuclideanDistance( table.rowAsValueArray<kvs::Real32>(j), center[k] );
                if ( d < distance ) { distance = d; }
            }
            D[j] = distance;
            S += distance * distance;
        }

        size_t index = 0;
        kvs::Real32 P = 0.0;
        for ( size_t j = 0; j < nrows; j++ )
        {
            const kvs::Real32 DD = D[j] * D[j];
            if ( P < kvs::Math::Max( P, DD / S ) )
            {
                P = DD / S;
                index = j;
            }
        }

        for ( size_t j = 0; j < ncolumns; j++ )
        {
            center[i].at(j) = table.column(j).at<kvs::Real32>( index );
        }

        delete [] D;
    }
}

}

namespace
{

/*===========================================================================*/
/**
 *  @brief  Updates upper and lower bounds and index of the center over all centers.
 *  @param  nclusters [in] number of clusters
 *  @param  xi [in] data point at i-th row in the table data
 *  @param  c [in] set of centers
 *  @param  ai [out] index of the centers for xi
 *  @param  ui [out] upper bound for xi
 *  @param  li [out] lower bound for xi
 */
/*===========================================================================*/
void PointAllCtrs(
    const size_t nclusters,
    const kvs::ValueArray<kvs::Real32>& xi,
    const kvs::ValueArray<kvs::Real32>* c,
    kvs::UInt32& ai,
    kvs::Real32& ui,
    kvs::Real32& li )
{
    // Algorithm 3: POINT-ALL-CTRS( x(i), c, a(i), u(i), l(i) )

    kvs::UInt32 index = 0;
    kvs::Real32 dmin = kvs::Value<kvs::Real32>::Max();
    for ( size_t j = 0; j < nclusters; j++ )
    {
        const kvs::Real32 d = ::GetEuclideanDistance( xi, c[j] );
        if ( d < dmin )
        {
            dmin = d;
            index = static_cast<kvs::UInt32>(j);
        }
    }
    ai = index;

    ui = ::GetEuclideanDistance( xi, c[ai] );

    dmin = kvs::Value<kvs::Real32>::Max();
    for ( size_t j = 0; j < nclusters; j++ )
    {
        if ( j != ai )
        {
            const kvs::Real32 d = ::GetEuclideanDistance( xi, c[j] );
            dmin = kvs::Math::Min( dmin, d );
        }
    }
    li = dmin;
}

/*===========================================================================*/
/**
 *  @brief  Initializes the upper and lower bounds and the assignments.
 *  @param  nclusters [in] number of clusters
 *  @param  table [in] table data
 *  @param  c [in] set of cluster centers
 *  @param  q [out] number of points
 *  @param  cp [out] vector sum of all points
 *  @param  u [out] upper bound
 *  @param  l [out] lower bound
 *  @param  a [out] index of the center
 */
/*===========================================================================*/
void Initialize(
    const size_t nclusters,
    const kvs::AnyValueTable& table,
    const kvs::ValueArray<kvs::Real32>* c,
    kvs::ValueArray<kvs::UInt32>& q,
    kvs::ValueArray<kvs::Real32>* cp,
    kvs::ValueArray<kvs::Real32>& u,
    kvs::ValueArray<kvs::Real32>& l,
    kvs::ValueArray<kvs::UInt32>& a )
{
    // Algorithm 2: INITIALIZE( c, x, q, c', u, l, a )

    for ( size_t j = 0; j < nclusters; j++ )
    {
        q[j] = 0;
        cp[j].fill( 0x00 );
    }

    const size_t nrows = table.column(0).size();
    const size_t ncolumns = table.columnSize();
    for ( size_t i = 0; i < nrows; i++ )
    {
        const kvs::ValueArray<kvs::Real32> xi = table.rowAsValueArray<kvs::Real32>(i);
        PointAllCtrs( nclusters, xi, c, a[i], u[i], l[i] );
        q[a[i]] += 1;
        for ( size_t k = 0; k < ncolumns; k++ )
        {
            cp[a[i]][k] += xi[k];
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Updates the center locations.
 *  @param  cp [in] set of the vector sum of all points
 *  @param  q [in] array of the number of points
 *  @param  c [out] updated cluster centers
 *  @param  p [out] array of the distance that the cluster center moved
 */
/*===========================================================================*/
void MoveCenters(
    const kvs::ValueArray<kvs::Real32>* cp,
    const kvs::ValueArray<kvs::UInt32>& q,
    kvs::ValueArray<kvs::Real32>* c,
    kvs::ValueArray<kvs::Real32>& p )
{
    // Algorithm 4: MOVE-CENTERS( c', q, c, p )

    const size_t nclusters = q.size();
    for ( size_t j = 0; j < nclusters; j++ )
    {
        kvs::ValueArray<kvs::Real32> cs = c[j].clone();

        const size_t nrows = cp[j].size();
        const kvs::Real32 qj = static_cast<kvs::Real32>( q[j] );
        for ( size_t k = 0; k < nrows; k++ )
        {
            c[j][k] = cp[j][k] / qj;
        }
        p[j] = ::GetEuclideanDistance( cs, c[j] );
    }
}

/*===========================================================================*/
/**
 *  @brief  Updates the upper and lower bounds.
 *  @param  p [in] array of the distance that the cluster center moved
 *  @param  a [in] array of index of the center
 *  @param  u [out] upper bound
 *  @param  l [out] lower bound
 */
/*===========================================================================*/
void UpdateBounds(
    const kvs::ValueArray<kvs::Real32>& p,
    const kvs::ValueArray<kvs::UInt32>& a,
    kvs::ValueArray<kvs::Real32>& u,
    kvs::ValueArray<kvs::Real32>& l )
{
    // Algorithm 5: UPDATE-BOUNDS( p, a, u, l )

    kvs::UInt32 r = 0;
    kvs::UInt32 rp = 0;

    kvs::Real32 pmax = kvs::Value<kvs::Real32>::Min();
    const size_t nclusters = p.size();
    for ( size_t j = 0; j < nclusters; j++ )
    {
        if ( p[j] > pmax )
        {
            pmax = p[j];
            r = j;
        }
    }

    pmax = kvs::Value<kvs::Real32>::Min();
    for ( size_t j = 0; j < nclusters; j++ )
    {
        if ( j != r )
        {
            if ( p[j] > pmax )
            {
                pmax = p[j];
                rp = j;
            }
        }
    }

    const size_t nrows = u.size();
    for ( size_t i = 0; i < nrows; i++ )
    {
        u[i] += p[a[i]];
        l[i] -= ( r == a[i] ) ? p[rp] : p[r];
    }
}

/*===========================================================================*/
/**
 *  @brief  Updates the number of points specified by the given index.
 *  @param  m [in] index of the center
 *  @param  a [in] array of index of the center
 *  @param  q [in/out] updated the array of the number of points
 */
/*===========================================================================*/
void Update(
    const size_t m,
    const kvs::ValueArray<kvs::UInt32>& a,
    kvs::ValueArray<kvs::UInt32>& q )
{
    size_t counter = 0;
    const size_t nrows = a.size();
    for ( size_t i = 0; i < nrows; i++ )
    {
        if ( a[i] == m ) counter++;
    }

    q[m] = counter;
}

/*===========================================================================*/
/**
 *  @brief  Updates the vector sum of all points specified by the given index.
 *  @param  m [in] index of the center
 *  @param  table [in] table data
 *  @param  a [in] array of index of the center
 *  @param  cp [out] set of the vector sum of all points
 */
/*===========================================================================*/
void Update(
    const size_t m,
    const kvs::AnyValueTable& table,
    const kvs::ValueArray<kvs::UInt32>& a,
    kvs::ValueArray<kvs::Real32>* cp )
{
    const size_t nrows = a.size();
    const size_t ncolumns = table.columnSize();

    cp[m].fill( 0x00 );
    for ( size_t i = 0; i < nrows; i++ )
    {
        if ( a[i] == m )
        {
            for ( size_t k = 0; k < ncolumns; k++ )
            {
                cp[m][k] += table.column(k).at<kvs::Real32>(i);
            }
        }
    }
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new FastKMeans class.
 */
/*===========================================================================*/
FastKMeans::FastKMeans():
    m_seeding_method( FastKMeans::SmartSeeding ),
    m_nclusters( 10 ),
    m_max_iterations( 100 ),
    m_tolerance( float(1.e-6) ),
    m_cluster_centers( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the FastKMeans class.
 */
/*===========================================================================*/
FastKMeans::~FastKMeans()
{
    if ( m_cluster_centers ) delete [] m_cluster_centers;
}

/*===========================================================================*/
/**
 *  @brief  Executes Hamerly's k-means clustering.
 */
/*===========================================================================*/
void FastKMeans::run()
{
    if ( m_input_table.empty() )
    {
        kvsMessageError("Input table data is not assigned.");
        return;
    }

    const size_t ncolumns = m_input_table.columnSize();
    const size_t nrows = m_input_table.column(0).size();
    for ( size_t i = 1; i < m_input_table.columnSize(); i++ )
    {
        if ( nrows != m_input_table.column(i).size() )
        {
            kvsMessageError("The number of rows is different between each column.");
            return;
        }
    }

    // Parameters that relate to cluster centers.
    /*   c:  cluster center
     *   cp: vector sum of all points in the cluster
     *   q:  number of points assigned to the cluster
     *   p:  distance that c last moved
     *   s:  distance from c to its closest other center
     */
    kvs::ValueArray<kvs::Real32>* c = new kvs::ValueArray<kvs::Real32> [ m_nclusters ];
    kvs::ValueArray<kvs::Real32>* cp = new kvs::ValueArray<kvs::Real32> [ m_nclusters ];
    kvs::ValueArray<kvs::UInt32> q( m_nclusters );
    kvs::ValueArray<kvs::Real32> p( m_nclusters );
    kvs::ValueArray<kvs::Real32> s( m_nclusters );

    for ( size_t j = 0; j < m_nclusters; j++ )
    {
        c[j].allocate( ncolumns );
        cp[j].allocate( ncolumns );
    }

    // Parameters that relate to data points.
    /*   a:  index of the center to which the data point x is assigned
     *   u:  upper bound on the distance between the data point x and
     *       its assigned center c(a)
     *   l:  lower bound on the distance between the data point x and
     *       its second closest center (the closest center to the data
     *       point that is not c(a))
     */
    kvs::ValueArray<kvs::UInt32> a( nrows );
    kvs::ValueArray<kvs::Real32> u( nrows );
    kvs::ValueArray<kvs::Real32> l( nrows );

    // Assign initial centers.
    switch ( m_seeding_method )
    {
    case RandomSeeding:
        ::InitializeCenterWithRandomSeeding( m_input_table, m_nclusters, m_random, c );
        break;
    case SmartSeeding:
        ::InitializeCenterWithSmartSeeding( m_input_table, m_nclusters, m_random, c );
        break;
    default:
        ::InitializeCenterWithRandomSeeding( m_input_table, m_nclusters, m_random, c );
        break;
    }

    // Initialize.
    ::Initialize( m_nclusters, m_input_table, c, q, cp, u, l, a );

    // Cluster IDs.
    kvs::ValueArray<kvs::UInt32> IDs;

    // Clustering.
    bool converged = false;
    size_t counter = 0;
    while ( !converged )
    {
        // Update s.
        for ( size_t j = 0; j < m_nclusters; j++ )
        {
            kvs::Real32 dmin = kvs::Value<kvs::Real32>::Max();
            for ( size_t jp = 0; jp < m_nclusters; jp++ )
            {
                if ( jp != j )
                {
                    const kvs::Real32 d = ::GetEuclideanDistance( c[jp], c[j] );
                    dmin = kvs::Math::Min( dmin, d );
                }
            }
            s[j] = dmin;
        }

        for ( size_t i = 0; i < nrows; i++ )
        {
            const kvs::Real32 m = kvs::Math::Max( s[a[i]] * 0.5f, l[i] );
            if ( u[i] > m ) // First bound test.
            {
                // Tighten upper bound.
                const kvs::ValueArray<kvs::Real32> xi = m_input_table.rowAsValueArray<kvs::Real32>(i);
                u[i] = ::GetEuclideanDistance( xi, c[a[i]] );
                if ( u[i] > m ) // Second bound test.
                {
                    const kvs::UInt32 ap = a[i];
                    ::PointAllCtrs( m_nclusters, xi, c, a[i], u[i], l[i] );
                    if ( ap != a[i] )
                    {
                        ::Update( ap, a, q );
                        ::Update( a[i], a, q );
                        ::Update( ap, m_input_table, a, cp );
                        ::Update( a[i], m_input_table, a, cp );
                    }
                }
            }
        }

        ::MoveCenters( cp, q, c, p );
        ::UpdateBounds( p, a, u, l );

        // Update cluster IDs.
        IDs = a;

        // Convergence test.
        converged = true;
        for ( size_t j = 0; j < m_nclusters; j++ )
        {
            if ( !( p[j] < m_tolerance ) ) { converged = false; break; }
        }

        if ( counter++ > m_max_iterations ) break;
    }

    if ( m_cluster_centers ) delete [] m_cluster_centers;
    m_cluster_centers = c;

    delete [] cp;

    m_cluster_ids = IDs;
}

} // end of namespace kvs
