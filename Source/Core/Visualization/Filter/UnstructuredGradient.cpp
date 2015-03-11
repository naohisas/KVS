/*****************************************************************************/
/**
 *  @file   UnstructuredGradient.cpp
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
/*****************************************************************************/
#include "UnstructuredGradient.h"
#include "InverseDistanceWeighting.h"
#include <kvs/UnstructuredVolumeObject>
#include <kvs/PrismaticCell>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns casted pointer to an unstructured volume object.
 *  @param  object [in] pointer to an input object
 *  @return pointer to an unstructured volume object
 */
/*===========================================================================*/
const kvs::UnstructuredVolumeObject* Cast( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        kvsMessageError("Input object is not a volume data.");
        return NULL;
    }

    if ( volume->volumeType() != kvs::VolumeObjectBase::Unstructured )
    {
        kvsMessageError("Input object is not an unstructured volume object.");
        return NULL;
    }

    return kvs::UnstructuredVolumeObject::DownCast( volume );
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs an UnstructuredGradient class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
UnstructuredGradient::UnstructuredGradient( const kvs::UnstructuredVolumeObject* volume ):
    kvs::FilterBase(),
    kvs::UnstructuredVolumeObject()
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes gradient calculation.
 *  @param  object [in] pointer to the input volume object
 *  @return Calculated gradient volume object
 */
/*===========================================================================*/
UnstructuredGradient::SuperClass* UnstructuredGradient::exec( const kvs::ObjectBase* object )
{
    const kvs::UnstructuredVolumeObject* volume = ::Cast( object );
    if ( !volume ) { return NULL; }

    if ( volume->veclen() == 1 )
    {
        this->scalar_gradient( volume );
    }
    else if ( volume->veclen() == 3 )
    {
        this->vector_gradient( volume );
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Calculates gradient vectors from scalar values.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void UnstructuredGradient::scalar_gradient( const kvs::UnstructuredVolumeObject* volume )
{
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();
    const kvs::UInt32* connections = volume->connections().data();

    kvs::InverseDistanceWeighting<kvs::Vec3> idw( nnodes );
    kvs::PrismaticCell cell( volume );
    const kvs::Vec3 center = cell.localCenter();
    for ( size_t i = 0; i < ncells; i++ )
    {
        cell.bindCell( i );
        cell.setLocalPoint( center );
        const kvs::Vec3 V = cell.gradientVector();

        const kvs::Vec3 c = cell.center();
        for ( size_t j = 0; j < cell.numberOfCellNodes(); j++ )
        {
            const kvs::UInt32 id = *( connections++ );
            const kvs::Real32 d = ( cell.coord(j) - c ).length();
            idw.insert( id, V, d );
        }
    }

    SuperClass::shallowCopy( *volume );
    SuperClass::setVeclen( 3 );
    SuperClass::setValues( kvs::AnyValueArray( idw.serialize() ) );
    SuperClass::updateMinMaxValues();
}

/*===========================================================================*/
/**
 *  @brief  Calculates gradient tensors from vector values.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void UnstructuredGradient::vector_gradient( const kvs::UnstructuredVolumeObject* volume )
{
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();
    const kvs::UInt32* connections = volume->connections().data();

    kvs::InverseDistanceWeighting<kvs::Mat3> idw( nnodes );
    kvs::PrismaticCell cell( volume );
    const kvs::Vec3 center = cell.localCenter();
    for ( size_t i = 0; i < ncells; i++ )
    {
        cell.bindCell( i );
        cell.setLocalPoint( center );
        const kvs::Mat3 T = cell.gradientTensor();

        const kvs::Vec3 c = cell.center();
        for ( size_t j = 0; j < cell.numberOfCellNodes(); j++ )
        {
            const kvs::UInt32 id = *( connections++ );
            const kvs::Real32 d = ( cell.coord(j) - c ).length();
            idw.insert( id, T, d );
        }
    }

    SuperClass::shallowCopy( *volume );
    SuperClass::setVeclen( 9 );
    SuperClass::setValues( kvs::AnyValueArray( idw.serialize() ) );
    SuperClass::updateMinMaxValues();
}

} // end of namespace kvs
