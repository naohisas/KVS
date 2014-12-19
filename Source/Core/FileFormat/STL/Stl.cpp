/*****************************************************************************/
/**
 *  @file   Stl.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Stl.cpp 1718 2014-03-08 08:59:23Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Stl.h"
#include <cstring>
#include <kvs/File>
#include <kvs/Assert>


namespace
{
const int MaxLineLength = 256;
const char* const Delimiter = " \t\n\r";
const std::string FileTypeToString[2] = { "ascii", "binary" };
}

namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns true if the file type is 'ascii'.
 *  @param  ifs [in] file pointer
 *  @return true ('ascii') or false ('binary')
 */
/*===========================================================================*/
bool IsAsciiType( FILE* ifs )
{
    // Go back file-pointer to head.
    fseek( ifs, 0, SEEK_SET );

    // Check file type.
    char buffer[ ::MaxLineLength ];
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '\n' ) continue;

        const char* head = strtok( buffer, ::Delimiter );
        if ( !strcmp( head, "solid" ) ) break;
        else if ( !strcmp( head, "facet" ) ) return true;
        else return false;
    }

    size_t counter = 0;
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( counter > 5 ) return false;
        if ( buffer[0] == '\n' ) continue;
        if ( strstr( buffer, "endsolid" ) ) return true;
        if ( strstr( buffer, "facet" ) ) return true;
        counter++;
    }

    return false;
}

} // end of namespace

namespace kvs
{

bool Stl::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "stl" || file.extension() == "STL" ||
         file.extension() == "stla" || file.extension() == "STLA" ||
         file.extension() == "stlb" || file.extension() == "STLB" ||
         file.extension() == "sla" || file.extension() == "SLA" ||
         file.extension() == "slb" || file.extension() == "SLB" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new STL class.
 */
/*===========================================================================*/
Stl::Stl():
    m_file_type( Stl::Ascii )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new STL class and reads a file.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
Stl::Stl( const std::string& filename ):
    m_file_type( Stl::Ascii )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the STL class.
 */
/*===========================================================================*/
Stl::~Stl()
{
}

/*===========================================================================*/
/**
 *  @brief  Prints the file information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Stl::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "File type : " << ::FileTypeToString[ m_file_type ] << std::endl;
    os << indent << "Number of triangles : " << m_normals.size() / 3;
}

/*===========================================================================*/
/**
 *  @brief  Read a STL file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Stl::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    FILE* ifs = fopen( filename.c_str(), "r" );
    if ( !ifs )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    bool success = false;
    if ( ::IsAsciiType( ifs ) )
    {
        m_file_type = Stl::Ascii;
        success = this->read_ascii( ifs );
    }
    else
    {
        m_file_type = Stl::Binary;
        success = this->read_binary( ifs );
    }
    BaseClass::setSuccess( success );

    fclose( ifs );

    return success;
}

/*===========================================================================*/
/**
 *  @brief  Writes the polygon data to a file as STL format.
 *  @param  filename [in] filename
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Stl::write( const std::string& filename )
{
    KVS_ASSERT( ( m_normals.size() / 3 ) == ( m_coords.size() / 9 ) );

    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    FILE* ofs = fopen( filename.c_str(), "w" );
    if ( !ofs )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    bool success = false;
    if ( m_file_type == Stl::Ascii )
    {
        success = this->write_ascii( ofs );
    }
    else
    {
        success = this->write_binary( ofs );
    }
    BaseClass::setSuccess( success );

    fclose( ofs );

    return success;
}

/*===========================================================================*/
/**
 *  @brief  Check file type whether the ascii or the binary.
 *  @param  ifs [in] file pointer
 *  @return true, if the file type is ascii
 */
/*===========================================================================*/
bool Stl::is_ascii_type( FILE* ifs )
{
    // Go back file-pointer to head.
    fseek( ifs, 0, SEEK_SET );

    // Check file type.
    char buffer[ ::MaxLineLength ];
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '\n' ) continue;

        const char* head = strtok( buffer, ::Delimiter );
        if ( !strcmp( head, "solid" ) ) break;
        else if ( !strcmp( head, "facet" ) ) return true;
        else return false;
    }

    size_t counter = 0;
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( counter > 5 ) return false;
        if ( buffer[0] == '\n' ) continue;
        if ( strstr( buffer, "endsolid" ) ) return true;
        if ( strstr( buffer, "facet" ) ) return true;
        counter++;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Reads the polygon data as ascii format.
 *  @param  ifs [in] file pointer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Stl::read_ascii( FILE* ifs )
{
    // Go back file-pointer to head.
    fseek( ifs, 0, SEEK_SET );

    // Check head line.
    char buffer[ ::MaxLineLength ];
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '\n' ) continue;

        const char* head = strtok( buffer, ::Delimiter );
        if ( !strcmp( head, "solid" ) ) break;

        if ( !strcmp( head, "facet" ) )
        {
            fseek( ifs, 0, SEEK_SET );
            break;
        }
    }

    std::vector<kvs::Real32> normals;
    std::vector<kvs::Real32> coords;
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '\n' ) continue;
        if ( strstr( buffer, "endsolid" ) ) break;

        // facet
        const char* facet = strtok( buffer, ::Delimiter );
        if ( !strcmp( facet, "facet" ) )
        {
            strtok( 0, ::Delimiter ); // 'normal'
            normals.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            normals.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            normals.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
        }
        else
        {
            kvsMessageError("Cannot find 'facet'.");
            return false;
        }

        // outer loop
        if ( fgets( buffer, ::MaxLineLength, ifs ) == 0 ) return false;
        const char* outer = strtok( buffer, ::Delimiter );
        const char* loop = strtok( 0, ::Delimiter );
        if ( strcmp( outer, "outer" ) || strcmp( loop, "loop" ) )
        {
            kvsMessageError("Cannot find 'outer loop'.");
            return false;
        }

        // vertex 0
        if ( fgets( buffer, ::MaxLineLength, ifs ) == 0 ) return false;
        const char* vertex0 = strtok( buffer, ::Delimiter );
        if ( !strcmp( vertex0, "vertex" ) )
        {
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
        }
        else
        {
            kvsMessageError("Cannot find 'vertex' (0).");
            return false;
        }

        // vertex 1
        if ( fgets( buffer, ::MaxLineLength, ifs ) == 0 ) return false;
        const char* vertex1 = strtok( buffer, ::Delimiter );
        if ( !strcmp( vertex1, "vertex" ) )
        {
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
        }
        else
        {
            kvsMessageError("Cannot find 'vertex' (1).");
            return false;
        }

        // vertex 2
        if ( fgets( buffer, ::MaxLineLength, ifs ) == 0 ) return false;
        const char* vertex2 = strtok( buffer, ::Delimiter );
        if ( !strcmp( vertex2, "vertex" ) )
        {
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
            coords.push_back( static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) ) );
        }
        else
        {
            kvsMessageError("Cannot find 'vertex' (2).");
            return false;
        }

        // endloop
        if ( fgets( buffer, ::MaxLineLength, ifs ) == 0 ) return false;
        const char* endloop = strtok( buffer, ::Delimiter );
        if ( strcmp( endloop, "endloop" ) )
        {
            kvsMessageError("Cannot find 'endloop'.");
            return false;
        }

        // endfacet
        if ( fgets( buffer, ::MaxLineLength, ifs ) == 0 ) return false;
        const char* endfacet = strtok( buffer, ::Delimiter );
        if ( strcmp( endfacet, "endfacet" ) )
        {
            kvsMessageError("Cannot find 'endfacet'.");
            return false;
        }
    }

    m_normals = kvs::ValueArray<kvs::Real32>( normals );
    m_coords = kvs::ValueArray<kvs::Real32>( coords );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Reads the polygon data as binary format.
 *  @param  ifs [in] file pointer
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool Stl::read_binary( FILE* ifs )
{
    // Go back file-pointer to head.
    fseek( ifs, 0, SEEK_SET );

    // Read header string (80bytes).
    const size_t HeaderLength = 80;
    char buffer[ HeaderLength ];
    if ( fread( buffer, sizeof( char ), HeaderLength, ifs ) != HeaderLength )
    {
        kvsMessageError("Cannot read a header string (80byets).");
        return false;
    }

    // Read a number of triangles (4bytes).
    kvs::UInt32 ntriangles = 0;
    if ( fread( &ntriangles, sizeof( kvs::UInt32 ), 1, ifs ) != 1 )
    {
        kvsMessageError("Cannot read a number of triangles.");
        return false;
    }

    // Memory allocation.
    m_normals.allocate( ntriangles * 3 );
    m_coords.allocate( ntriangles * 9 );

    // Read trianbles.
    kvs::Real32* normals = m_normals.data();
    kvs::Real32* coords = m_coords.data();
    kvs::UInt32 index3 = 0;
    kvs::UInt32 index9 = 0;
    for ( kvs::UInt32 i = 0; i < ntriangles; i++, index3 += 3, index9 += 9 )
    {
        // Normal vector (4x3=12byets)
        if ( fread( normals + index3, sizeof( kvs::Real32 ), 3, ifs ) != 3 )
        {
            kvsMessageError("Cannot read a normal vector.");
            m_normals.release();
            return false;
        }

        // Coordinate value (4x3=12bytes)
        if ( fread( coords + index9, sizeof( kvs::Real32 ), 9, ifs ) != 9 )
        {
            kvsMessageError("Cannot read a coordinate value.");
            m_coords.release();
            return false;
        }

        // Unused (2bytes)
        // NOTE: This block is sometimes used for storing color infomartion,
        // but we don't currently supported such color STL format.
        char unused[2];
        if ( fread( unused, sizeof( char ), 2, ifs ) != 2 )
        {
            kvsMessageError("Cannot read unused block (2bytes).");
            m_normals.release();
            m_coords.release();
            return false;
        }
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the polygon data as ascii format.
 *  @param  ifs [in] file pointer
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Stl::write_ascii( FILE* ofs )
{
    const char* header = "Generated by KVS";
    fprintf( ofs, "solid %s\n", header );

    const size_t ntriangles = m_normals.size() / 3;
    size_t index3 = 0;
    size_t index9 = 0;
    for ( size_t i = 0; i < ntriangles; i++, index3 += 3, index9 += 9 )
    {
        fprintf( ofs, "facet normal %f %f %f\n",
                 m_normals[ index3 + 0 ],
                 m_normals[ index3 + 1 ],
                 m_normals[ index3 + 2 ] );
        fprintf( ofs, "outer loop\n" );
        fprintf( ofs, "vertex %f %f %f\n",
                 m_coords[ index9 + 0 ],
                 m_coords[ index9 + 1 ],
                 m_coords[ index9 + 2 ] );
        fprintf( ofs, "vertex %f %f %f\n",
                 m_coords[ index9 + 3 ],
                 m_coords[ index9 + 4 ],
                 m_coords[ index9 + 5 ] );
        fprintf( ofs, "vertex %f %f %f\n",
                 m_coords[ index9 + 6 ],
                 m_coords[ index9 + 7 ],
                 m_coords[ index9 + 8 ] );
        fprintf( ofs, "endloop\n" );
        fprintf( ofs, "endfacet\n" );
    }

    fprintf( ofs, "endsolid\n" );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the polygon data as binary format.
 *  @param  ifs [in] file pointer
 *  @return true, if the writting process is done successfully
 */
/*===========================================================================*/
bool Stl::write_binary( FILE* ofs )
{
    // Header string (80bytes).
    char header[80]; memset( header, 0, 80 );
    sprintf( header, "Generated by KVS" );
    if ( fwrite( header, sizeof( char ), 80, ofs ) != 80 )
    {
        kvsMessageError("Cannot write a header string (80bytes).");
        return false;
    }

    // Number of triangles (4bytes).
    const kvs::UInt32 ntriangles = static_cast<kvs::UInt32>( m_normals.size() / 3 );
    if ( fwrite( &ntriangles, sizeof( kvs::UInt32 ), 1, ofs ) != 1 )
    {
        kvsMessageError("Cannot write a number of triangles.");
        return false;
    }

    // Triangles (50*ntriangles bytes)
    const kvs::Real32* normals = m_normals.data();
    const kvs::Real32* coords = m_coords.data();
    size_t index3 = 0;
    size_t index9 = 0;
    for ( size_t i = 0; i < ntriangles; i++, index3 += 3, index9 += 9 )
    {
        // Normal vector
        if ( fwrite( normals + index3, sizeof( kvs::Real32 ), 3, ofs ) != 3 )
        {
            kvsMessageError("Cannot write a normal vector.");
            return false;
        }

        // Coordinate values
        if ( fwrite( coords + index9, sizeof( kvs::Real32 ), 9, ofs ) != 9 )
        {
            kvsMessageError("Cannot write a coordinate values (3 vertices = 9 elements).");
            return false;
        }

        // Unused block (2bytes)
        const char unused[2] = {0,0};
        if ( fwrite( unused, sizeof( char ), 2, ofs ) != 2 )
        {
            kvsMessageError("Cannot write a unused block (2byets).");
            return false;
        }
    }

    return true;
}

} // end of namespace kvs
