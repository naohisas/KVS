/****************************************************************************/
/**
 *  @file   MapperBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MapperBase.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MapperBase.h"
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new empty MapperBase.
 */
/*==========================================================================*/
MapperBase::MapperBase()
    : m_transfer_function()
    , m_volume( 0 )
    , m_is_success( false )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new MapperBase.
 */
/*==========================================================================*/
MapperBase::MapperBase( const TransferFunction& transfer_function )
    : m_transfer_function( transfer_function )
    , m_volume( 0 )
    , m_is_success( false )
{
}

/*==========================================================================*/
/**
 *  @brief  Destroys the MapperBase.
 */
/*==========================================================================*/
MapperBase::~MapperBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Sets a transfer function.
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
void MapperBase::setTransferFunction( const kvs::TransferFunction& transfer_function )
{
    m_transfer_function = transfer_function;
}

/*===========================================================================*/
/**
 *  @brief  Sets a color map.
 *  @param  color_map [in] color map
 */
/*===========================================================================*/
void MapperBase::setColorMap( const kvs::ColorMap& color_map )
{
    m_transfer_function.setColorMap( color_map );
}

/*===========================================================================*/
/**
 *  @brief  Sets a opacity map.
 *  @param  opacity_map [in] opacity map
 */
/*===========================================================================*/
void MapperBase::setOpacityMap( const kvs::OpacityMap& opacity_map )
{
    m_transfer_function.setOpacityMap( opacity_map );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the volume object.
 *  @return pointer to the volume object
 */
/*===========================================================================*/
const kvs::VolumeObjectBase* MapperBase::volume() const
{
    return m_volume;
}

/*===========================================================================*/
/**
 *  @brief  Returns the transfer function.
 */
/*===========================================================================*/
const kvs::TransferFunction& MapperBase::transferFunction() const
{
    return m_transfer_function;
}

/*===========================================================================*/
/**
 *  @brief  Returns the color map.
 */
/*===========================================================================*/
const kvs::ColorMap& MapperBase::colorMap() const
{
    return m_transfer_function.colorMap();
}

/*===========================================================================*/
/**
 *  @brief  Returns the opacity map.
 */
/*===========================================================================*/
const kvs::OpacityMap& MapperBase::opacityMap() const
{
    return m_transfer_function.opacityMap();
}

/*==========================================================================*/
/**
 *  @brief  Returns true if the mapping is success; otherwise returns false.
 *  @return Whether the mapping is success or not.
 */
/*==========================================================================*/
bool MapperBase::isSuccess() const
{
    return m_is_success;
}

/*==========================================================================*/
/**
 *  @brief  Returns true if the mapping is failure; otherwise returns false.
 *  @return Whether the mapping is failure or not.
 */
/*==========================================================================*/
bool MapperBase::isFailure() const
{
    return !m_is_success;
}

/*===========================================================================*/
/**
 *  @brief  Attaches a voluem object.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void MapperBase::attachVolume( const kvs::VolumeObjectBase* volume )
{
    m_volume = volume;
}

/*===========================================================================*/
/**
 *  @brief  Sets a data range of the transfer function.
 *  @param  volume [in] pointer to volume object
 */
/*===========================================================================*/
void MapperBase::setRange( const kvs::VolumeObjectBase* volume )
{
    if ( !volume->hasMinMaxValues() ) volume->updateMinMaxValues();
    const std::type_info& type = volume->values().typeInfo()->type();
    if ( type == typeid( kvs::Int8 ) )
    {
        if ( !m_transfer_function.hasRange() ) m_transfer_function.setRange( -128, 127 );
    }
    else if ( type == typeid( kvs::UInt8  ) )
    {
        if ( !m_transfer_function.hasRange() ) m_transfer_function.setRange( 0, 255 );
    }
    else
    {
        const float min_value = static_cast<float>( volume->minValue() );
        const float max_value = static_cast<float>( volume->maxValue() );
        if ( !m_transfer_function.hasRange() ) m_transfer_function.setRange( min_value, max_value );
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculates the min/max coordinate values.
 *  @param  volume [in] pointer to the input volume object
 *  @param  object [in] pointer to the object (The calculated coordinates are set to this object)
 */
/*===========================================================================*/
void MapperBase::setMinMaxCoords( const kvs::VolumeObjectBase* volume, kvs::ObjectBase* object )
{
    if ( !volume->hasMinMaxObjectCoords() )
    {
        switch ( volume->volumeType() )
        {
        case kvs::VolumeObjectBase::Structured:
        {
            // WARNING: remove constness, but safe in this case.
            kvs::VolumeObjectBase* b = const_cast<kvs::VolumeObjectBase*>( volume );
            reinterpret_cast<kvs::StructuredVolumeObject*>( b )->updateMinMaxCoords();
            break;
        }
        case kvs::VolumeObjectBase::Unstructured:
        {
            // WARNING: remove constness, but safe in this case.
            kvs::VolumeObjectBase* b = const_cast<kvs::VolumeObjectBase*>( volume );
            reinterpret_cast<kvs::UnstructuredVolumeObject*>( b )->updateMinMaxCoords();
            break;
        }
        default: break;
        }
    }

    const kvs::Vector3f min_obj_coord( m_volume->minObjectCoord() );
    const kvs::Vector3f max_obj_coord( m_volume->maxObjectCoord() );
    const kvs::Vector3f min_ext_coord( m_volume->minExternalCoord() );
    const kvs::Vector3f max_ext_coord( m_volume->maxExternalCoord() );
    object->setMinMaxObjectCoords( min_obj_coord, max_obj_coord );
    object->setMinMaxExternalCoords( min_ext_coord, max_ext_coord );
}

/*===========================================================================*/
/**
 *  @brief  Sets a status of the mapping process.
 *  @param  success [in] status of mapping process
 */
/*===========================================================================*/
void MapperBase::setSuccess( const bool success )
{
    m_is_success = success;
}

} // end of namespace kvs
