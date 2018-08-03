/****************************************************************************/
/**
 *  @file   LineImporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LineImporter.cpp 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "LineImporter.h"
#include <kvs/DebugNew>
#include <kvs/KVSMLLineObject>
#include <kvs/Math>
#include <kvs/Vector3>
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new LineImporter class.
 */
/*==========================================================================*/
LineImporter::LineImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineImporter class.
 *  @param  filename [in] input filename
 */
/*===========================================================================*/
LineImporter::LineImporter( const std::string& filename )
{
    if ( kvs::KVSMLLineObject::CheckExtension( filename ) )
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

/*==========================================================================*/
/**
 *  @brief  Constructs a new LineImporter class.
 *  @param file_format [in] pointer to the file format
 */
/*==========================================================================*/
LineImporter::LineImporter( const kvs::FileFormatBase* file_format )
{
    this->exec( file_format );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the LineImporter class.
 */
/*===========================================================================*/
LineImporter::~LineImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the import process.
 *  @param  file_format [in] pointer to the file format class
 *  @return pointer to the imported line object
 */
/*===========================================================================*/
LineImporter::SuperClass* LineImporter::exec( const kvs::FileFormatBase* file_format )
{
    if ( !file_format )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is NULL.");
        return NULL;
    }

    if ( dynamic_cast<const kvs::KVSMLLineObject*>( file_format ) )
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
