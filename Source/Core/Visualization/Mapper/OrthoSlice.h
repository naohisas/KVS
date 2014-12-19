/****************************************************************************/
/**
 *  @file   OrthoSlice.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OrthoSlice.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__ORTHO_SLICE_H_INCLUDE
#define KVS__ORTHO_SLICE_H_INCLUDE

#include <kvs/SlicePlane>
#include <kvs/VolumeObjectBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Axis aligned slice plane class.
 */
/*==========================================================================*/
class OrthoSlice : public kvs::SlicePlane
{
    kvsModule( kvs::OrthoSlice, Mapper );
    kvsModuleSuperClass( kvs::SlicePlane );

public:

    enum AlignedAxis
    {
        XAxis = 0,
        YAxis = 1,
        ZAxis = 2
    };

protected:

    AlignedAxis m_aligned_axis; ///< aligned axis

public:

    OrthoSlice();
    OrthoSlice(
        const kvs::VolumeObjectBase* volume,
        const float position,
        const AlignedAxis aligned_axis,
        const kvs::TransferFunction& transfer_function );

    void setPlane( const float position, const kvs::OrthoSlice::AlignedAxis axis );
};

} // end of namespace kvs

#endif // KVS__ORTHO_SLICE_H_INCLUDE
