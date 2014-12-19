/****************************************************************************/
/**
 *  @file   HitAndMissSampling.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HitAndMissSampling.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "HitAndMissSampling.h"
#include <kvs/MersenneTwister>
#include <kvs/IgnoreUnusedVariable>
#include <vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new HitAndMissSampling class.
 */
/*===========================================================================*/
HitAndMissSampling::HitAndMissSampling():
    kvs::MapperBase(),
    kvs::PointObject()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new HitAndMissSampling class.
 *  @param  volume [in] pointer to the volume object
 */
/*==========================================================================*/
HitAndMissSampling::HitAndMissSampling( const kvs::VolumeObjectBase* volume ):
    kvs::MapperBase(),
    kvs::PointObject()
{
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new HitAndMissSampling class.
 *  @param  volume [in] pointer to the volume object
 *  @param  transfer_function [in] transfer function
 */
/*==========================================================================*/
HitAndMissSampling::HitAndMissSampling(
    const kvs::VolumeObjectBase* volume,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject()
{
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Destroys the HitAndMissSampling class.
 */
/*==========================================================================*/
HitAndMissSampling::~HitAndMissSampling()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return pointer to the point object
 */
/*===========================================================================*/
HitAndMissSampling::SuperClass* HitAndMissSampling::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return( NULL );
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return( NULL );
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

    return( this );
}

/*==========================================================================*/
/**
 *  @brief  Mapping for the structured volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
void HitAndMissSampling::mapping( const kvs::StructuredVolumeObject* volume )
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
 *  @brief  Mapping for the unstructured volume object.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
void HitAndMissSampling::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Generate the particles.
    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) this->generate_particles<kvs::Int8>( volume );
    else if ( type == typeid( kvs::Int16  ) ) this->generate_particles<kvs::Int16>( volume );
    else if ( type == typeid( kvs::Int32  ) ) this->generate_particles<kvs::Int32>( volume );
    else if ( type == typeid( kvs::Int64  ) ) this->generate_particles<kvs::Int64>( volume );
    else if ( type == typeid( kvs::UInt8  ) ) this->generate_particles<kvs::UInt8>( volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->generate_particles<kvs::UInt16>( volume );
    else if ( type == typeid( kvs::UInt32 ) ) this->generate_particles<kvs::UInt32>( volume );
    else if ( type == typeid( kvs::UInt64 ) ) this->generate_particles<kvs::UInt64>( volume );
    else if ( type == typeid( kvs::Real32 ) ) this->generate_particles<kvs::Real32>( volume );
    else if ( type == typeid( kvs::Real64 ) ) this->generate_particles<kvs::Real64>( volume );
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
    }
}

/*==========================================================================*/
/**
 *  @brief  Generate particles for the structured volume object.
 *  @param  volume [in] pointer to the structured volume object
 */
/*==========================================================================*/
template <typename T>
void HitAndMissSampling::generate_particles( const kvs::StructuredVolumeObject* volume  )
{
    // Set the geometry arrays.
    const size_t max_nparticles = volume->numberOfNodes();
    std::vector<kvs::Real32> coords;  coords.reserve( max_nparticles * 3 );
    std::vector<kvs::UInt8>  colors;  colors.reserve( max_nparticles * 3 );
    std::vector<kvs::Real32> normals; normals.reserve( max_nparticles * 3 );

    // Aliases.
    const kvs::Vector3ui resolution = volume->resolution();
    const size_t line_size  = volume->numberOfNodesPerLine();
    const size_t slice_size = volume->numberOfNodesPerSlice();
    const T* values = reinterpret_cast<const T*>( volume->values().data() );

    kvs::MersenneTwister R; // Random number generator
    size_t index = 0;     // index of voxel
    for ( size_t k = 0; k < resolution.z(); k++ )
    {
        for ( size_t j = 0; j < resolution.y(); j++ )
        {
            for ( size_t i = 0; i < resolution.x(); i++, index++ )
            {
                // Rejection.
                const size_t voxel_value = values[ index ];
                if( R() < BaseClass::opacityMap()[ voxel_value ] )
                {
                    // Set coordinate value.
                    coords.push_back( static_cast<kvs::Real32>(i) );
                    coords.push_back( static_cast<kvs::Real32>(j) );
                    coords.push_back( static_cast<kvs::Real32>(k) );

                    // Set color value.
                    colors.push_back( BaseClass::colorMap()[ voxel_value ].r() );
                    colors.push_back( BaseClass::colorMap()[ voxel_value ].g() );
                    colors.push_back( BaseClass::colorMap()[ voxel_value ].b() );

                    // Calculate a normal vector at the node(i,j,k).
                    kvs::Vector3ui front( index, index, index ); // front index
                    kvs::Vector3ui back( index, index, index );  // back index

                    if(      i == 0                  ) front.x() += 1;
                    else if( i == resolution.x() - 1 ) back.x()  -= 1;
                    else{ front.x() += 1; back.x() -= 1; }

                    if(      j == 0                  ) front.y() += line_size;
                    else if( j == resolution.y() - 1 ) back.y()  -= line_size;
                    else{ front.y() += line_size; back.y() -= line_size; }

                    if(      k == 0                  ) front.z() += slice_size;
                    else if( k == resolution.z() - 1 ) back.z()  -= slice_size;
                    else{ front.z() += slice_size; back.z() -= slice_size; }

                    // Set normal vector.
                    normals.push_back( static_cast<kvs::Real32>( values[ front.x() ] - values[ back.x() ] ) );
                    normals.push_back( static_cast<kvs::Real32>( values[ front.y() ] - values[ back.y() ] ) );
                    normals.push_back( static_cast<kvs::Real32>( values[ front.z() ] - values[ back.z() ] ) );
                }
            } // end of i-loop
        } // end of j-loop
    } // end of k-loop

    SuperClass::setCoords( kvs::ValueArray<kvs::Real32>( coords ) );
    SuperClass::setColors( kvs::ValueArray<kvs::UInt8>( colors ) );
    SuperClass::setNormals( kvs::ValueArray<kvs::Real32>( normals ) );
    SuperClass::setSize( 1.0f );
}

template void HitAndMissSampling::generate_particles<kvs::UInt8>( const kvs::StructuredVolumeObject* volume );
template void HitAndMissSampling::generate_particles<kvs::UInt16>( const kvs::StructuredVolumeObject* volume );

/*==========================================================================*/
/**
 *  Generate particles for the unstructured volume object.
 *  @param volume [in] pointer to the unstructured volume object
 */
/*==========================================================================*/
template <typename T>
void HitAndMissSampling::generate_particles( const kvs::UnstructuredVolumeObject* volume  )
{
    kvs::IgnoreUnusedVariable( volume );

    BaseClass::setSuccess( false );
    kvsMessageError("Not yet implemented the hit-and-miss method for the unstructured volume.");
}

} // end of namespace kvs
