/*****************************************************************************/
/**
 *  @file   response_surface.cpp
 *  @brief  Response surface method class
 *
 *  @author Yukio YASUHARA
 *  @date   2019/09/22 10:26:16
 */
/*----------------------------------------------------------------------------
 *
 *  $Author: naohisa $
 *  $Date: 2007/11/16 11:38:50 $
 *  $Source: /home/Repository/viz-server2/cvsroot/KVS/Library/Numeric/response_surface.cpp,v $
 *  $Revision: 1.7 $
 */
/*****************************************************************************/
#include "ResponseSurface.h"
#include <kvs/Math>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a number of terms.
 *  @param  nvariables [in] number of independent variables
 *  @return number of terms
 */
/*===========================================================================*/
size_t GetNumberOfTerms( const size_t nvariables )
{
    return( 1 + 2 * nvariables + ( nvariables - 1 ) * nvariables / 2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a sum of elements of the given vector.
 *  @param  v [in] vector
 *  @return sum of elements
 */
/*===========================================================================*/
template <typename T>
T GetSumOfElements( const kvs::Vector<T>& v )
{
    T sum = 0;
    const size_t nelements = v.size();
    for ( size_t i = 0; i < nelements; i++ ) sum += v[i];

    return( sum );
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResponseSurface class.
 */
/*===========================================================================*/
template <typename T>
ResponseSurface<T>::ResponseSurface():
    m_npoints( 0 ),
    m_nvariables( 0 ),
    m_nterms( 0 ),
    m_r_square( T(0) ),
    m_adjusted_r_square( T(0) )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResponseSurface class.
 *  @param  variables [in] matrix of independent variables
 *  @param  responses [in] vector of dependent variables
 */
/*===========================================================================*/
template <typename T>
ResponseSurface<T>::ResponseSurface(
    const kvs::Matrix<T>& variables,
    const kvs::Vector<T>& responses )
{
    this->solve( variables, responses );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ResponseSurface class.
 */
/*===========================================================================*/
template <typename T>
ResponseSurface<T>::~ResponseSurface()
{
}

/*===========================================================================*/
/**
 *  @brief  Solves the response surface model.
 *  @param  variables [in] matrix of independent variables
 *  @param  responses [in] vector of dependent variables
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& ResponseSurface<T>::solve(
    const kvs::Matrix<T>& variables,
    const kvs::Vector<T>& responses )
{
    m_npoints = variables.rowSize();
    m_nvariables = variables.columnSize();
    m_nterms = GetNumberOfTerms( m_nvariables );
    m_responses = responses;

    kvs::Vector<T>::resize( m_nterms );
    m_t_values.resize( m_nterms );
    m_mask.allocate( m_nterms );
    m_mask.set(); // all coefficients are active.

    this->create_coefficient_matrix( variables );
    this->solve_regression_coefficients();

    return( *this );
}

/*===========================================================================*/
/**
 *  @brief  Improves the response surface model by applying the t-test.
 *  @param  threshold [in] threshold
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& ResponseSurface<T>::improve( const T threshold )
{
    if ( m_npoints == 0 ) return( *this );

    m_nterms = 0;
    for ( size_t i = 0; i < kvs::Vector<T>::size(); i++ )
    {
        if( m_mask.test(i) )
        {
            if( kvs::Math::Abs( m_t_values[i] ) < threshold ) m_mask.reset(i);
            else m_nterms++;
        }
    }

    this->solve_regression_coefficients();

    return( *this );
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of data points.
 *  @return number of data points
 */
/*===========================================================================*/
template <typename T>
size_t ResponseSurface<T>::numberOfPoints() const
{
    return( m_npoints );
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of independent variables.
 *  @return number of independent variables
 */
/*===========================================================================*/
template <typename T>
size_t ResponseSurface<T>::numberOfVariables() const
{
    return( m_nvariables );
}

/*===========================================================================*/
/**
 *  @brief  Returns a determination coefficient (R-square).
 *  @return determination coefficient (R-square)
 */
/*===========================================================================*/
template <typename T>
T ResponseSurface<T>::Rsquare() const
{
    return( m_r_square );
}

/*===========================================================================*/
/**
 *  @brief  Returns an adjusted determination coefficient.
 *  @return adjusted determination coefficient
 */
/*===========================================================================*/
template <typename T>
T ResponseSurface<T>::adjustedRsquare() const
{
    return( m_adjusted_r_square );
}

/*===========================================================================*/
/**
 *  @brief  Returns T-test results.
 *  @return T-test results
 */
/*===========================================================================*/
template <typename T>
const kvs::Vector<T>& ResponseSurface<T>::Tvalues() const
{
    return( m_t_values );
}

/*===========================================================================*/
/**
 *  @brief  Solves regression coefficients.
 */
/*===========================================================================*/
template <typename T>
void ResponseSurface<T>::solve_regression_coefficients()
{
    this->update_coefficient_matrix();

    const kvs::Matrix<T> X = m_coefficient_matrix; // variable matrix
    const kvs::Vector<T> y = m_responses;          // response vector
    const size_t n = m_responses.size();           // num. of responses

    const kvs::Matrix<T> Xt = X.transposed();      // X^{t}
    const kvs::Matrix<T> XtX = Xt * X;             // X^{t} X
    const kvs::Vector<T> Xty = Xt * y;             // X^{t} y
    const kvs::Matrix<T> invXtX = XtX.inverted();  // ( X^{t} X )^{-1}
    const kvs::Vector<T> b = invXtX * Xty;         // ( X^{t} X )^{-1} X^{t} y
    const T btXty = b.dot( Xty );                  // b^{t} X^{t} y

    /* Calculate adjusted coefficient of multiple determination
     *     SSE : Square Sum of Errors
     *     SSR : Regression Sum of Squares
     *     Syy : total sum of squares
     *     sigma2 : error covariance
     */
    const T SSE = y.dot( y ) - btXty;
    const T SSR = btXty - kvs::Math::Square( GetSumOfElements( y ) ) / n;
    const T Syy = SSR + SSE;
    const T sigma2 = SSE / ( n - m_nterms );

    m_r_square = T(1) - ( SSE * ( n - 1 ) ) / ( Syy * ( n - m_nterms ) );
    m_adjusted_r_square = T(1) - SSE / Syy;

    // T-test.
    kvs::Vector<T> t( m_nterms );
    for ( size_t i = 0; i < m_nterms; i++ )
    {
        t[i] = b[i] / static_cast<T>(sqrt( (double)(sigma2 * invXtX[i][i]) ));
    }

    for ( size_t i = 0, index = 0; i < kvs::Vector<T>::size(); i++ )
    {
        (*this)[i] = T(0);
        m_t_values[i] = T(0);
        if ( m_mask.test(i) )
        {
            (*this)[i] = b[index];
            m_t_values[i] = t[index];
            index++;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a coefficient matrix.
 *  @param  variables [in] matrix of independent variables
 */
/*===========================================================================*/
template <typename T>
void ResponseSurface<T>::create_coefficient_matrix(
    const kvs::Matrix<T>& variables )
{
    m_coefficient_matrix.resize( m_responses.size(), m_nterms );
    for ( size_t row = 0; row < m_responses.size(); row++ )
    {
        m_coefficient_matrix[row][0] = 1;
        for ( size_t i = 0; i < m_nvariables; i++ )
        {
            // Calculate first-order term's value (ex. x, y, z, ...)
            m_coefficient_matrix[row][i+1] = variables[row][i];

            // Calculate second-order term's value (ex. x^2, y^2, z^2, ...)
            m_coefficient_matrix[row][i+1+m_nvariables] = kvs::Math::Square( variables[row][i] );
        }

        // Caluculate another term's value (ex. xy, yz, ...)
        for ( size_t i = 0, index = 2 * m_nvariables + 1; i < m_nvariables; i++ )
        {
            for ( size_t j = i + 1; j < m_nvariables; j++ )
            {
                m_coefficient_matrix[row][index] = variables[row][i] * variables[row][j];
                index++;
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Updates a coefficient matrix.
 */
/*===========================================================================*/
template <typename T>
void ResponseSurface<T>::update_coefficient_matrix()
{
    const size_t nrows = m_coefficient_matrix.rowSize();
    const size_t ncolumns = m_coefficient_matrix.columnSize();

    kvs::Matrix<T> temp( m_responses.size(), m_nterms );
    for ( size_t j = 0, index = 0; j < ncolumns; j++ )
    {
        if ( m_mask.test(j) )
        {
            for( size_t i = 0; i < nrows; i++ )
            {
                temp[i][index] = m_coefficient_matrix[i][j];
            }
            index++;
        }
    }

    m_coefficient_matrix = temp;
}

// template instantiation
template class ResponseSurface<float>;
template class ResponseSurface<double>;

} // end of namespace kvs
