/*****************************************************************************/
/**
 *  @file   PreIntegrationTable2D.cpp
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
 * [1] K.Engel, M.Kraus, and T.Ertl, "High-Quality Pre-Integration Volume
 *     Rendering Using Hardware-Accelerated Pixel Shading," In Proc. of
 *     Eurographics/SIGGRAPH Workshop on Graphics Hardware, pp.9-16, 2001.
 */
/*****************************************************************************/
#include "PreIntegrationTable2D.h"
#include <kvs/Assert>
#include <kvs/Math>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns tau calculated from opacity.
 *  @param  alpha [in] opacity value
 *  @return tau
 */
/*===========================================================================*/
double AlphaToTau( double alpha )
{
    return -std::log( kvs::Math::Max( 1.0 - alpha, 0.000001 ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns float type array converted from double type array.
 *  @param  src [in] double type array
 */
/*===========================================================================*/
kvs::ValueArray<float> DoubleToFloat( const kvs::ValueArray<double>& src )
{
    kvs::ValueArray<float> dst( src.size() );
    for ( size_t i = 0; i < src.size(); ++i )
    {
        dst[i] = static_cast<float>( src[i] );
    }

    return dst;
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns the tau value array.
 *  @return tau value array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> PreIntegrationTable2D::tau() const
{
    return ::DoubleToFloat( m_tau );
}

/*===========================================================================*/
/**
 *  @brief  Returns the T value array.
 *  @return T value array
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> PreIntegrationTable2D::T() const
{
    return ::DoubleToFloat( m_T );
}

/*===========================================================================*/
/**
 *  @brief  Returns the inverse numbers of T values.
 *  @param  resolution [in] resolution of the inverse T values
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> PreIntegrationTable2D::inverseT( const size_t resolution ) const
{
    KVS_ASSERT( resolution > 1 );

    kvs::ValueArray<kvs::Real64> T = m_T;
    const size_t table_size = T.size();
    KVS_ASSERT( table_size > 1 );

    const double max_T = T.back();
    const float L = 1.0f / ( table_size - 1 );
    kvs::ValueArray<kvs::Real32> T_inv( resolution );
    size_t scalar_index = 1;
    double T0 = T[0];
    double T1 = T[1];
    double s0 = 0.0;
    for ( size_t i = 0; i < resolution; i++ )
    {
        const double value_T = i * max_T / ( resolution - 1 );
        while ( value_T >= T1 )
        {
            ++scalar_index;
            if ( scalar_index >= table_size ) { break; }

            T1 = T[ scalar_index ];
            T0 = T[ scalar_index - 1 ];
            s0 = ( scalar_index - 1 ) * L;
        }
        double s = s0 + L * ( value_T - T0 ) / ( T1 - T0 );
        T_inv[i] = static_cast<double>( s );
    }
    T_inv.back() = 1.0f;

    return T_inv;
}

/*===========================================================================*/
/**
 *  @brief  Sets a transfer function.
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
void PreIntegrationTable2D::setTransferFunction( const kvs::TransferFunction& transfer_function )
{
    const kvs::ValueArray<kvs::Real32> omap = transfer_function.opacityMap().table();
    const size_t resolution = omap.size();
    KVS_ASSERT( resolution > 1 );

    kvs::ValueArray<kvs::Real64> tau( resolution );
    for ( size_t i = 0; i < resolution; i++ )
    {
        tau[i] = ::AlphaToTau( omap[i] );
    }

    kvs::ValueArray<kvs::Real64> T( resolution );
    const double L = 1.0f / ( resolution - 1 );
    double Tau = 0.0f;
    double tau0 = tau[0];
    T[0] = Tau;
    for ( size_t i = 1; i < resolution; i++ )
    {
        double tau1 = tau[i];
        Tau += L / 2.0 * ( tau1 + tau0 );
        T[i] = Tau;
        tau0 = tau1;
    }

    m_tau = tau;
    m_T = T;
}

/*===========================================================================*/
/**
 *  @brief  Creates pre-integration table.
 */
/*===========================================================================*/
void PreIntegrationTable2D::create()
{
    kvs::ValueArray<kvs::Real64> T = m_T;
    kvs::ValueArray<kvs::Real64> tau = m_tau;

    const size_t resolution = tau.size();
    kvs::ValueArray<kvs::Real32> table( resolution * resolution );
    for ( size_t i = 0, index = 0; i < resolution; i++ )
    {
        for ( size_t j = 0; j < resolution; j++, index++ )
        {
            if ( i == j )
            {
                table[index] = tau[i];
            }
            else
            {
                const double sf = j / static_cast<double>( resolution - 1 );
                const double sb = i / static_cast<double>( resolution - 1 );
                table[index] = ( T[i] - T[j] ) / ( sb - sf );
            }
        }
    }

    m_table = table;
}

} // end of namespace kvs
