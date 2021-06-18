/****************************************************************************/
/**
 *  @file   OrthoSlice.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
