/*****************************************************************************/
/**
 *  @file   PointExporter.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/PointObject>
#include <kvs/KVSMLPointObject>
#include "ExporterBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Point exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class PointExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::PointExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return NULL; }
};

/*===========================================================================*/
/**
 *  @brief  Point exporter class as KVSMLPointObject format.
 */
/*===========================================================================*/
template <>
class PointExporter<kvs::KVSMLPointObject> :
        public kvs::ExporterBase,
        public kvs::KVSMLPointObject
{
public:
    PointExporter( const kvs::PointObject* object );
    kvs::KVSMLPointObject* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs
