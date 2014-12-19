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
#ifndef KVS__LINE_EXPORTER_H_INCLUDE
#define KVS__LINE_EXPORTER_H_INCLUDE

#include <kvs/LineObject>
#include <kvs/KVSMLObjectLine>
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
 *  @brief  Line exporter class as KVSMLObjectLine format.
 */
/*===========================================================================*/
template <>
class LineExporter<kvs::KVSMLObjectLine> :
        public kvs::ExporterBase,
        public kvs::KVSMLObjectLine
{
public:
    LineExporter( const kvs::LineObject* object );
    kvs::KVSMLObjectLine* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs

#endif // KVS__LINE_EXPORTER_H_INCLUDE
