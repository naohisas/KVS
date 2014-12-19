/*****************************************************************************/
/**
 *  @file   CellByCellLayeredSampling.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellByCellLayeredSampling.h 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__CELL_BY_CELL_LAYERED_SAMPLING_H_INCLUDE
#define KVS__CELL_BY_CELL_LAYERED_SAMPLING_H_INCLUDE

#include <kvs/MapperBase>
#include <kvs/Camera>
#include <kvs/PointObject>
#include <kvs/VolumeObjectBase>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/TetrahedralCell>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Cell-by-cell particle generation class.
 */
/*===========================================================================*/
class CellByCellLayeredSampling : public kvs::MapperBase, public kvs::PointObject
{
    kvsModule( kvs::CellByCellLayeredSampling, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PointObject );

protected:

    struct SelectedParticles
    {
        size_t nparticles; ///< number of selected particles
        kvs::ValueArray<kvs::UInt32> indices; ///< index array of the selected particles
    };

private:

    const kvs::Camera* m_camera; ///< camera (reference)
    size_t m_subpixel_level; ///< subpixel level
    float m_sampling_step; ///< sampling step in the object coordinate
    float m_object_depth; ///< object depth
    kvs::ValueArray<float> m_density_map; ///< density map
    kvs::PointObject* m_pregenerated_particles; ///< pregenerated particles
    SelectedParticles m_selected_particles; ///< particles selected from the pregenerated particles
    kvs::Real32 m_M_value;  ///< numerical integration value of density distribution
    kvs::Matrix44f m_L_matrix; ///< conversion matrix
    kvs::Matrix44f m_A_matrix; ///< normalization conversion matrix

public:

    CellByCellLayeredSampling();
    CellByCellLayeredSampling(
        const kvs::VolumeObjectBase* volume,
        const size_t subpixel_level,
        const float sampling_step,
        const kvs::TransferFunction& transfer_function,
        const float object_depth = 0.0f );
    CellByCellLayeredSampling(
        const kvs::Camera* camera,
        const kvs::VolumeObjectBase* volume,
        const size_t subpixel_level,
        const float sampling_step,
        const kvs::TransferFunction& transfer_function,
        const float object_depth = 0.0f );
    virtual ~CellByCellLayeredSampling();

    SuperClass* exec( const kvs::ObjectBase* object );

    size_t subpixelLevel() const;
    float samplingStep() const;
    float objectDepth() const;

    void attachCamera( const kvs::Camera* camera );
    void setSubpixelLevel( const size_t subpixel_level );
    void setSamplingStep( const float sampling_step );
    void setObjectDepth( const float object_depth );

private:

    void mapping( const kvs::Camera* camera, const kvs::UnstructuredVolumeObject* volume );
    void generate_particles( const kvs::UnstructuredVolumeObject* volume );
    void pregenerate_particles( const size_t nparticles );
    void uniform_sampling(
        const kvs::TetrahedralCell* cell,
        const kvs::TransferFunction& tfunc,
        const size_t nparticles,
        std::vector<kvs::Real32>* coords,
        std::vector<kvs::UInt8>* colors,
        std::vector<kvs::Real32>* normals );
    void rejection_sampling(
        const kvs::TetrahedralCell* cell,
        const kvs::TransferFunction& tfunc,
        const size_t nparticles,
        std::vector<kvs::Real32>* coords,
        std::vector<kvs::UInt8>* colors,
        std::vector<kvs::Real32>* normals );
    void roulette_selection(
        const kvs::TetrahedralCell* cell,
        const kvs::TransferFunction& tfunc,
        const size_t nparticles,
        std::vector<kvs::Real32>* coords,
        std::vector<kvs::UInt8>* colors,
        std::vector<kvs::Real32>* normals );
    float calculate_density( const float scalar );
    float calculate_maximum_density( const float scalar0, const float scalar1 );
    size_t calculate_number_of_particles( const float density, const kvs::TetrahedralCell* cell );
    size_t calculate_number_of_particles( const size_t nparticles_in_cell );
    void calculate_particles_in_cell( const kvs::TetrahedralCell* cell );
};

} // end of namespace kvs

#endif // KVS__CELL_BY_CELL_LAYERED_SAMPLING_H_INCLUDE
