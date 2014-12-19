/*****************************************************************************/
/**
 *  @file   StructuredVectorToScalar.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVectorToScalar.cpp 1403 2012-12-07 07:35:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "StructuredVectorToScalar.h"
#include <kvs/Math>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StructuredVectorToScalar class.
 */
/*===========================================================================*/
StructuredVectorToScalar::StructuredVectorToScalar()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new StructuredVectorToScalar class.
 *  @param  volume [in] pointer to the structured volume
 */
/*===========================================================================*/
StructuredVectorToScalar::StructuredVectorToScalar( const kvs::StructuredVolumeObject* volume )
{
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the StructuredVolumeObject class.
 */
/*===========================================================================*/
StructuredVectorToScalar::~StructuredVectorToScalar()
{
}

/*===========================================================================*/
/**
 *  @brief  Main routine.
 *  @param  object [in] pointer to the object
 *  @return pointer to the converted structured volume object
 */
/*===========================================================================*/
StructuredVectorToScalar::SuperClass* StructuredVectorToScalar::exec( const kvs::ObjectBase* object )
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
    if (      type == typeid( kvs::Int8   ) ) this->calculate_magnitude<kvs::Int8>(   volume );
    else if ( type == typeid( kvs::Int16  ) ) this->calculate_magnitude<kvs::Int16>(  volume );
    else if ( type == typeid( kvs::Int32  ) ) this->calculate_magnitude<kvs::Int32>(  volume );
    else if ( type == typeid( kvs::Int64  ) ) this->calculate_magnitude<kvs::Int64>(  volume );
    else if ( type == typeid( kvs::UInt8  ) ) this->calculate_magnitude<kvs::UInt8>(  volume );
    else if ( type == typeid( kvs::UInt16 ) ) this->calculate_magnitude<kvs::UInt16>( volume );
    else if ( type == typeid( kvs::UInt32 ) ) this->calculate_magnitude<kvs::UInt32>( volume );
    else if ( type == typeid( kvs::UInt64 ) ) this->calculate_magnitude<kvs::UInt64>( volume );
    else if ( type == typeid( kvs::Real32 ) ) this->calculate_magnitude<kvs::Real32>( volume );
    else if ( type == typeid( kvs::Real64 ) ) this->calculate_magnitude<kvs::Real64>( volume );
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
 *  @brief  Calculates magnitude of the vector value
 *  @param  volume [in] pointer to the structured volume object
 */
/*===========================================================================*/
template <typename T>
void StructuredVectorToScalar::calculate_magnitude( const kvs::StructuredVolumeObject* volume )
{
    const size_t veclen = volume->veclen();
    const size_t nnodes = volume->numberOfNodes();

    kvs::ValueArray<kvs::Real32> values( nnodes );
    kvs::Real32* dst = values.data();
    const T* src = static_cast<const T*>( volume->values().data() );

    for ( size_t i = 0; i < nnodes; i++ )
    {
        kvs::Real32 magnitude = 0;
        for ( size_t j = 0; j < veclen; j++ )
        {
            magnitude += kvs::Math::Square( static_cast<kvs::Real32>( *(src++) ) );
        }

        *(dst++) = std::sqrt( magnitude );
    }

    SuperClass::setGridType( volume->gridType() );
    SuperClass::setVeclen( 1 );
    SuperClass::setResolution( volume->resolution() );
    SuperClass::setValues( kvs::AnyValueArray( values ) );
    SuperClass::updateMinMaxValues();
    SuperClass::updateMinMaxCoords();
}

} // end of namespace kvs
