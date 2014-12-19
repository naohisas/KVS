/*****************************************************************************/
/**
 *  @file   StructuredVolumeExporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: StructuredVolumeExporter.h 1794 2014-08-01 07:45:27Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__STRUCTURED_VOLUME_EXPORTER_H_INCLUDE
#define KVS__STRUCTURED_VOLUME_EXPORTER_H_INCLUDE

#include <kvs/ObjectBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/KVSMLObjectStructuredVolume>
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
    FileFormatType* exec( const kvs::ObjectBase* ) { return NULL; }
};

/*===========================================================================*/
/**
 *  @brief  Specialized structured volume exporter class for kvs::KVSMLObjectStructuredVolume.
 */
/*===========================================================================*/
template <>
class StructuredVolumeExporter<kvs::KVSMLObjectStructuredVolume>:
        public kvs::ExporterBase,
        public kvs::KVSMLObjectStructuredVolume
{
public:
    StructuredVolumeExporter( const kvs::StructuredVolumeObject* object );
    kvs::KVSMLObjectStructuredVolume* exec( const kvs::ObjectBase* object );
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
    StructuredVolumeExporter( const kvs::StructuredVolumeObject* object );
    kvs::AVSField* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs

#endif // KVS__STRUCTURED_VOLUME_EXPORTER_H_INCLUDE
