#include "StructuredExtractScalar.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StructuredExtractScalar class.
 *  @param  volume [in] pointer to the volume object
 *  @param  offset [in] offset for extracting scalar from the volume object
 */
/*===========================================================================*/
StructuredExtractScalar::StructuredExtractScalar(
    const kvs::StructuredVolumeObject* volume,
    const size_t offset ):
    m_offset( offset )
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Returns the extracted scalar volume object.
 *  @param  object [in] input volume object
 *  @return extracted scalar volume object
 */
/*===========================================================================*/
StructuredExtractScalar::SuperClass* StructuredExtractScalar::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not supported.");
        return NULL;
    }

    const std::type_info& type = volume->values().typeInfo()->type();
    if (      type == typeid( kvs::Int8   ) ) this->slice_scalar<kvs::Int8>(   volume );
    else if ( type == typeid( kvs::Int16  ) ) this->slice_scalar<kvs::Int16>(  volume );
    else if ( type == typeid( kvs::Int32  ) ) this->slice_scalar<kvs::Int32>(  volume );
    else if ( type == typeid( kvs::Int64  ) ) this->slice_scalar<kvs::Int64>(  volume );
    else if ( type == typeid( kvs::UInt8  ) ) this->slice_scalar<kvs::UInt8>(  volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->slice_scalar<kvs::UInt16>( volume );
    else if ( type == typeid( kvs::UInt32 ) ) this->slice_scalar<kvs::UInt32>( volume );
    else if ( type == typeid( kvs::UInt64 ) ) this->slice_scalar<kvs::UInt64>( volume );
    else if ( type == typeid( kvs::Real32 ) ) this->slice_scalar<kvs::Real32>( volume );
    else if ( type == typeid( kvs::Real64 ) ) this->slice_scalar<kvs::Real64>( volume );
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
        return NULL;
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Extracts scalar volume with the specified offset.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
void StructuredExtractScalar::slice_scalar( const kvs::StructuredVolumeObject* volume )
{
    const size_t veclen = volume->veclen();
    const size_t nnodes = volume->numberOfNodes();
    const kvs::ValueArray<T> vectors = volume->values().asValueArray<T>();
    kvs::ValueArray<T> scalars( nnodes );
    for ( size_t i = 0; i < nnodes; i++ ) { scalars[i] = vectors[ veclen * i + m_offset ]; }

    SuperClass::setGridTypeToUniform();
    SuperClass::setVeclen( 1 );
    SuperClass::setResolution( volume->resolution() );
    SuperClass::setValues( scalars );
    SuperClass::updateMinMaxValues();
    SuperClass::updateMinMaxCoords();
}

} // end of namespace kvs
