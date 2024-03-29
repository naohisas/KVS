/*****************************************************************************/
/**
 *  @file   LineExporter.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "LineExporter.h"
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Executes the export process.
 *  @param  object [in] pointer to the input object
 *  @return pointer to the KVSMLLineObject format
 */
/*===========================================================================*/
kvs::KVSMLLineObject*
LineExporter<kvs::KVSMLLineObject>::exec( const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return nullptr;
    }

    const auto* line = kvs::LineObject::DownCast( object );
    if ( !line )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not line object.");
        return nullptr;
    }

    switch ( line->lineType() )
    {
    case kvs::LineObject::Strip: this->setLineType( "strip" ); break;
    case kvs::LineObject::Uniline: this->setLineType( "uniline" ); break;
    case kvs::LineObject::Polyline: this->setLineType( "polyline" ); break;
    case kvs::LineObject::Segment:this->setLineType( "segment" ); break;
    default: break;
    }

    switch ( line->colorType() )
    {
    case kvs::LineObject::VertexColor: this->setColorType( "vertex" ); break;
    case kvs::LineObject::LineColor: this->setColorType( "line" ); break;
    default: break;
    }

    this->setCoords( line->coords() );
    this->setColors( line->colors() );
    this->setConnections( line->connections() );
    this->setSizes( line->sizes() );

    return this;
}

} // end of namespace kvs
