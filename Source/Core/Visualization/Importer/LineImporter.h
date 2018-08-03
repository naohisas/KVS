/****************************************************************************/
/**
 *  @file   LineImporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineImporter.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__LINE_IMPORTER_H_INCLUDE
#define KVS__LINE_IMPORTER_H_INCLUDE

#include "ImporterBase.h"
#include <kvs/Module>
#include <kvs/LineObject>
#include <kvs/KVSMLLineObject>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Line object importer.
 */
/*==========================================================================*/
class LineImporter : public kvs::ImporterBase, public kvs::LineObject
{
    kvsModule( kvs::LineImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::LineObject );

public:

    LineImporter();
    LineImporter( const std::string& filename );
    LineImporter( const kvs::FileFormatBase* file_format );
    virtual ~LineImporter();

    SuperClass* exec( const kvs::FileFormatBase* file_format );
};

} // end of namespace kvs

#endif // KVS__LINE_IMPORTER_H_INCLUDE
