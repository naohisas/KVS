/*****************************************************************************/
/**
 *  @file   PreIntegrationTable2D.h
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
#pragma once

#include <kvs/ValueArray>
#include <kvs/TransferFunction>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PreIntegrationTable2D class
 */
/*===========================================================================*/
class PreIntegrationTable2D
{
private:

    kvs::ValueArray<kvs::Real64> m_tau; ///< extinction densities
    kvs::ValueArray<kvs::Real64> m_T; ///< integral of the tau
    kvs::ValueArray<kvs::Real32> m_table; ///< 2D pre-integration table

public:

    PreIntegrationTable2D() {}

    const kvs::ValueArray<kvs::Real32> tau() const;
    const kvs::ValueArray<kvs::Real32> T() const;
    const kvs::ValueArray<kvs::Real32>& table() const { return m_table; }
    kvs::ValueArray<kvs::Real32> inverseT( const size_t resolution ) const;

    void setTransferFunction( const kvs::TransferFunction& transfer_function );
    void create();
};

} // end of namespace kvs
