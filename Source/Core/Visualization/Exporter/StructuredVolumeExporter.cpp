/*****************************************************************************/
/**
 *  @file   StructuredVolumeExporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeExporter.cpp 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "StructuredVolumeExporter.h"
#include <kvs/ObjectBase>
#include <kvs/VolumeObjectBase>
#include <kvs/StructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a KVSMLStructuredVolumeObject data from given object.
 *  @param  object [in] pointer to the structured volume object
 */
/*===========================================================================*/
StructuredVolumeExporter<kvs::KVSMLStructuredVolumeObject>::StructuredVolumeExporter(
    const kvs::StructuredVolumeObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Exports object to a KVSMLStructuredVolumeObject data.
 *  @param  object [in] pointer to the structured volume object
 */
/*===========================================================================*/
kvs::KVSMLStructuredVolumeObject* StructuredVolumeExporter<kvs::KVSMLStructuredVolumeObject>::exec(
    const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    // Cast to the structured volume object.
    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not structured volume object.");
        return NULL;
    }

    if ( volume->label() != "" ) { this->setLabel( volume->label() ); }
    if ( volume->unit() != "" ) { this->setUnit( volume->unit() ); }

    // Check the grid type of the given structured volume object.
    switch ( volume->gridType() )
    {
    case kvs::StructuredVolumeObject::UnknownGridType:
    {
        kvsMessageError("Unknown grid type.");
        break;
    }
    case kvs::StructuredVolumeObject::Uniform:
    {
        this->setGridType("uniform");
        break;
    }
/*
    case kvs::StructuredVolumeObject::Rectilinear:
        this->setGridType("rectilinear");
        break;
    case kvs::StructuredVolumeObject::Curvilinear:
        this->setGridType("curvilinear");
        break;
*/
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("'uniform' grid type is only supported.");
        break;
    }
    }

    this->setVeclen( volume->veclen() );
    this->setResolution( volume->resolution() );
    this->setValues( volume->values() );

    if ( volume->hasMinMaxValues() )
    {
        this->setMinValue( volume->minValue() );
        this->setMaxValue( volume->maxValue() );
    }

    return this;
}


/*===========================================================================*/
/**
 *  @brief  Constructs a AVSField data from given object.
 *  @param  object [in] pointer to the structured volume object
 */
/*===========================================================================*/
StructuredVolumeExporter<kvs::AVSField>::StructuredVolumeExporter(
    const kvs::StructuredVolumeObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Exports object to a AVSField data.
 *  @param  object [in] pointer to the structured volume object
 */
/*===========================================================================*/
kvs::AVSField* StructuredVolumeExporter<kvs::AVSField>::exec(
    const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    // Cast to the structured volume object.
    const kvs::StructuredVolumeObject* volume = kvs::StructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not structured volume object.");
        return NULL;
    }

    const std::type_info& type = volume->values().typeInfo()->type();
    if ( type == typeid( kvs::Int8 ) )
    {
        this->setBits( 8 );
        this->setSigned( true );
        this->setDataType( kvs::AVSField::Byte );
    }
    else if ( type == typeid( kvs::UInt8 ) )
    {
        this->setBits( 8 );
        this->setSigned( false );
        this->setDataType( kvs::AVSField::Byte );
    }
    else if ( type == typeid( kvs::Int16 ) )
    {
        this->setBits( 16 );
        this->setSigned( true );
        this->setDataType( kvs::AVSField::Short );
    }
    else if ( type == typeid( kvs::UInt16 ) )
    {
        this->setBits( 16 );
        this->setSigned( false );
        this->setDataType( kvs::AVSField::Short );
    }
    else if ( type == typeid( kvs::Int32 ) )
    {
        this->setBits( 32 );
        this->setSigned( true );
        this->setDataType( kvs::AVSField::Integer );
    }
    else if ( type == typeid( kvs::UInt32 ) )
    {
        this->setBits( 32 );
        this->setSigned( false );
        this->setDataType( kvs::AVSField::Integer );
    }
    else if ( type == typeid( kvs::Real32 ) )
    {
        this->setBits( 32 );
        this->setSigned( true );
        this->setDataType( kvs::AVSField::Float );
    }
    else if ( type == typeid( kvs::Real64 ) )
    {
        this->setBits( 64 );
        this->setSigned( true );
        this->setDataType( kvs::AVSField::Double );
    }
    else
    {
        kvsMessageError("Unsupported data type '%s'.", volume->values().typeInfo()->typeName() );
    }

    // Check the grid type of the given structured volume object.
    switch ( volume->gridType() )
    {
    case kvs::StructuredVolumeObject::Uniform:
    {
        this->setFieldType( kvs::AVSField::Uniform );
        this->setValues( volume->values() );
        break;
    }
/*
    case kvs::StructuredVolumeObject::Rectilinear:
        this->setFieldType( kvs::AVSField::Rectilinear );
        this->setValues( volume->values() );
        this->setCoords( volume->coords() );
        break;
    case kvs::StructuredVolumeObject::Curvilinear:
        this->setFieldType( kvs::AVSField::Irregular );
        this->setValues( volume->values() );
        this->setCoords( volume->coords() );
        break;
*/
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Unknown grid type.");
        break;
    }
    }

    this->setVeclen( volume->veclen() );
    this->setNSpace( 3 );
    this->setNDim( 3 );
    this->setDim( volume->resolution() );

    return this;
}

} // end of namespace kvs
