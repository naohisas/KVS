/*****************************************************************************/
/**
 *  @file   Stl.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Stl.h 1718 2014-03-08 08:59:23Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__STL_H_INCLUDE
#define KVS__STL_H_INCLUDE

#include <iostream>
#include <fstream>
#include <string>
#include <kvs/ValueArray>
#include <kvs/FileFormatBase>
#include <kvs/Type>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  STL file format class.
 */
/*===========================================================================*/
class Stl : public kvs::FileFormatBase
{
public:

    typedef kvs::FileFormatBase BaseClass;

    enum FileType
    {
        Ascii, // ascii type
        Binary // binary type
    };

protected:

    FileType m_file_type; ///< file type
    kvs::ValueArray<kvs::Real32> m_normals; /// normal vector array
    kvs::ValueArray<kvs::Real32> m_coords; /// coordinate value array

public:

    static bool CheckExtension( const std::string& filename );

public:

    Stl();
    Stl( const std::string& filename );
    virtual ~Stl();

    FileType fileType() const { return m_file_type; }
    const kvs::ValueArray<kvs::Real32>& normals() const { return m_normals; }
    const kvs::ValueArray<kvs::Real32>& coords() const { return m_coords; }
    size_t numberOfTriangles() const { return m_normals.size() / 3; }

    void setFileType( const FileType file_type ) { m_file_type = file_type; }
    void setNormals( const kvs::ValueArray<kvs::Real32>& normals ) { m_normals = normals; }
    void setCoords( const kvs::ValueArray<kvs::Real32>& coords ) { m_coords = coords; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    bool is_ascii_type( FILE* ifs );
    bool read_ascii( FILE* ifs );
    bool read_binary( FILE* ifs );
    bool write_ascii( FILE* ifs );
    bool write_binary( FILE* ifs );

public:
    KVS_DEPRECATED( size_t ntriangles() const ) { return this->numberOfTriangles(); }
};

} // end of namespace kvs

#endif // KVS__STL_H_INCLUDE
