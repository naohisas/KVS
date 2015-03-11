#include "UnstructuredQCriterion.h"
#include "InverseDistanceWeighting.h"
#include <kvs/PrismaticCell>
#include <map>
#include <kvs/Type>
#include <kvs/UnstructuredVolumeObject>


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

/*===========================================================================*/
/**
 *  @brief  Returns a tensor as a 3x3 matrix.
 *  @param  volume [in] pointer to an input volume object
 *  @param  index [in] index of cell
 *  @return tensor value specified by the given index
 */
/*===========================================================================*/
kvs::Mat3 Tensor( const kvs::UnstructuredVolumeObject* volume, const size_t index )
{
    const size_t veclen = volume->veclen();
    switch ( volume->values().typeID() )
    {
    case kvs::Type::TypeInt8:
    {
        const kvs::Int8* t = volume->values().asValueArray<kvs::Int8>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeInt16:
    {
        const kvs::Int16* t = volume->values().asValueArray<kvs::Int16>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeInt32:
    {
        const kvs::Int32* t = volume->values().asValueArray<kvs::Int32>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeInt64:
    {
        const kvs::Int64* t = volume->values().asValueArray<kvs::Int64>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeUInt8:
    {
        const kvs::UInt8* t = volume->values().asValueArray<kvs::UInt8>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeUInt16:
    {
        const kvs::UInt16* t = volume->values().asValueArray<kvs::UInt16>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeUInt32:
    {
        const kvs::UInt32* t = volume->values().asValueArray<kvs::UInt32>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeUInt64:
    {
        const kvs::UInt64* t = volume->values().asValueArray<kvs::UInt64>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeReal32:
    {
        const kvs::Real32* t = volume->values().asValueArray<kvs::Real32>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    case kvs::Type::TypeReal64:
    {
        const kvs::Real64* t = volume->values().asValueArray<kvs::Real64>().data() + index * veclen;
        return kvs::Mat3( t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8] );
    }
    default: break;
    }

    return kvs::Mat3::Zero();
}

/*===========================================================================*/
/**
 *  @brief  Returns a q-value calculated from the input tensor value.
 *  @param  T [in] tensor value
 *  @return q-value
 */
/*===========================================================================*/
kvs::Real32 Q( const kvs::Mat3& T )
{
    const kvs::Real32 t00 = T[0][0];
    const kvs::Real32 t11 = T[1][1];
    const kvs::Real32 t22 = T[2][2];
    const kvs::Real32 t01 = T[0][1];
    const kvs::Real32 t10 = T[1][0];
    const kvs::Real32 t12 = T[1][2];
    const kvs::Real32 t21 = T[2][1];
    const kvs::Real32 t02 = T[0][2];
    const kvs::Real32 t20 = T[2][0];
    return t00 * t11 + t11 * t22 + t22 * t00 - t01 * t10 - t12 * t21 - t02 * t20;
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs an UnstructuredQCriterion class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
UnstructuredQCriterion::UnstructuredQCriterion( const kvs::UnstructuredVolumeObject* volume ):
    kvs::FilterBase(),
    kvs::UnstructuredVolumeObject()
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes Q-criterion calculation.
 *  @param  object [in] pointer to the input volume object
 *  @return Calculated q-criterion volume object
 */
/*===========================================================================*/
UnstructuredQCriterion::SuperClass* UnstructuredQCriterion::exec( const kvs::ObjectBase* object )
{
    const kvs::UnstructuredVolumeObject* volume = ::Cast( object );
    if ( !volume ) { return NULL; }

    if ( volume->veclen() == 3 )
    {
        this->qvalues_from_vectors( volume );
    }
    else if ( volume->veclen() == 9 )
    {
        this->qvalues_from_tensors( volume );
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Calculates q-values from vector values.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void UnstructuredQCriterion::qvalues_from_vectors( const kvs::UnstructuredVolumeObject* volume )
{
    const kvs::UInt32* connections = volume->connections().data();
    const size_t ncells = volume->numberOfCells();
    const size_t nnodes = volume->numberOfNodes();

    kvs::InverseDistanceWeighting<kvs::Real32> idw( nnodes );
    kvs::PrismaticCell cell( volume );
    const kvs::Vec3 center = cell.localCenter();
    for ( size_t i = 0; i < ncells; i++ )
    {
        cell.bindCell( i );
        cell.setLocalPoint( center );
        const kvs::Mat3 T = cell.gradientTensor();
        const kvs::Real32 Q = ::Q( T );

        const kvs::Vec3 c = cell.center();
        for ( size_t j = 0; j < cell.numberOfCellNodes(); j++ )
        {
            const kvs::UInt32 id = *( connections++ );
            const kvs::Real32 d = ( cell.coord(j) - c ).length();
            idw.insert( id, Q, d );
        }
    }

    SuperClass::shallowCopy( *volume );
    SuperClass::setVeclen( 1 );
    SuperClass::setValues( kvs::AnyValueArray( idw.serialize() ) );
    SuperClass::updateMinMaxValues();
}

/*===========================================================================*/
/**
 *  @brief  Calculates q-values from tensor values.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void UnstructuredQCriterion::qvalues_from_tensors( const kvs::UnstructuredVolumeObject* volume )
{
    const size_t nnodes = volume->numberOfNodes();

    kvs::ValueArray<kvs::Real32> values( nnodes );
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const kvs::Mat3 T = ::Tensor( volume, i );
        values[i] = ::Q( T );
    }

    SuperClass::shallowCopy( *volume );
    SuperClass::setVeclen( 1 );
    SuperClass::setValues( kvs::AnyValueArray( values ) );
    SuperClass::updateMinMaxValues();
}

} // end of namespace kvs
