/*****************************************************************************/
/**
 *  @file   PointExporter.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PointExporter.h"
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Executes the export process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the KVSMLPointObject format
 */
/*===========================================================================*/
kvs::KVSMLPointObject*
PointExporter<kvs::KVSMLPointObject>::exec( const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return nullptr;
    }

    const auto* point = kvs::PointObject::DownCast( object );
    if ( !point )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not point object.");
        return nullptr;
    }

    this->setCoords( point->coords() );
    this->setColors( point->colors() );
    this->setNormals( point->normals() );
    this->setSizes( point->sizes() );

    return this;
}

} // end of namespace kvs
