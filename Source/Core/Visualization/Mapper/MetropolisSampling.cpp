/****************************************************************************/
/**
 *  @file   MetropolisSampling.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MetropolisSampling.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MetropolisSampling.h"
#include <kvs/MersenneTwister>
#include <kvs/TrilinearInterpolator>
#include <kvs/IgnoreUnusedVariable>
#include <vector>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new MetropolisSampling class.
 */
/*==========================================================================*/
MetropolisSampling::MetropolisSampling():
    kvs::MapperBase(),
    kvs::PointObject()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new MetropolisSampling class.
 *  @param volume [in] pointer to the volume object
 *  @param nparticles [in] number of particles
 */
/*==========================================================================*/
MetropolisSampling::MetropolisSampling(
    const kvs::VolumeObjectBase* volume,
    const size_t                 nparticles ):
    kvs::MapperBase(),
    kvs::PointObject(),
    m_nparticles( nparticles )
{
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new MetropolisSampling class.
 *  @param volume [in] pointer to the volume object
 *  @param nparticles [in] number of generated particles
 *  @param transfer_function [in] transfer function
 */
/*==========================================================================*/
MetropolisSampling::MetropolisSampling(
    const kvs::VolumeObjectBase* volume,
    const size_t                 nparticles,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject(),
    m_nparticles( nparticles )
{
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Destroys the MetropolisSampling class.
 */
/*==========================================================================*/
MetropolisSampling::~MetropolisSampling()
{
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of generated particles.
 */
/*==========================================================================*/
size_t MetropolisSampling::nparticles() const
{
    return m_nparticles;
}

/*==========================================================================*/
/**
 *  @brief  Set the number of generated particles.
 *  @param  npoints [in] number of points
 */
/*==========================================================================*/
void MetropolisSampling::setNParticles( const size_t nparticles )
{
    m_nparticles = nparticles;
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return pointer to the point object
 */
/*===========================================================================*/
MetropolisSampling::SuperClass* MetropolisSampling::exec( const kvs::ObjectBase* object )
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

    const kvs::VolumeObjectBase::VolumeType volume_type = volume->volumeType();
    if ( volume_type == kvs::VolumeObjectBase::Structured )
    {
        this->mapping( reinterpret_cast<const kvs::StructuredVolumeObject*>( object ) );
    }
    else // volume_type == kvs::VolumeObjectBase::Unstructured
    {
        this->mapping( reinterpret_cast<const kvs::UnstructuredVolumeObject*>( object ) );
    }

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Mapping for the structure volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
void MetropolisSampling::mapping( const kvs::StructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Generate the particles.
    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::UInt8  ) ) this->generate_particles<kvs::UInt8>( volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->generate_particles<kvs::UInt16>( volume );
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
    }
}

/*==========================================================================*/
/**
 *  @brief  Mapping for the unstructure volume object.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
void MetropolisSampling::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    kvs::IgnoreUnusedVariable( volume );

    BaseClass::setSuccess( false );
    kvsMessageError("Not yet supported the metropolis method for the unstructured volume");
}

/*==========================================================================*/
/**
 *  @brief  Generate particles for the structured volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void MetropolisSampling::generate_particles( const kvs::StructuredVolumeObject* volume  )
{
    // Set the trilinear interpolator.
    kvs::TrilinearInterpolator interpolator( volume );

    // Alias.
    const kvs::Vector3ui r = volume->resolution() - kvs::Vector3ui::Constant(1);

    // Allocate memory for generated particles.
    kvs::ValueArray<kvs::Real32> coords( m_nparticles * 3 );
    kvs::ValueArray<kvs::UInt8> colors( m_nparticles * 3 );
    kvs::ValueArray<kvs::Real32> normals( m_nparticles * 3 );
/*
    SuperClass::m_coords.allocate( m_nparticles * 3 );
    SuperClass::m_colors.allocate( m_nparticles * 3 );
    SuperClass::m_normals.allocate( m_nparticles * 3 );
*/

    // Random number generator.
    kvs::MersenneTwister R;

    // Set a initial particle and a trial particle.
    kvs::Vector3f particle( static_cast<float>(R() * r.x()),
                            static_cast<float>(R() * r.y()),
                            static_cast<float>(R() * r.z()) );
    kvs::Vector3f trial_particle( 0.0f, 0.0f, 0.0f );

    // Attach the initial particle to the interpolator and get a rho value.
    interpolator.attachPoint( particle );
    size_t scalar = static_cast<size_t>( interpolator.template scalar<T>() );
    float  rho    = BaseClass::opacityMap()[ scalar ];
    float  trial_rho( 0.0f );

    // Point sampling process.
    size_t counter = 0;
    while( counter < m_nparticles )
    {
        trial_particle.set( static_cast<float>(R() * r.x()),
                            static_cast<float>(R() * r.y()),
                            static_cast<float>(R() * r.z()) );
        interpolator.attachPoint( trial_particle );

        scalar    = static_cast<size_t>( interpolator.template scalar<T>() );
        trial_rho = BaseClass::opacityMap()[ scalar ];

        const float ratio = trial_rho / rho;
        if( ratio >= 1.0f )
        {
            // Adopt the particle.
            const kvs::Vector3f gradient = interpolator.template gradient<T>();
//            this->adopt_particle( counter, trial_particle, scalar, gradient );
            const size_t index3 = counter * 3;
            coords[ index3 + 0 ]  = trial_particle.x();
            coords[ index3 + 1 ]  = trial_particle.y();
            coords[ index3 + 2 ]  = trial_particle.z();
            colors[ index3 + 0 ]  = BaseClass::colorMap()[ scalar ].r();
            colors[ index3 + 1 ]  = BaseClass::colorMap()[ scalar ].g();
            colors[ index3 + 2 ]  = BaseClass::colorMap()[ scalar ].b();
            normals[ index3 + 0 ] = gradient.x();
            normals[ index3 + 1 ] = gradient.y();
            normals[ index3 + 2 ] = gradient.z();

            // Update the particle.
            particle = trial_particle;
            rho      = trial_rho;

            counter++;
        }
        else
        {
            if( ratio >= R() )
            {
                // Adopt the particle.
                const kvs::Vector3f gradient = interpolator.template gradient<T>();
//                this->adopt_particle( counter, trial_particle, scalar, gradient );
                const size_t index3 = counter * 3;
                coords[ index3 + 0 ]  = trial_particle.x();
                coords[ index3 + 1 ]  = trial_particle.y();
                coords[ index3 + 2 ]  = trial_particle.z();
                colors[ index3 + 0 ]  = BaseClass::colorMap()[ scalar ].r();
                colors[ index3 + 1 ]  = BaseClass::colorMap()[ scalar ].g();
                colors[ index3 + 2 ]  = BaseClass::colorMap()[ scalar ].b();
                normals[ index3 + 0 ] = gradient.x();
                normals[ index3 + 1 ] = gradient.y();
                normals[ index3 + 2 ] = gradient.z();

                // Update the particle.
                particle = trial_particle;
                rho      = trial_rho;

                counter++;
            }
            else
            {
#ifdef DUPLICATION
                interpolator.attachPoint( particle );
                scalar = interpolator.template scalar<T>();
                const kvs::Vector3f gradient = interpolator.template gradient<T>();
//                this->adopt_particle( counter, particle, scalar, gradient );
                const size_t index3 = counter * 3;
                coords[ index3 + 0 ]  = particle.x();
                coords[ index3 + 1 ]  = particle.y();
                coords[ index3 + 2 ]  = particle.z();
                colors[ index3 + 0 ]  = BaseClass::colorMap()[ scalar ].r();
                colors[ index3 + 1 ]  = BaseClass::colorMap()[ scalar ].g();
                colors[ index3 + 2 ]  = BaseClass::colorMap()[ scalar ].b();
                normals[ index3 + 0 ] = gradient.x();
                normals[ index3 + 1 ] = gradient.y();
                normals[ index3 + 2 ] = gradient.z();

                counter++;
#else
                continue;
#endif
            }
        }
    }

    SuperClass::setCoords( coords );
    SuperClass::setColors( colors );
    SuperClass::setNormals( normals );
    SuperClass::setSize( 1.0f );
}

template
void MetropolisSampling::generate_particles<kvs::UInt8>( const kvs::StructuredVolumeObject* volume );

template
void MetropolisSampling::generate_particles<kvs::UInt16>( const kvs::StructuredVolumeObject* volume );

/*==========================================================================*/
/**
 *  @brief  Adopt the particle.
 *  @param  index [in] index of the particle
 *  @param  coord [in] coordinate of the particle
 *  @param  scalar [in] scalar value of the particle
 *  @param  gradient [in] gradient vector of the particle
 */
/*==========================================================================*/
/*
void MetropolisSampling::adopt_particle(
    const size_t         index,
    const kvs::Vector3f& coord,
    const size_t         scalar,
    const kvs::Vector3f& gradient )
{
    const size_t index3 = index * 3;
    m_coords[ index3 + 0 ]  = coord.x();
    m_coords[ index3 + 1 ]  = coord.y();
    m_coords[ index3 + 2 ]  = coord.z();
    m_colors[ index3 + 0 ]  = BaseClass::colorMap()[ scalar ].r();
    m_colors[ index3 + 1 ]  = BaseClass::colorMap()[ scalar ].g();
    m_colors[ index3 + 2 ]  = BaseClass::colorMap()[ scalar ].b();
    m_normals[ index3 + 0 ] = gradient.x();
    m_normals[ index3 + 1 ] = gradient.y();
    m_normals[ index3 + 2 ] = gradient.z();
}
*/

} // end of namespace kvs
