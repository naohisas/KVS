/*****************************************************************************/
/**
 *  @file   TableImporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TableImporter.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/ImporterBase>
#include <kvs/Module>
#include <kvs/TableObject>
#include <kvs/KVSMLTableObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Importer class for TableObject.
 */
/*===========================================================================*/
class TableImporter : public kvs::ImporterBase, public kvs::TableObject
{
    kvsModule( kvs::TableImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::TableObject );

public:
    TableImporter();
    TableImporter( const std::string& filename );
    TableImporter( const kvs::FileFormatBase* file_format );

    SuperClass* exec( const kvs::FileFormatBase* file_format );
};

} // end of namespace kvs
