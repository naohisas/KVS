/*****************************************************************************/
/**
 *  @file   FileChecker.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/File>
#include <kvs/AVSField>
#include <kvs/AVSUcd>
#include <kvs/Stl>
#include <kvs/Ply>
#include <kvs/KVSMLPointObject>
#include <kvs/KVSMLLineObject>
#include <kvs/KVSMLPolygonObject>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/KVSMLUnstructuredVolumeObject>
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
    if ( kvs::KVSMLPointObject::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLPointObject::CheckFormat( filename ) )
        {
            return true;
        }
    }

    return false;
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
    if ( kvs::KVSMLLineObject::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLLineObject::CheckFormat( filename ) )
        {
            return true;
        }
    }

    return false;
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
    if ( kvs::KVSMLPolygonObject::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLPolygonObject::CheckFormat( filename ) )
        {
            return true;
        }
    }

    // STL format.
    if ( kvs::Stl::CheckExtension( filename ) )
    {
        return true;
    }

    // Ply format.
    if ( kvs::Ply::CheckExtension( filename ) )
    {
        return true;
    }

    return false;
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
        if ( kvs::KVSMLStructuredVolumeObject::CheckExtension( filename ) )
        {
            if ( kvs::KVSMLStructuredVolumeObject::CheckFormat( filename ) )
            {
                return true;
            }
        }

        // AVS field format.
        if ( kvs::AVSField::CheckExtension( filename ) )
        {
            return true;
        }
    }
    else
    {
        // DICOM list.
        if ( kvs::DicomList::CheckDirectory( filename ) )
        {
            return true;
        }
    }

    return false;
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
    if ( kvs::KVSMLUnstructuredVolumeObject::CheckExtension( filename ) )
    {
        if ( kvs::KVSMLUnstructuredVolumeObject::CheckFormat( filename ) )
        {
            return true;
        }
    }

    // AVS UCD format.
    if ( kvs::AVSUcd::CheckExtension( filename ) )
    {
        return true;
    }

    return false;
}

} // end of namespace FileChecker

} // end of namespace kvsview
