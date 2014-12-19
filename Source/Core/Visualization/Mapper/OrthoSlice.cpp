/****************************************************************************/
/**
 *  @file   OrthoSlice.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OrthoSlice.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "OrthoSlice.h"
#include <kvs/Matrix33>


namespace
{

const kvs::Matrix33f Normal(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f );

}

namespace kvs
{

/*==========================================================================*/
/**
 *  Construct a new OrthoSlice class.
 */
/*==========================================================================*/
OrthoSlice::OrthoSlice():
    m_aligned_axis( OrthoSlice::XAxis )
{
}

/*==========================================================================*/
/**
 *  @brief  Construct and create a slice plane as a polygon object.
 *  @param  volume [in] pointer to the volume object
 *  @param  position [in] position on the specified axis
 *  @param  axis [in] aligned axis
 *  @param  transfer_function [in] transfer function
 */
/*==========================================================================*/
OrthoSlice::OrthoSlice(
    const kvs::VolumeObjectBase* volume,
    const float                  position,
    const AlignedAxis            axis,
    const kvs::TransferFunction& transfer_function ):
    kvs::SlicePlane(
        volume,
        ::Normal[axis] * position,
        ::Normal[axis],
        transfer_function )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets a plane information.
 *  @param  position [in] slice position
 *  @param  axis [in] slice orientation
 */
/*===========================================================================*/
void OrthoSlice::setPlane( const float position, const kvs::OrthoSlice::AlignedAxis axis )
{
    SuperClass::setPlane( ::Normal[axis] * position, ::Normal[axis] );
}

} // end of namespace kvs
