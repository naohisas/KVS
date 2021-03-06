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
 *  @brief  Constructs a new PointExporter class for KVMLObjectPoint format.
 *  @param  object [in] pointer to the input point object
 */
/*===========================================================================*/
PointExporter<kvs::KVSMLPointObject>::PointExporter( const kvs::PointObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Executes the export process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the KVSMLPointObject format
 */
/*===========================================================================*/
kvs::KVSMLPointObject* PointExporter<kvs::KVSMLPointObject>::exec( const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::PointObject* point = kvs::PointObject::DownCast( object );
    if ( !point )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not point object.");
        return NULL;
    }

    this->setCoords( point->coords() );
    this->setColors( point->colors() );
    this->setNormals( point->normals() );
    this->setSizes( point->sizes() );

    return this;
}

} // end of namespace kvs
