/****************************************************************************/
/**
 *  @file   UnstructuredVolumeImporter.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeImporter.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include "ImporterBase.h"
#include <kvs/Module>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/AVSUcd>
#include <kvs/AVSField>
#include <kvs/FieldViewData>


namespace kvs
{

/*==========================================================================*/
/**
 *  Unstructured volume object importer class.
 */
/*==========================================================================*/
class UnstructuredVolumeImporter : public kvs::ImporterBase, public kvs::UnstructuredVolumeObject
{
    kvsModule( kvs::UnstructuredVolumeImporter, Importer );
    kvsModuleBaseClass( kvs::ImporterBase );
    kvsModuleSuperClass( kvs::UnstructuredVolumeObject );

public:
    UnstructuredVolumeImporter();
    UnstructuredVolumeImporter( const std::string& filename );
    UnstructuredVolumeImporter( const kvs::FileFormatBase* file_format );
    virtual ~UnstructuredVolumeImporter();

    SuperClass* exec( const kvs::FileFormatBase* file_format );

private:
    void import( const kvs::AVSUcd* const ucd );
    void import( const kvs::AVSField* const field );
    void import( const kvs::FieldViewData* const fvdata );
};

} // end of namespace kvs
