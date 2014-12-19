/*****************************************************************************/
/**
 *  @file   Ply.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Ply.h 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__PLY_H_INCLUDE
#define KVS__PLY_H_INCLUDE

#include <iostream>
#include <fstream>
#include <string>
#include <kvs/FileFormatBase>
#include <kvs/Vector3>
#include <kvs/ValueArray>
#include <kvs/Platform>
#include <kvs/Type>
#include <kvs/Indent>
#include "PlyFile.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PLY class.
 */
/*===========================================================================*/
class Ply : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;

    enum FileType
    {
        Ascii = PLY_ASCII, // ascii type
#if defined( KVS_PLATFORM_LITTLE_ENDIAN )
        Binary = PLY_BINARY_LE // binary type (little endian)
#else
        Binary = PLY_BINARY_BE // binary type (big endian)
#endif
    };

private:

    FileType m_file_type; ///< file type
    size_t m_nverts; ///< number of vertices
    size_t m_nfaces; ///< number of faces
    kvs::ValueArray<kvs::Real32> m_coords; ///< coordinate value array
    kvs::ValueArray<kvs::UInt8> m_colors; ///< color value array
    kvs::ValueArray<kvs::Real32> m_normals; ///< normal value array
    kvs::ValueArray<kvs::UInt32> m_connections; ///< connection array
    bool m_has_connections; ///< true, if the connections is included
    bool m_has_colors; ///< true, if the colors is included
    bool m_has_normals; ///< true, if the normals is included
    kvs::Vector3f m_min_coord; ///< min. coordinate
    kvs::Vector3f m_max_coord; ///< max. coordinate

public:

    static bool CheckExtension( const std::string& filename );

public:

    Ply();
    Ply( const std::string& filename );
    virtual ~Ply();

    FileType fileType() const;
    size_t numberOfVertices() const;
    size_t numberOfFaces() const;
    bool hasConnections() const;
    bool hasColors() const;
    bool hasNormals() const;
    const kvs::ValueArray<kvs::Real32>& coords() const;
    const kvs::ValueArray<kvs::UInt8>& colors() const;
    const kvs::ValueArray<kvs::Real32>& normals() const;
    const kvs::ValueArray<kvs::UInt32>& connections() const;
    const kvs::Vector3f& minCoord() const;
    const kvs::Vector3f& maxCoord() const;

    void setFileType( const FileType file_type );
    void setCoords( const kvs::ValueArray<kvs::Real32>& coords );
    void setColors( const kvs::ValueArray<kvs::UInt8>& colors );
    void setNormals( const kvs::ValueArray<kvs::Real32>& normals );
    void setConnections( const kvs::ValueArray<kvs::UInt32>& connections );

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    void calculate_min_max_coord();
    void calculate_normals();
};

} // end of namespace kvs

#endif // KVS__PLY_H_INCLUDE
