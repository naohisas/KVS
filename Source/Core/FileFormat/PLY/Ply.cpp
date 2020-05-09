/*****************************************************************************/
/**
 *  @file   Ply.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Ply.cpp 1384 2012-12-04 01:26:40Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <limits>
#include <kvs/DebugNew>
#include <kvs/Math>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/File>
#include <kvs/Assert>
#include "Ply.h"
#include "PlyFile.h"


namespace
{

struct Vertex
{
    float         x;
    float         y;
    float         z;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    float         nx;
    float         ny;
    float         nz;
};

struct Face
{
    unsigned char nverts;
    int*          verts;
};

} // end of namespace

namespace
{

const char* ElemNames[2] =
{
    "vertex",
    "face"
};

kvs::ply::PlyProperty VertProps[9] =
{
    {"x",     PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x),  0, 0, 0, 0},
    {"y",     PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y),  0, 0, 0, 0},
    {"z",     PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z),  0, 0, 0, 0},
    {"red",   PLY_UCHAR, PLY_UCHAR, offsetof(Vertex,r),  0, 0, 0, 0},
    {"green", PLY_UCHAR, PLY_UCHAR, offsetof(Vertex,g),  0, 0, 0, 0},
    {"blue",  PLY_UCHAR, PLY_UCHAR, offsetof(Vertex,b),  0, 0, 0, 0},
    {"nx",    PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nx), 0, 0, 0, 0},
    {"ny",    PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,ny), 0, 0, 0, 0},
    {"nz",    PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,nz), 0, 0, 0, 0},
};

kvs::ply::PlyProperty FaceProps[1] =
{
    {"vertex_indices",
     PLY_INT,
     PLY_INT,
     offsetof(Face,verts),
     1,
     PLY_UCHAR,
     PLY_UCHAR,
     offsetof(Face,nverts)},
};

} // end of namespace

namespace kvs
{

bool Ply::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "ply" || file.extension() == "PLY" )
    {
        return true;
    }

    return false;
}

Ply::Ply():
    m_file_type( Ply::Ascii ),
    m_nverts( 0 ),
    m_nfaces( 0 ),
    m_has_connections( false ),
    m_has_colors( false ),
    m_has_normals( false )
{
}

Ply::Ply( const std::string& filename ):
    m_file_type( Ply::Ascii ),
    m_nverts( 0 ),
    m_nfaces( 0 ),
    m_has_connections( false ),
    m_has_colors( false ),
    m_has_normals( false )
{
    this->read( filename );
}

Ply::~Ply()
{
}

Ply::FileType Ply::fileType() const
{
    return m_file_type;
}

size_t Ply::numberOfVertices() const
{
    return m_nverts;
}

size_t Ply::numberOfFaces() const
{
    return m_nfaces;
}

bool Ply::hasConnections() const
{
    return m_has_connections;
}

bool Ply::hasColors() const
{
    return m_has_colors;
}

bool Ply::hasNormals() const
{
    return m_has_normals;
}

const kvs::ValueArray<kvs::Real32>& Ply::coords() const
{
    return m_coords;
}

const kvs::ValueArray<kvs::UInt8>& Ply::colors() const
{
    return m_colors;
}

const kvs::ValueArray<kvs::Real32>& Ply::normals() const
{
    return m_normals;
}

const kvs::ValueArray<kvs::UInt32>& Ply::connections() const
{
    return m_connections;
}

const kvs::Vector3f& Ply::minCoord() const
{
    return m_min_coord;
}

const kvs::Vector3f& Ply::maxCoord() const
{
    return m_max_coord;
}

void Ply::setFileType( const Ply::FileType file_type )
{
    m_file_type = file_type;
}

void Ply::setCoords( const kvs::ValueArray<kvs::Real32>& coords )
{
    m_nverts = coords.size() / 3;
    m_coords = coords;
}

void Ply::setColors( const kvs::ValueArray<kvs::UInt8>& colors )
{
    m_has_colors = true;
    m_colors = colors;
}

void Ply::setNormals( const kvs::ValueArray<kvs::Real32>& normals )
{
    m_has_normals = true;
    m_normals = normals;
}

void Ply::setConnections( const kvs::ValueArray<kvs::UInt32>& connections )
{
    m_nfaces = connections.size() / 3;
    m_has_connections = true;
    m_connections = connections;
}

void Ply::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of vertices : " << m_nverts << std::endl;
    os << indent << "Number of faces : " << m_nfaces << std::endl;
    os << indent << "Min. coordinate : " << m_min_coord << std::endl;
    os << indent << "Max. coordinate : " << m_max_coord << std::endl;
}

bool Ply::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Read PLY file.
    kvs::ply::PlyFile* ply;
    int nelems;
    char** elist;
    int file_type;
    float version;
    ply = kvs::ply::ply_open_for_reading( filename.c_str(), &nelems, &elist, &file_type, &version );
    if( !ply )
    {
        kvsMessageError( "Cannot read ply file." );
        BaseClass::setSuccess( false );
        return false;
    }

    m_file_type = Ply::FileType( file_type );

    // For properties checking.
    kvs::ply::PlyElement *elem;
    int index;

    // Check for vertex data.
    if( ( elem = kvs::ply::find_element( ply, "vertex" ) ) == NULL ||
        kvs::ply::find_property( elem, "x", &index ) == NULL ||
        kvs::ply::find_property( elem, "y", &index ) == NULL ||
        kvs::ply::find_property( elem, "z", &index ) == NULL )
    {
        kvsMessageError( "Cannot read vertex element." );
        kvs::ply::ply_close( ply );
        BaseClass::setSuccess( false );
        return false;
    }

    // Check for index property.
    if( ( elem = kvs::ply::find_element( ply, "face" ) ) != NULL &&
        kvs::ply::find_property( elem, "vertex_indices", &index ) != NULL )
    {
        m_has_connections = true;
    }

    // Check for color property.
    if( ( elem = kvs::ply::find_element( ply, "vertex" ) ) != NULL &&
        kvs::ply::find_property( elem, "red",   &index ) != NULL &&
        kvs::ply::find_property( elem, "green", &index ) != NULL &&
        kvs::ply::find_property( elem, "blue",  &index ) != NULL )
    {
        m_has_colors = true;
    }

    // Check for normal property.
    if( ( elem = kvs::ply::find_element( ply, "vertex" ) ) != NULL &&
        kvs::ply::find_property( elem, "nx", &index ) != NULL &&
        kvs::ply::find_property( elem, "ny", &index ) != NULL &&
        kvs::ply::find_property( elem, "nz", &index ) != NULL )
    {
        m_has_normals = true;
    }

    // Read the data.
    for ( int i = 0; i < nelems; i++ )
    {
        char* elem_name = elist[i];
        int elem_count;
        int elem_nprops;
        kvs::ply::ply_get_element_description( ply, elem_name, &elem_count, &elem_nprops);

        // Read vertex information.
        if ( elem_name && !strcmp( "vertex", elem_name ) )
        {
            // Create a vertex list to hold all the vertices.
            m_nverts = elem_count;

            // Set up for getting vertex elements.
            kvs::ply::ply_get_property( ply, elem_name, &::VertProps[0] );
            kvs::ply::ply_get_property( ply, elem_name, &::VertProps[1] );
            kvs::ply::ply_get_property( ply, elem_name, &::VertProps[2] );
            m_coords.allocate( m_nverts * 3 );

            if ( m_has_colors )
            {
                kvs::ply::ply_get_property( ply, elem_name, &::VertProps[3] );
                kvs::ply::ply_get_property( ply, elem_name, &::VertProps[4] );
                kvs::ply::ply_get_property( ply, elem_name, &::VertProps[5] );
                m_colors.allocate( m_nverts * 3 );
            }

            if ( m_has_normals )
            {
                kvs::ply::ply_get_property( ply, elem_name, &::VertProps[6] );
                kvs::ply::ply_get_property( ply, elem_name, &::VertProps[7] );
                kvs::ply::ply_get_property( ply, elem_name, &::VertProps[8] );
                m_normals.allocate( m_nverts * 3 );
            }

            kvs::Real32* pcoords = m_coords.data();
            kvs::UInt8* pcolors = m_colors.data();
            kvs::Real32* pnormals = m_normals.data();
            for ( int j = 0; j < elem_count; j++ )
            {
                ::Vertex* vertex = (::Vertex*)malloc( sizeof(::Vertex) );
                if ( vertex ) memset( vertex, 0, sizeof(::Vertex) );
                kvs::ply::ply_get_element( ply, (void*)vertex );

                *(pcoords++) = vertex->x;
                *(pcoords++) = vertex->y;
                *(pcoords++) = vertex->z;

                if ( m_has_colors )
                {
                    *(pcolors++) = vertex->r;
                    *(pcolors++) = vertex->g;
                    *(pcolors++) = vertex->b;
                }

                if ( m_has_normals )
                {
                    *(pnormals++) = vertex->nx;
                    *(pnormals++) = vertex->ny;
                    *(pnormals++) = vertex->nz;
                }
                free( vertex );
            }
        }

        // Read face information.
        if ( m_has_connections )
        {
            if ( elem_name && !strcmp( "face", elem_name ) )
            {
                // Create a list to hold all the face elements.
                m_nfaces = elem_count;

                // Set up for getting face elements.
                kvs::ply::ply_get_property( ply, elem_name, &::FaceProps[0] );
                m_connections.allocate( m_nfaces * 3 );

                // Grab all the face elements.
                kvs::UInt32* pconnections = m_connections.data();
                for ( int j = 0; j < elem_count; j++ )
                {
                    ::Face* face = (::Face*)malloc( sizeof(::Face) );
                    if ( face ) memset( face, 0, sizeof(::Face) );
                    kvs::ply::ply_get_element( ply, (void*)face );

                    *(pconnections++) = face->verts[0];
                    *(pconnections++) = face->verts[1];
                    *(pconnections++) = face->verts[2];
                    free( face );
                }
            }
        }

        free( elist[i] );
    }
    free( elist );

    this->calculate_min_max_coord();
    if ( !m_has_normals ) this->calculate_normals();
    if ( !m_has_connections ) m_nfaces = m_nverts / 3;

    kvs::ply::ply_close( ply );

    return true;
}

bool Ply::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Write PLY file.
    kvs::ply::PlyFile* ply;
    const int nelems = 2;
    const int file_type = m_file_type;
    float version;
    ply = kvs::ply::ply_open_for_writing( filename.c_str(), nelems, ::ElemNames, file_type, &version );
    if( !ply )
    {
        kvsMessageError( "Cannot open writing ply file." );
        BaseClass::setSuccess( false );
        return false;
    }

    const size_t nverts = m_nverts;
    kvs::ply::ply_element_count( ply, "vertex", int( nverts ) );
    kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[0] );
    kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[1] );
    kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[2] );

    if ( m_has_colors )
    {
        kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[3] );
        kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[4] );
        kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[5] );
    }

    if ( m_has_normals )
    {
        kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[6] );
        kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[7] );
        kvs::ply::ply_describe_property( ply, "vertex", &::VertProps[8] );
    }

    if ( m_nfaces > 0 )
    {
        const size_t nfaces = m_nfaces;
        kvs::ply::ply_element_count( ply, "face", int( nfaces ) );
        kvs::ply::ply_describe_property( ply, "face", &::FaceProps[0] );
    }

    kvs::ply::ply_header_complete( ply );

    const kvs::Real32* pcoords = m_coords.data();
    const kvs::UInt8* pcolors = m_colors.data();
    const kvs::Real32* pnormals = m_normals.data();
    kvs::ply::ply_put_element_setup( ply, "vertex" );
    for ( size_t i = 0; i < nverts; i++ )
    {
        ::Vertex* vertex = (::Vertex*)malloc( sizeof(::Vertex) );
        if ( vertex ) memset( vertex, 0, sizeof(::Vertex) );

        vertex->x = *(pcoords++);
        vertex->y = *(pcoords++);
        vertex->z = *(pcoords++);

        if ( m_has_colors )
        {
            vertex->r = *(pcolors++);
            vertex->g = *(pcolors++);
            vertex->b = *(pcolors++);
        }

        if ( m_has_normals )
        {
            vertex->nx = *(pnormals++);
            vertex->ny = *(pnormals++);
            vertex->nz = *(pnormals++);
        }

        kvs::ply::ply_put_element( ply, (void*)vertex );

        free( vertex );
    }

    if ( m_has_connections )
    {
        const size_t nfaces = m_nfaces;
        const kvs::UInt32* pconnections = m_connections.data();
        kvs::ply::ply_put_element_setup( ply, "face" );
        for ( size_t i = 0; i < nfaces; i++ )
        {
            ::Face* face = (::Face*)malloc( sizeof(::Face) );
            if ( face ) memset( face, 0, sizeof(::Face) );

            face->verts[0] = *(pconnections++);
            face->verts[1] = *(pconnections++);
            face->verts[2] = *(pconnections++);

            kvs::ply::ply_put_element( ply, (void*)face );
        }
    }

    kvs::ply::ply_close( ply );

    return true;
}

void Ply::calculate_min_max_coord()
{
    m_min_coord = kvs::Vector3f::Constant( std::numeric_limits<float>::max() );
    m_max_coord = kvs::Vector3f::Constant( std::numeric_limits<float>::min() );
    const kvs::Real32* pcoords = m_coords.data();
    for ( size_t i = 0; i < m_nverts; i++ )
    {
        const kvs::Real32 x= *(pcoords++);
        const kvs::Real32 y= *(pcoords++);
        const kvs::Real32 z= *(pcoords++);

        m_min_coord.x() = kvs::Math::Min( m_min_coord.x(), x );
        m_min_coord.y() = kvs::Math::Min( m_min_coord.y(), y );
        m_min_coord.z() = kvs::Math::Min( m_min_coord.z(), z );

        m_max_coord.x() = kvs::Math::Max( m_max_coord.x(), x );
        m_max_coord.y() = kvs::Math::Max( m_max_coord.y(), y );
        m_max_coord.z() = kvs::Math::Max( m_max_coord.z(), z );
    }
}

void Ply::calculate_normals()
{
    kvs::ValueArray<kvs::UInt32> counter( m_nverts );
    counter.fill( 0 );

    m_normals.allocate( m_nverts * 3 );
    const kvs::UInt32* pconnections = m_connections.data();
    const kvs::Real32* pcoords = m_coords.data();
    for ( size_t i = 0; i < m_nfaces; i++ )
    {
        // Calculate normal vector for each triangles.
        const kvs::UInt32 index0 = *(pconnections++);
        const kvs::UInt32 index1 = *(pconnections++);
        const kvs::UInt32 index2 = *(pconnections++);

        const kvs::Vector3f v0( pcoords + 3 * index0 );
        const kvs::Vector3f v1( pcoords + 3 * index1 );
        const kvs::Vector3f v2( pcoords + 3 * index2 );
        const kvs::Vector3f normal = ( ( v1 - v0 ).cross( v2 - v1 ) ).normalized();

        // Sum of the normal vectors of the adjacent triangles for the vertex.
        m_normals[ 3 * index0 + 0 ] = normal.x();
        m_normals[ 3 * index0 + 1 ] = normal.y();
        m_normals[ 3 * index0 + 2 ] = normal.z();
        counter[ index0 ] += 1;

        m_normals[ 3 * index1 + 0 ] = normal.x();
        m_normals[ 3 * index1 + 1 ] = normal.y();
        m_normals[ 3 * index1 + 2 ] = normal.z();
        counter[ index1 ] += 1;

        m_normals[ 3 * index2 + 0 ] = normal.x();
        m_normals[ 3 * index2 + 1 ] = normal.y();
        m_normals[ 3 * index2 + 2 ] = normal.z();
        counter[ index2 ] += 1;
    }

    for ( size_t i = 0; i < m_nverts; i++ )
    {
        if ( counter[i] == 0 ) continue;
        m_normals[ 3 * i + 0 ] /= static_cast<kvs::Real32>( counter[i] );
        m_normals[ 3 * i + 1 ] /= static_cast<kvs::Real32>( counter[i] );
        m_normals[ 3 * i + 2 ] /= static_cast<kvs::Real32>( counter[i] );
    }
}

} // end of namespace kvs
