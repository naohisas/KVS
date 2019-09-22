#include "LassoRegression.h"
#include <kvs/StudentTDistribution>
#include <kvs/Matrix>
#include <kvs/Vector>


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

template <typename T>
T SoftThresholding( const T x, const T lambda )
{
    if ( x > T(0) && lambda < kvs::Math::Abs( x ) ) return x - lambda;
    else if ( x < T(0) && lambda < kvs::Math::Abs( x ) ) return x + lambda;
    else return T(0);
}

template <typename T>
T Intercept( const kvs::Vector<T>& Y, const kvs::Matrix<T>& X, const kvs::Vector<T>& beta )
{
    const size_t n = Y.size();
    T sum = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += Y[i] - ( X[i].dot( beta ) - X[i][0] * beta[0] );
    }
    return sum / T(n);
}

template <typename T>
kvs::Vector<T> Column( const kvs::Matrix<T>& X, const size_t index )
{
    const size_t nrows = X.rowSize();
    kvs::Vector<T> column( nrows );
    for ( size_t i = 0; i < nrows; i++ ) { column[i] = X[i][index]; }
    return column;
}

} // end of namespace

namespace kvs
{

template <typename T>
LassoRegression<T>::LassoRegression():
    m_dof( 0 ),
    m_r2( 0.0 ),
    m_adjusted_r2( 0.0 ),
    m_complexity( 1.0 ),
    m_normalize( false )
{
}

template <typename T>
LassoRegression<T>::LassoRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep ):
    m_dof( 0 ),
    m_r2( 0.0 ),
    m_adjusted_r2( 0.0 ),
    m_complexity( 1.0 ),
    m_normalize( false )
{
    this->fit( dep, indep );
}

template <typename T>
void LassoRegression<T>::fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
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

    // Coordinate Descent Algorithm
    // T.T.Wu and K.Lange, Coordinate descent algorithms for lasso penalized regression,
    // The Annals of Applied Statistics, Vol.2, No.1, pp.224-244, 2008

    // Initialize coefficients
    m_coef.resize( ncols );
    m_coef[0] = ::Intercept<T>( Y, X, m_coef );
    const size_t max_iterations = 100;
    const float lambda = m_complexity * nrows;
    for ( size_t i = 0; i < max_iterations; i++ )
    {
        for ( size_t j = 1; j < ncols; j++ )
        {
            kvs::Vector<T> beta = m_coef; beta[j] = T(0);
            const kvs::Vector<T> rj = Y - X * beta;
            const kvs::Vector<T> Xj = ::Column( X, j );
            m_coef[j] = ::SoftThresholding<T>( Xj.dot( rj ), lambda ) / Xj.squaredLength();
            m_coef[0] = ::Intercept<T>( Y, X, m_coef );
        }
    }

    // Degree of freedom
    const size_t n = dep.size();
    const size_t k = indep.columnSize();
    m_dof = n - k - 1;

    // Fitting scores (R square)
    const kvs::Vector<T> y = X * m_coef;
    const kvs::Real64 rss = ( Y - y ).squaredLength();
    m_r2 = 1.0 - rss / ::DevSQ(Y);
    m_adjusted_r2 = 1.0 - ( 1.0 - m_r2 ) * ( n - 1.0 ) / m_dof;

    // Standard error
    const kvs::Matrix<T> Xt = X.transposed();
    const kvs::Matrix<T> XtX = Xt * X;
    const kvs::Matrix<T> XtX_inv = XtX.inverted();
    const kvs::Real64 ve = rss / m_dof;
    m_standard_errors.resize( m_coef.size() );
    for ( size_t i = 0; i < m_coef.size(); i++ )
    {
        m_standard_errors[i] = std::sqrt( ve * XtX_inv[i][i] );
    }
}

template <typename T>
void LassoRegression<T>::test()
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
template class LassoRegression<float>;
template class LassoRegression<double>;

} // end of namespace kvs
