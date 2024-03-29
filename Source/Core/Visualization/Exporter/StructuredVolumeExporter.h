/*****************************************************************************/
/**
 *  @file   StructuredVolumeExporter.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/AVSField>
#include <kvs/ExporterBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  Structured volume exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class StructuredVolumeExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::StructuredVolumeExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return nullptr; }
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::KVSMLStructuredVolumeObject.
 */
/*===========================================================================*/
template <>
class StructuredVolumeExporter<kvs::KVSMLStructuredVolumeObject>:
        public kvs::ExporterBase,
        public kvs::KVSMLStructuredVolumeObject
{
public:
    StructuredVolumeExporter( const kvs::StructuredVolumeObject* object ) { this->exec( object ); }
    kvs::KVSMLStructuredVolumeObject* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::AVSField.
 */
/*===========================================================================*/
template <>
class StructuredVolumeExporter<kvs::AVSField>:
        public kvs::ExporterBase,
        public kvs::AVSField
{
public:
    StructuredVolumeExporter( const kvs::StructuredVolumeObject* object ) { this->exec( object ); }
    kvs::AVSField* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs
