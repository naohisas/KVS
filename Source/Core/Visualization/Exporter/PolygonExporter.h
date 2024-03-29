/*****************************************************************************/
/**
 *  @file   PolygonExporter.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/PolygonObject>
#include <kvs/KVSMLPolygonObject>
#include <kvs/Stl>
#include <kvs/Ply>
#include "ExporterBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class.
 */
/*===========================================================================*/
template <typename FileFormatType>
class PolygonExporter : public kvs::ExporterBase, public FileFormatType
{
    kvsModule( kvs::PolygonExporter<FileFormatType>, Exporter );
    kvsModuleBaseClass( kvs::ExporterBase );

public:
    FileFormatType* exec( const kvs::ObjectBase* ) { return nullptr; }
};

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class as KVSMLPointObject format.
 */
/*===========================================================================*/
template <>
class PolygonExporter<kvs::KVSMLPolygonObject> :
        public kvs::ExporterBase,
        public kvs::KVSMLPolygonObject
{
public:
    PolygonExporter( const kvs::PolygonObject* object ) { this->exec( object ); }
    kvs::KVSMLPolygonObject* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class as STL format.
 */
/*===========================================================================*/
template <>
class PolygonExporter<kvs::Stl> :
        public kvs::ExporterBase,
        public kvs::Stl
{
public:
    PolygonExporter( const kvs::PolygonObject* object ) { this->exec( object ); }
    kvs::Stl* exec( const kvs::ObjectBase* object );
};

/*===========================================================================*/
/**
 *  @brief  Polygon exporter class as PLY format.
 */
/*===========================================================================*/
template <>
class PolygonExporter<kvs::Ply> :
        public kvs::ExporterBase,
        public kvs::Ply
{
public:
    PolygonExporter( const kvs::PolygonObject* object ) { this->exec( object ); }
    kvs::Ply* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs
