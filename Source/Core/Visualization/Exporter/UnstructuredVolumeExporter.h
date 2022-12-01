/*****************************************************************************/
/**
 *  @file   UnstructuredVolumeExporter.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ObjectBase>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/AVSUcd>
#include <kvs/ExporterBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Unstructured volume exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class UnstructuredVolumeExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::UnstructuredVolumeExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return nullptr; }
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::KVSMLUnstructuredVolumeObject.
 */
/*===========================================================================*/
template <>
class UnstructuredVolumeExporter<kvs::KVSMLUnstructuredVolumeObject> :
        public kvs::ExporterBase,
        public kvs::KVSMLUnstructuredVolumeObject
{
public:
    UnstructuredVolumeExporter( const kvs::UnstructuredVolumeObject* object ) { this->exec( object ); }
    kvs::KVSMLUnstructuredVolumeObject* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::AVSUcd.
 */
/*===========================================================================*/
template <>
class UnstructuredVolumeExporter<kvs::AVSUcd> :
        public kvs::ExporterBase,
        public kvs::AVSUcd
{
public:
    UnstructuredVolumeExporter( const kvs::UnstructuredVolumeObject* object ) { this->exec( object ); }
    kvs::AVSUcd* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs
