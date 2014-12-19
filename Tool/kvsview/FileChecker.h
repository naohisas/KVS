/*****************************************************************************/
/**
 *  @file   FileChecker.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FileChecker.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVSVIEW__FILE_CHECKER_H_INCLUDE
#define KVSVIEW__FILE_CHECKER_H_INCLUDE

#include <kvs/File>
#include <kvs/AVSField>
#include <kvs/AVSUcd>
#include <kvs/Stl>
#include <kvs/Ply>
#include <kvs/KVSMLObjectPoint>
#include <kvs/KVSMLObjectLine>
#include <kvs/KVSMLObjectPolygon>
#include <kvs/KVSMLObjectStructuredVolume>
#include <kvs/KVSMLObjectUnstructuredVolume>
#include <kvs/DicomList>


namespace kvsview
{

namespace FileChecker
{

/*===========================================================================*/
/**
 *  @brief  Check function for the supported point file format in the command.
 *  @param  filename [in] input filename
 *  @return true if the input file is supported
 */
/*===========================================================================*/
inline const bool ImportablePoint( const std::string& filename )
{
    // KVSML format.
    if ( kvs::KVSMLObjectPoint::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLObjectPoint::CheckFormat( filename ) )
        {
            return( true );
        }
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Check function for the supported line file format in the command.
 *  @param  filename [in] input filename
 *  @return true if the input file is supported
 */
/*===========================================================================*/
inline const bool ImportableLine( const std::string& filename )
{
    // KVSML format.
    if ( kvs::KVSMLObjectLine::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLObjectLine::CheckFormat( filename ) )
        {
            return( true );
        }
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Check function for the supported polygon file format in the command.
 *  @param  filename [in] input filename
 *  @return true if the input file is supported
 */
/*===========================================================================*/
inline const bool ImportablePolygon( const std::string& filename )
{
    // KVSML format.
    if ( kvs::KVSMLObjectPolygon::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLObjectPolygon::CheckFormat( filename ) )
        {
            return( true );
        }
    }

    // STL format.
    if ( kvs::Stl::CheckExtension( filename ) )
    {
        return( true );
    }

    // Ply format.
    if ( kvs::Ply::CheckExtension( filename ) )
    {
        return( true );
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Check for the supported structured volume object.
 *  @param  filename [in] input filename
 *  @return true if the input file is supported as structured volume object
 */
/*===========================================================================*/
inline const bool ImportableStructuredVolume( const std::string& filename )
{
    kvs::File file( filename );
    if ( file.isFile() )
    {
        // KVSML format.
        if ( kvs::KVSMLObjectStructuredVolume::CheckExtension( filename ) )
        {
            if ( kvs::KVSMLObjectStructuredVolume::CheckFormat( filename ) )
            {
                return( true );
            }
        }

        // AVS field format.
        if ( kvs::AVSField::CheckExtension( filename ) )
        {
            return( true );
        }
    }
    else
    {
        // DICOM list.
        if ( kvs::DicomList::CheckDirectory( filename ) )
        {
            return( true );
        }
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Check for the supported unstructured volume object.
 *  @param  filename [in] input filename
 *  @return true if the input file is supported as unstructured volume object
 */
/*===========================================================================*/
inline const bool ImportableUnstructuredVolume( const std::string& filename )
{
    // KVSML format.
    if ( kvs::KVSMLObjectUnstructuredVolume::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLObjectUnstructuredVolume::CheckFormat( filename ) )
        {
            return( true );
        }
    }

    // AVS UCD format.
    if ( kvs::AVSUcd::CheckExtension( filename ) )
    {
        return( true );
    }

    return( false );
}

} // end of namespace FileChecker

} // end of namespace kvsview

#endif // KVSVIEW__FILE_CHECKER_H_INCLUDE
