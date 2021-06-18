/****************************************************************************/
/**
 *  @file   CellByCellRejectionSampling.h
 *  @author Takuma Kawamura, Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once

#include <kvs/MapperBase>
#include <kvs/Camera>
#include <kvs/PointObject>
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cell-by-cell particle generation class.
 */
/*===========================================================================*/
class CellByCellRejectionSampling : public kvs::MapperBase, public kvs::PointObject
{
    kvsModule( kvs::CellByCellRejectionSampling, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PointObject );

private:
    const kvs::Camera* m_camera = nullptr; ///< camera (reference)
    size_t m_repetition_level = 1; ///< repetition level
    float m_sampling_step = 0.5f; ///< sampling step in the object coordinate
    float m_object_depth = 0.0f; ///< object depth

public:
    CellByCellRejectionSampling() = default;
    CellByCellRejectionSampling(
        const kvs::VolumeObjectBase* volume,
        const size_t repetition_level = 1,
        const float sampling_step = 0.5f,
        const kvs::TransferFunction& transfer_function = kvs::TransferFunction(),
        const float object_depth = 0.0f );
    CellByCellRejectionSampling(
        const kvs::Camera* camera,
        const kvs::VolumeObjectBase* volume,
        const size_t repetition_level = 1,
        const float sampling_step = 0.5f,
        const kvs::TransferFunction& transfer_function = kvs::TransferFunction(),
        const float object_depth = 0.0f );

    SuperClass* exec( const kvs::ObjectBase* object );

    size_t repetitionLevel() const { return m_repetition_level; }
    float samplingStep() const { return m_sampling_step; }
    float objectDepth() const { return m_object_depth; }

    void attachCamera( const kvs::Camera* camera ) { m_camera = camera; }
    void setRepetitionLevel( const size_t repetition_level ) { m_repetition_level = repetition_level; }
    void setSamplingStep( const float step ) { m_sampling_step = step; }
    void setObjectDepth( const float depth ) { m_object_depth = depth; }

private:
    void mapping( const kvs::StructuredVolumeObject* volume );
    void mapping( const kvs::UnstructuredVolumeObject* volume );
    template <typename T>
    void generate_particles( const kvs::StructuredVolumeObject* volume );
    void generate_particles( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs
