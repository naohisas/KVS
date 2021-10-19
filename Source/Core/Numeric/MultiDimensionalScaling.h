/*****************************************************************************/
/**
 *  @file   MultiDimensionalScaling.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/ValueTable>
#include <kvs/Matrix>
#include <functional>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Multi-dimensional Scaling (MDS) class.
 */
/*===========================================================================*/
template <typename T>
class MultiDimensionalScaling
{
public:
    using Distance = std::function<T(const kvs::ValueTable<T>&,const size_t, const size_t)>;
    static T Euclidean( const kvs::ValueTable<T>& data, const size_t i, const size_t j );
    static T Manhattan( const kvs::ValueTable<T>& data, const size_t i, const size_t j );
    static kvs::Matrix<T> DistanceMatrix( const kvs::ValueTable<T>& data, Distance distance = Euclidean );

private:
    Distance m_distance = Euclidean; // distance function
    size_t m_ncomponents = 0; /// number of components (if 0, dimension of the input data matrix)
    kvs::Matrix<T> m_embedded_points{};

public:
    MultiDimensionalScaling( const size_t ncomps = 0 ): m_ncomponents( ncomps ) {}
    MultiDimensionalScaling( const kvs::ValueTable<T>& data, const size_t ncomponents = 0 );
    MultiDimensionalScaling( const kvs::ValueTable<T>& data, Distance distance, const size_t ncomponents = 0 );
    MultiDimensionalScaling( const kvs::Matrix<T>& matrix, const size_t ncomponents = 0 );

    void setNumberOfComponents( const size_t ncomponents ) { m_ncomponents = ncomponents; }
    void setDistance( Distance distance ) { m_distance = distance; }
    size_t numberOfComponents() const { return m_ncomponents; }
    const kvs::Matrix<T>& embeddedPoints() const { return m_embedded_points; }

    void fit( const kvs::ValueTable<T>& data );
    void fit( const kvs::Matrix<T>& matrix );
    kvs::ValueTable<T> transform( const kvs::ValueTable<T>& data );
    kvs::ValueTable<T> transform( const kvs::Matrix<T>& matrix );
};

} // end of namespace kvs
