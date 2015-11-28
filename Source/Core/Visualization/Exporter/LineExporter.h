/*****************************************************************************/
/**
 *  @file   LineExporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineExporter.h 1794 2014-08-01 07:45:27Z naohisa.sakamoto@gmail.com $
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
