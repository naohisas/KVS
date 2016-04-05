/*****************************************************************************/
/**
 *  @file   TableImporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TableImporter.cpp 1403 2012-12-07 07:35:18Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "TableImporter.h"
#include <kvs/DebugNew>
#include <kvs/KVSMLTableObject>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TableImporter class.
 */
/*===========================================================================*/
TableImporter::TableImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new TableImporter class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
TableImporter::TableImporter( const std::string& filename )
{
    if ( kvs::KVSMLTableObject::CheckExtension( filename ) )
    {
        BaseClass::setSuccess( SuperClass::read( filename ) );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Cannot import '%s'.",filename.c_str());
        return;
    }
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new TableImporter class.
 *  @param  file_format [in] pointer to the file format
 */
/*===========================================================================*/
TableImporter::TableImporter( const kvs::FileFormatBase* file_format )
{
    this->exec( file_format );
}

/*===========================================================================*/
/**
 *  @brief  Returns pointer to the imported table object.
 *  @param  file_format [in] pointer to the file format
 *  @return pointer to the imported table object
 */
/*===========================================================================*/
TableImporter::SuperClass* TableImporter::exec( const kvs::FileFormatBase* file_format )
{
    if ( !file_format )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is NULL.");
        return NULL;
    }

    if ( dynamic_cast<const kvs::KVSMLTableObject*>( file_format ) )
    {
        BaseClass::setSuccess( SuperClass::read( file_format->filename() ) );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is not supported.");
        return NULL;
    }

    return this;
}

} // end of namespace kvs
