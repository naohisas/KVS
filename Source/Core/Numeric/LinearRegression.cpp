#include "LinearRegression.h"
#include <kvs/Assert>
#include <kvs/Vector>
#include <kvs/Matrix>
#include <kvs/StudentTDistribution>


namespace
{

template <typename T>
T DevSQ( const kvs::Vector<T>& vec )
{
    const size_t n = vec.size();
    T sum = T(0);
    T sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += vec[i];
        sum2 += vec[i] * vec[i];
    }

    // Sum of squared deviations
    return sum2 - ( sum * sum ) / n;
}

}

namespace kvs
{

template <typename T>
LinearRegression<T>::LinearRegression():
    m_dof( 0 ),
    m_r2( 0.0 ),
    m_adjusted_r2( 0.0 )
{
}

template <typename T>
LinearRegression<T>::LinearRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep ):
    m_dof( 0 ),
    m_r2( 0.0 ),
    m_adjusted_r2( 0.0 )
{
    this->fit( dep, indep );
}

template <typename T>
void LinearRegression<T>::fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
{
    KVS_ASSERT( dep.size() == indep.column(0).size() );

    // Dependent (Y) and independent (Xi) variables
    const size_t nrows = dep.size();
    const size_t ncols = indep.columnSize() + 1;
    kvs::Vector<T> Y( nrows );
    kvs::Matrix<T> X( nrows, ncols );
    for ( size_t i = 0; i < nrows; i++ )
    {
        kvs::Vector<T>& Xi = X[i];
        Y[i] = dep[i];
        Xi[0] = 1.0f;
        for ( size_t j = 1; j < ncols; j++ )
        {
            Xi[j] = indep[j-1][i];
        }
    }

    // Degree of freedom
    const size_t n = dep.size();
    const size_t k = indep.columnSize();
    m_dof = n - k - 1;

    // Regression coefficients
    const kvs::Matrix<T> Xt = X.transposed();
    const kvs::Matrix<T> XtX = Xt * X;
    const kvs::Vector<T> XtY = Xt * Y;
    const kvs::Matrix<T> XtX_inv = XtX.inverted();
    m_coef = XtX_inv * XtY;

    // R square
    const kvs::Vector<T> y = X * m_coef;
    const kvs::Real64 rss = ( Y - y ).squaredLength();
    m_r2 = 1.0 - rss / ::DevSQ(Y);
    m_adjusted_r2 = 1.0 - ( 1.0 - m_r2 ) * ( n - 1.0 ) / m_dof;

    // Standard error
    const kvs::Real64 ve = rss / m_dof;
    m_standard_errors.resize( m_coef.size() );
    for ( size_t i = 0; i < m_coef.size(); i++ )
    {
        m_standard_errors[i] = std::sqrt( ve * XtX_inv[i][i] );
    }
}

template <typename T>
void LinearRegression<T>::test()
{
    m_t_values.resize( m_coef.size() );
    m_p_values.resize( m_coef.size() );
    kvs::StudentTDistribution tdist( m_dof );
    for ( size_t i = 0; i < m_coef.size(); i++ )
    {
        m_t_values[i] = m_coef[i] / m_standard_errors[i];
        m_p_values[i] = 2.0 * ( 1.0 - tdist.cdf( m_t_values[i] ) );
    }
}

// template instantiation
template class LinearRegression<float>;
template class LinearRegression<double>;

} // end of namespace kvs
