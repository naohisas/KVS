/****************************************************************************/
/**
 *  @file   CellByCellRejectionSampling.cpp
 *  @author Takum Kawamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellByCellRejectionSampling.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "CellByCellRejectionSampling.h"
#include <vector>
#include <kvs/OpenMP>
#include <kvs/DebugNew>
#include <kvs/Camera>
#include <kvs/TrilinearInterpolator>
#include <kvs/Value>
#include <kvs/CellBase>
#include <kvs/Math>
#include "CellByCellSampling.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellRejectionSampling class.
 */
/*===========================================================================*/
CellByCellRejectionSampling::CellByCellRejectionSampling():
    kvs::MapperBase(),
    kvs::PointObject(),
    m_camera( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellRejectionSampling class.
 *  @param  volume [in] pointer to the volume object
 *  @param  repetition_level [in] repetition level
 *  @param  sampling_step [in] sapling step
 *  @param  transfer_function [in] transfer function
 *  @param  object_depth [in] depth value of the input volume at the CoG
 */
/*===========================================================================*/
CellByCellRejectionSampling::CellByCellRejectionSampling(
    const kvs::VolumeObjectBase* volume,
    const size_t repetition_level,
    const float sampling_step,
    const kvs::TransferFunction& transfer_function,
    const float object_depth ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject(),
    m_camera( 0 )
{
    this->setRepetitionLevel( repetition_level );
    this->setSamplingStep( sampling_step );
    this->setObjectDepth( object_depth );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellRejectionSampling class.
 *  @param  camera [in] pointer to the camera
 *  @param  volume [in] pointer to the volume object
 *  @param  repetition_level [in] repetition level
 *  @param  sampling_step [in] sapling step
 *  @param  transfer_function [in] transfer function
 *  @param  object_depth [in] depth value of the input volume at the CoG
 */
/*===========================================================================*/
CellByCellRejectionSampling::CellByCellRejectionSampling(
    const kvs::Camera* camera,
    const kvs::VolumeObjectBase* volume,
    const size_t repetition_level,
    const float sampling_step,
    const kvs::TransferFunction& transfer_function,
    const float object_depth ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject()
{
    this->attachCamera( camera ),
    this->setRepetitionLevel( repetition_level );
    this->setSamplingStep( sampling_step );
    this->setObjectDepth( object_depth );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return pointer to the point object
 */
/*===========================================================================*/
CellByCellRejectionSampling::SuperClass* CellByCellRejectionSampling::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    bool delete_camera = false;
    if ( !m_camera )
    {
        m_camera = new kvs::Camera();
        delete_camera = true;
    }

    const kvs::VolumeObjectBase::VolumeType volume_type = volume->volumeType();
    if ( volume_type == kvs::VolumeObjectBase::Structured )
    {
        this->mapping( kvs::StructuredVolumeObject::DownCast( object ) );
    }
    else // volume_type == kvs::VolumeObjectBase::Unstructured
    {
        this->mapping( kvs::UnstructuredVolumeObject::DownCast( object ) );
    }

    if ( delete_camera )
    {
        delete m_camera;
        m_camera = 0;
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Mapping for the structured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellRejectionSampling::mapping( const kvs::StructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object and set the min/max coordinates.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Generate the particles.
    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) this->generate_particles<kvs::Int8>( volume );
    else if ( type == typeid( kvs::Int16  ) ) this->generate_particles<kvs::Int16>( volume );
    else if ( type == typeid( kvs::Int32  ) ) this->generate_particles<kvs::Int32>( volume );
    else if ( type == typeid( kvs::UInt8  ) ) this->generate_particles<kvs::UInt8>( volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->generate_particles<kvs::UInt16>( volume );
    else if ( type == typeid( kvs::UInt32 ) ) this->generate_particles<kvs::UInt32>( volume );
    else if ( type == typeid( kvs::Real32 ) ) this->generate_particles<kvs::Real32>( volume );
    else if ( type == typeid( kvs::Real64 ) ) this->generate_particles<kvs::Real64>( volume );
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Mapping for the unstructured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellRejectionSampling::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object and set the min/max coordinates.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Generate the particles.
    this->generate_particles( volume );
}

/*===========================================================================*/
/**
 *  @brief  Generates particles for the structured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
template <typename T>
void CellByCellRejectionSampling::generate_particles( const kvs::StructuredVolumeObject* volume )
{
    CellByCellSampling::ParticleDensityMap density_map;
    density_map.setSamplingStep( m_sampling_step );
    density_map.attachCamera( m_camera );
    density_map.attachObject( volume );
    density_map.create( BaseClass::transferFunction().opacityMap() );

    const kvs::Vec3ui ncells( volume->resolution() - kvs::Vec3u::Constant(1) );
    const kvs::ColorMap color_map( BaseClass::transferFunction().colorMap() );

    // Calculate number of particles.
    size_t N = 0;
    kvs::ValueArray<kvs::UInt32> nparticles( ncells.x() * ncells.y() * ncells.z() );
    KVS_OMP_PARALLEL()
    {
        kvs::TrilinearInterpolator interpolator( volume );
        CellByCellSampling::GridSampler<T> sampler( &interpolator, &density_map );

        sampler.bind( kvs::Vec3u( 0, 0, 0 ) );

        KVS_OMP_FOR( reduction(+:N) )
        for ( kvs::UInt32 z = 0; z < ncells.z(); ++z )
        {
            size_t cell_index_counter = z * ncells.x() * ncells.y();
            for ( kvs::UInt32 y = 0; y < ncells.y(); ++y )
            {
                for ( kvs::UInt32 x = 0; x < ncells.x(); ++x )
                {
                    sampler.bind( kvs::Vec3ui( x, y, z ) );
                    const size_t n = sampler.numberOfParticles();
                    const kvs::UInt32 index = cell_index_counter++;
                    nparticles[index] = n;
                    N += n;
                }
            }
        }
    }

    // Generate particles for each cell.
    const kvs::UInt32 repetitions = m_repetition_level;
    CellByCellSampling::ColoredParticles particles( color_map );
    particles.allocate( N * repetitions );
    KVS_OMP_PARALLEL()
    {
        kvs::TrilinearInterpolator interpolator( volume );
        CellByCellSampling::GridSampler<T> sampler( &interpolator, &density_map );

        KVS_OMP_FOR( schedule(dynamic) )
        for ( kvs::UInt32 r = 0; r < repetitions; r++ )
        {
            size_t cell_index_counter = 0;
            size_t particle_index_counter = N * r;
            for ( kvs::UInt32 z = 0; z < ncells.z(); ++z )
            {
                for ( kvs::UInt32 y = 0; y < ncells.y(); ++y )
                {
                    for ( kvs::UInt32 x = 0; x < ncells.x(); ++x )
                    {
                        const kvs::UInt32 index = cell_index_counter++;
                        const size_t n = nparticles[index];
                        if ( n == 0 ) continue;

                        sampler.bind( kvs::Vec3ui( x, y, z ) );
                        const kvs::Real32 max_density = density_map.maxValueInGrid<T>( interpolator, volume );
                        const kvs::Real32 pmax = max_density / n;

                        size_t counter = 0;
                        while ( counter < n )
                        {
                            const kvs::Real32 density = sampler.sample();
                            const kvs::Real32 p = density / n;
                            const kvs::Real32 R = CellByCellSampling::RandomNumber();
                            if ( p > pmax * R )
                            {
                                const CellByCellSampling::Particle& p = sampler.accept();
                                const size_t particle_index = particle_index_counter++;
                                particles.push( particle_index, p );

                                counter++;
                            }
                        }
                    }
                }
            }
        }
    }

    SuperClass::setCoords( particles.coords() );
    SuperClass::setColors( particles.colors() );
    SuperClass::setNormals( particles.normals() );
    SuperClass::setSize( 1.0f );
}

/*===========================================================================*/
/**
 *  @brief  Generates particles for the unstructured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellRejectionSampling::generate_particles( const kvs::UnstructuredVolumeObject* volume )
{
    CellByCellSampling::ParticleDensityMap density_map;
    density_map.setSamplingStep( m_sampling_step );
    density_map.attachCamera( m_camera );
    density_map.attachObject( volume );
    density_map.create( BaseClass::transferFunction().opacityMap() );

    const size_t ncells = volume->numberOfCells();
    const kvs::ColorMap color_map( BaseClass::transferFunction().colorMap() );

    // Calculate number of particles
    size_t N = 0;
    kvs::ValueArray<kvs::UInt32> nparticles( ncells );
    KVS_OMP_PARALLEL()
    {
        kvs::CellBase* cell = CellByCellSampling::Cell( volume );
        CellByCellSampling::CellSampler sampler( cell, &density_map );

        KVS_OMP_FOR( reduction(+:N) )
        for ( size_t index = 0; index < ncells; ++index )
        {
            sampler.bind( index );
            const size_t n = sampler.numberOfParticles();
            nparticles[index] = n;

            N += n;
        }

        delete cell;
    }

    // Generate particles for each cell.
    const kvs::UInt32 repetitions = m_repetition_level;
    CellByCellSampling::ColoredParticles particles( color_map );
    particles.allocate( N * repetitions );
    KVS_OMP_PARALLEL()
    {
        kvs::CellBase* cell = CellByCellSampling::Cell( volume );
        CellByCellSampling::CellSampler sampler( cell, &density_map );

        KVS_OMP_FOR( schedule(dynamic) )
        for ( kvs::UInt32 r = 0; r < repetitions; ++r )
        {
            size_t particle_index_counter = N * r;
            for ( size_t index = 0; index < ncells; ++index )
            {
                const size_t n = nparticles[index];
                if ( n == 0 ) continue;

                sampler.bind( index );

                const kvs::Real32 max_density = density_map.maxValueInCell( cell, volume );
                const kvs::Real32 pmax = max_density / n;

                size_t counter = 0;
                while ( counter < n )
                {
                    const kvs::Real32 density = sampler.sample();
                    const kvs::Real32 p = density / n;
                    const kvs::Real32 R = CellByCellSampling::RandomNumber();
                    if ( p > pmax * R )
                    {
                        const CellByCellSampling::Particle& p = sampler.accept();
                        const size_t particle_index = particle_index_counter++;
                        particles.push( particle_index, p );

                        counter++;
                    }
                }
            }
        }

        delete cell;
    }

    SuperClass::setCoords( particles.coords() );
    SuperClass::setColors( particles.colors() );
    SuperClass::setNormals( particles.normals() );
    SuperClass::setSize( 1.0f );
}

} // end of namespace kvs
