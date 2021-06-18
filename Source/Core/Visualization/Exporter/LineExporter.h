/*****************************************************************************/
/**
 *  @file   LineExporter.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/LineObject>
#include <kvs/KVSMLLineObject>
#include "ExporterBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Line exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class LineExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::LineExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return NULL; }
};

/*===========================================================================*/
/**
 *  @brief  Line exporter class as KVSMLLineObject format.
 */
/*===========================================================================*/
template <>
class LineExporter<kvs::KVSMLLineObject> :
        public kvs::ExporterBase,
        public kvs::KVSMLLineObject
{
public:
    LineExporter( const kvs::LineObject* object );
    kvs::KVSMLLineObject* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs
