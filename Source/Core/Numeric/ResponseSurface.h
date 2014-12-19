/*****************************************************************************/
/**
 *  @file   ResponseSurface.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResponseSurface.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__RESPONSE_SURFACE_H_INCLUDE
#define KVS__RESPONSE_SURFACE_H_INCLUDE

#include <string>
#include <vector>
#include <algorithm>
#include <kvs/BitArray>
#include <kvs/Assert>
#include <kvs/Vector2>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Vector>
#include <kvs/Matrix>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Response surface method class.
 */
/*===========================================================================*/
template <typename T>
class ResponseSurface : public kvs::Vector<T>
{
private:

    size_t m_npoints; ///< number of data points
    size_t m_nvariables; ///< number of independent variables (ex. x,y -> 2 )
    size_t m_nterms; ///< number of terms (ex. x,y -> 6 = {1, x, y, x^2, y^2, xy})
    kvs::BitArray m_mask; ///< mask table for the active coefficients
    kvs::Matrix<T> m_coefficient_matrix; ///< coeffiecinet matrix (matrix of independent variable)
    kvs::Vector<T> m_responses; ///< response values (vector of dependent variable)
    kvs::Vector<T> m_t_values; ///< t-values
    T m_r_square; ///< determination coefficient (R^2)
    T m_adjusted_r_square; ///< adjusted determination coefficient

public:

    ResponseSurface();
    ResponseSurface( const kvs::Matrix<T>& variables, const kvs::Vector<T>& responses );
    virtual ~ResponseSurface();

    const kvs::Vector<T>& solve( const kvs::Matrix<T>& variables, const kvs::Vector<T>& responses );
    const kvs::Vector<T>& improve( const T threshold = T( 2.45 ) );

    size_t numberOfPoints() const;
    size_t numberOfVariables() const;
    T Rsquare() const;
    T adjustedRsquare() const;
    const kvs::Vector<T>& Tvalues() const;

private:

    void solve_regression_coefficients();
    void create_coefficient_matrix( const kvs::Matrix<T>& variables );
    void update_coefficient_matrix();

#if 1 // KVS_ENABLE_DEPRECATED
public:
    size_t npoints() const { return this->numberOfPoints(); }
    size_t nvariables() const { return this->numberOfVariables(); }
#endif
};

} // end of namespace kvs

#endif // KVS__RESPONSE_SURFACE_H_INCLUDE
