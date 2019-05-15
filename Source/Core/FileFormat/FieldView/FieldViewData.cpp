/*****************************************************************************/
/**
 *  @file   FieldViewData.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "FieldViewData.h"
#include "FVReaderTags.h"
#include <cstdio>
#include <kvs/String>
#include <kvs/Message>
#include <kvs/Endian>
#include <kvs/ValueArray>
#include <kvs/File>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Data record reader class.
 */
/*===========================================================================*/
class Record
{
    FILE* m_fp; //< file pointer
    bool m_swap; //< flag for byte swapping
    size_t m_offset; //< offset bytes for reading record length
    size_t m_length; //< record length

public:

    static void Error()
    {
        kvsMessageError( "Failed to read the record." );
    }

    template <typename T>
    static T PeekValue( FILE* fp, bool swap, size_t offset )
    {
        T value = 0;

        fpos_t p;
        if ( fgetpos( fp, &p ) != 0 ) value = 0;
        if ( fseek( fp, offset, SEEK_CUR ) != 0 ) value = 0;
        if ( fread( &value, sizeof(T), 1, fp ) < 1 ) value = 0;
        if ( fsetpos( fp, &p ) != 0 ) value = 0;
        if ( swap ) kvs::Endian::Swap( &value );

        return value;
    }

    static std::string PeekString( size_t size, FILE* fp, size_t offset )
    {
        fpos_t p;
        if ( fgetpos( fp, &p ) != 0 ) Error();
        if ( fseek( fp, offset, SEEK_CUR ) != 0 ) Error();

        kvs::ValueArray<char> values( size );
        if ( fread( values.data(), sizeof(char), size, fp ) < size ) Error();
        if ( fsetpos( fp, &p ) != 0 ) Error();

        return std::string( values.data(), values.size() );
    }

public:

    Record( FILE* fp, bool swap, size_t offset ):
        m_fp( fp ),
        m_swap( swap ),
        m_offset( offset ),
        m_length( 0 )
    {
        KVS_ASSERT( m_offset == 0 || m_offset == 4 );

        if ( m_offset == 4 )
        {
            int length = this->read_length_field();
            if ( length < 0 ) Error();
            m_length = static_cast<size_t>( length );
        }
    }

    ~Record()
    {
        if ( m_offset == 4 )
        {
            int retval = this->seek_length_field();
            if ( retval != 0 ) Error();
        }
    }

    //size_t offset() const { return m_offset; }
    size_t length() const { return m_length; }
    //void skip() { if ( fseek( m_fp, m_length, SEEK_CUR ) != 0 ) Error(); }

    template <typename T>
    T readValue()
    {
        T value;
        if ( fread( &value, sizeof(T), 1, m_fp ) < 1 ) perror( NULL );
        if ( m_swap ) kvs::Endian::Swap( &value );
        return value;
    }

    template <typename T>
    kvs::ValueArray<T> readValues( size_t size )
    {
        kvs::ValueArray<T> values( size );
        if ( fread( values.data(), sizeof(T), size, m_fp ) < size ) Error();
        if ( m_swap ) kvs::Endian::Swap( values.data(), size );
        return values;
    }

    std::string readString( size_t size )
    {
        kvs::ValueArray<char> values( size );
        if ( fread( values.data(), sizeof(char), size, m_fp ) < size ) Error();
        return std::string( values.data(), values.size() );
    }

private:

    int seek_length_field()
    {
        KVS_ASSERT( m_offset == 4 );
        return fseek( m_fp, m_offset, SEEK_CUR );
    }

    int read_length_field()
    {
        KVS_ASSERT( m_offset == 4 );
        int length = 0;
        if ( fread( &length, m_offset, 1, m_fp ) < 1 ) return -1;
        if ( m_swap ) kvs::Endian::Swap( &length );
        return length;
    }
};

/*===========================================================================*/
/**
 *  @brief  Format check class.
 */
/*===========================================================================*/
class Format
{
    std::string m_filename; ///< filename
    bool m_swap; ///< flag for byte-swap
    size_t m_offset; ///< offset byte

public:

    Format( const std::string& filename ):
        m_filename( filename ) {}

    bool swap() const { return m_swap; }
    size_t offset() const { return m_offset; }

    bool isAscii()
    {
        FILE* fp = fopen( m_filename.c_str(), "r" );
        if ( !fp )
        {
            kvsMessageError( "Cannot open %s.", m_filename.c_str() );
            return false;
        }

        bool retval = Record::PeekString( 9, fp, 0 ) == "FIELDVIEW";

        fclose( fp );

        return retval;
    }

    bool isBinary()
    {
        FILE* fp = fopen( m_filename.c_str(), "rb" );
        if ( !fp )
        {
            kvsMessageError( "Cannot open %s.", m_filename.c_str() );
            return false;
        }

        bool retval = false;
        if ( this->check_binary( fp, false, 0 ) ) { m_swap = false; m_offset = 0; retval = true; }
        if ( this->check_binary( fp, false, 4 ) ) { m_swap = false; m_offset = 4; retval = true; }
        if ( this->check_binary( fp, true,  0 ) ) { m_swap = true;  m_offset = 0; retval = true; }
        if ( this->check_binary( fp, true,  4 ) ) { m_swap = true;  m_offset = 4; retval = true; }

        fclose( fp );

        return retval;
    }

private:

    bool check_binary( FILE* fp, bool swap, size_t offset ) const
    {
        int magic = Record::PeekValue<int>( fp, swap, offset );
        return magic == FV_MAGIC;
    }
};

}


namespace kvs
{

bool FieldViewData::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "fv" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Returns total number of nodes.
 *  @return total number of nodes
 */
/*===========================================================================*/
size_t FieldViewData::totalNumberOfNodes() const
{
    size_t nnodes = 0;
    const size_t ngrids = this->numberOfGrids();
    for ( size_t i = 0; i < ngrids; i++ )
    {
        nnodes += this->grid(i).nnodes;
    }
    return nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Returns total number of elements.
 *  @param  etype [in] element type (0:all, 1:tet, 2:hex, 3:prism, 4:pyramid)
 *  @return total number of elements
 */
/*===========================================================================*/
size_t FieldViewData::totalNumberOfElements( const size_t etype ) const
{
    size_t nelements = 0;
    const size_t ngrids = this->numberOfGrids();
    for ( size_t i = 0; i < ngrids; i++ )
    {
        nelements += this->grid(i).nelements[ etype ];
    }
    return nelements;
}

/*===========================================================================*/
/**
 *  @brief  Print data information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void FieldViewData::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Version : " << m_version.major << "." << m_version.minor << std::endl;
    os << indent << "Constant :" << std::endl;
    os << indent << "\tTime : " << m_constant.time << std::endl;
    os << indent << "\tFSMach : " << m_constant.mach << std::endl;
    os << indent << "\tAlpha : " << m_constant.alpha << std::endl;
    os << indent << "\tRe : " << m_constant.re << std::endl;
    os << indent << "Number of variables : " << numberOfVariables() << std::endl;
    for ( size_t i = 0; i < numberOfVariables(); i++ )
    {
        os << indent << "\t" << variableName(i) << std::endl;
    }
    os << indent << "Number of grids : " << numberOfGrids() << std::endl;
    for ( size_t i = 0; i < numberOfGrids(); i++ )
    {
        os << indent << "\tNumber of nodes : " << m_grids[i].nodes.size() << std::endl;
        os << indent << "\tNumber of elements : " << m_grids[i].elements.size() << std::endl;
        os << indent << "\t\tTet : " << m_grids[i].nelements[1] << std::endl;
        os << indent << "\t\tHex : " << m_grids[i].nelements[2] << std::endl;
        os << indent << "\t\tPrism : " << m_grids[i].nelements[3] << std::endl;
        os << indent << "\t\tPyramid : " << m_grids[i].nelements[4] << std::endl;
    }
}

/*===========================================================================*/
/**
 *  @brief  Read data from a file
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::read( const std::string& filename )
{
    setFilename( filename );
    setSuccess( false );

    ::Format format( filename );
    if ( format.isAscii() )
    {
        return this->readAscii( filename );
    }
    else if ( format.isBinary() )
    {
        return this->readBinary( filename, format.swap(), format.offset() );
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Read data from grid and result files.
 *  @param  gfilename [in] filename of grid data
 *  @param  rfilename [in] filename of result data
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::read( const std::string& gfilename, const std::string& rfilename )
{
    return this->readBinaryGrid( gfilename ) && this->readBinaryResult( rfilename );
}

/*===========================================================================*/
/**
 *  @brief  Read data from ascii type file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readAscii( const std::string& filename )
{
    setFilename( filename );
    setSuccess( true );

    FILE* fp = fopen( filename.c_str(), "r" );
    if ( !fp )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        setSuccess( false );
        return false;
    }

    this->read_version( fp );
    this->read_constants( fp );
    this->read_grids( fp );
    this->read_variable_names( fp );

    for ( size_t i = 0; i < m_ngrids; i++ )
    {
        if ( m_grids.size() <= i )
        {
            Grid grid;
            grid.nelements[0] = 0;
            grid.nelements[1] = 0;
            grid.nelements[2] = 0;
            grid.nelements[3] = 0;
            grid.nelements[4] = 0;
            m_grids.push_back( grid );
        }

        this->read_nodes( fp, i );
        this->read_elements( fp, i );
        this->read_variables( fp, i );
    }

    fclose( fp );

    // Initialize variables for importing
    m_importing_element_type = 0;
    m_importing_grid_index = m_ngrids;
    m_importing_variable_index = 0;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read data from binary type file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readBinary( const std::string& filename )
{
    setFilename( filename );
    setSuccess( false );

    ::Format format( filename );
    if ( format.isBinary() )
    {
        return this->readBinary( filename, format.swap(), format.offset() );
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Read data from binary type file.
 *  @param  filename [in] filename
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readBinary( const std::string& filename, bool swap, size_t offset )
{
    setFilename( filename );
    setSuccess( true );

    FILE* fp = fopen( filename.c_str(), "rb" );
    if ( !fp )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        setSuccess( false );
        return false;
    }

    this->read_magic( fp, swap, offset );
    this->read_title( fp, swap, offset );
    this->read_version( fp, swap, offset );
    this->read_constants( fp, swap, offset );
    this->read_grids( fp, swap, offset );
    this->read_boundary_condition( fp, swap, offset );
    this->read_variable_names( fp, swap, offset );
    this->read_variable_names_on_face( fp, swap, offset );

    const size_t ngrids = this->numberOfGrids();
    for ( size_t i = 0; i < ngrids; i++ )
    {
        if ( m_grids.size() <= i )
        {
            Grid grid;
            grid.nelements[0] = 0;
            grid.nelements[1] = 0;
            grid.nelements[2] = 0;
            grid.nelements[3] = 0;
            grid.nelements[4] = 0;
            m_grids.push_back( grid );
        }

        this->read_nnodes( fp, swap, offset, i );
        this->read_nodes( fp, swap, offset, i );
        this->read_faces( fp, swap, offset, i );
        this->read_elements( fp, swap, offset, i );
        this->read_variables( fp, swap, offset, i );
        this->read_variables_on_face( fp, swap, offset, i );
    }

    fclose( fp );

    // Initialize variables for importing
    m_importing_element_type = 0;
    m_importing_grid_index = m_ngrids;
    m_importing_variable_index = 0;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read data from binary type grid file
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readBinaryGrid( const std::string& filename )
{
    setFilename( filename );
    setSuccess( false );

    ::Format format( filename );
    if ( format.isBinary() )
    {
        return this->readBinaryGrid( filename, format.swap(), format.offset() );
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Read data from binary type grid file
 *  @param  filename [in] filename
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readBinaryGrid( const std::string& filename, bool swap, size_t offset )
{
    setFilename( filename );
    setSuccess( true );

    FILE* fp = fopen( filename.c_str(), "rb" );
    if ( !fp )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        setSuccess( false );
        return false;
    }

    this->read_magic( fp, swap, offset );
    this->read_title( fp, swap, offset );
    this->read_version( fp, swap, offset );
    this->read_grids( fp, swap, offset );
    this->read_boundary_condition( fp, swap, offset );

    const size_t ngrids = this->numberOfGrids();
    for ( size_t i = 0; i < ngrids; i++ )
    {
        if ( m_grids.size() <= i )
        {
            Grid grid;
            grid.nelements[0] = 0;
            grid.nelements[1] = 0;
            grid.nelements[2] = 0;
            grid.nelements[3] = 0;
            grid.nelements[4] = 0;
            m_grids.push_back( grid );
        }

        this->read_nnodes( fp, swap, offset, i );
        this->read_nodes( fp, swap, offset, i );
        this->read_faces( fp, swap, offset, i );
        this->read_elements( fp, swap, offset, i );
    }

    fclose( fp );

    // Initialize variables for importing
    m_importing_element_type = 0;
    m_importing_grid_index = m_ngrids;
    m_importing_variable_index = 0;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read data from binary type result file
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readBinaryResult( const std::string& filename )
{
    setFilename( filename );
    setSuccess( false );

    ::Format format( filename );
    if ( format.isBinary() )
    {
        return this->readBinaryResult( filename, format.swap(), format.offset() );
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Read data from binary type result file
 *  @param  filename [in] filename
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool FieldViewData::readBinaryResult( const std::string& filename, bool swap, size_t offset )
{
    setFilename( filename );
    setSuccess( true );

    FILE* fp = fopen( filename.c_str(), "rb" );
    if ( !fp )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        setSuccess( false );
        return false;
    }

    this->read_magic( fp, swap, offset );
    this->read_title( fp, swap, offset );
    this->read_version( fp, swap, offset );
    this->read_constants( fp, swap, offset );
    this->read_grids( fp, swap, offset );
    this->read_variable_names( fp, swap, offset );
    this->read_variable_names_on_face( fp, swap, offset );

    const size_t ngrids = this->numberOfGrids();
    for ( size_t i = 0; i < ngrids; i++ )
    {
        if ( m_grids.size() <= i )
        {
            Grid grid;
            grid.nelements[0] = 0;
            grid.nelements[1] = 0;
            grid.nelements[2] = 0;
            grid.nelements[3] = 0;
            grid.nelements[4] = 0;
            m_grids.push_back( grid );
        }

        this->read_nnodes( fp, swap, offset, i );
        this->read_variables( fp, swap, offset, i );
    }

    fclose( fp );

    // Initialize variables for importing
    m_importing_element_type = 0;
    m_importing_grid_index = m_ngrids;
    m_importing_variable_index = 0;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read version information from ascii file.
 *  @param  fp [in] file pointer
 */
/*===========================================================================*/
void FieldViewData::read_version( FILE* fp )
{
    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];
    int major;
    int minor;

    if ( fscanf( fp, "%s %d %d", buffer, &major, &minor ) != -1 )
    {
        m_version.major = major;
        m_version.minor = minor;
    }
}

/*===========================================================================*/
/**
 *  @brief  Read constant parameters from ascii file.
 *  @param  fp [in] file pointer
 */
/*===========================================================================*/
void FieldViewData::read_constants( FILE* fp )
{
    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];

    while ( fscanf( fp, "%s", buffer ) != -1 )
    {
        if ( kvs::String::ToUpper( std::string( buffer ) ) == "CONSTANTS" )
        {
            float time; fscanf( fp, "%f ", &time );
            float mach; fscanf( fp, "%f ", &mach );
            float alpha; fscanf( fp, "%f ", &alpha );
            float re; fscanf( fp, "%f ", &re );

            m_constant.time = time;
            m_constant.mach = mach;
            m_constant.alpha = alpha;
            m_constant.re = re;
            break;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read grid information from ascii file.
 *  @param  fp [in] file pointer
 */
/*===========================================================================*/
void FieldViewData::read_grids( FILE* fp )
{
    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];

    while ( fscanf( fp, "%s", buffer ) != -1 )
    {
        if ( kvs::String::ToUpper( std::string( buffer ) ) == "GRIDS" )
        {
            int ngrids; fscanf( fp, "%d ", &ngrids );
            m_ngrids = static_cast<size_t>( ngrids );
            break;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read variable names from ascii file.
 *  @param  fp [in] file pointer
 */
/*===========================================================================*/
void FieldViewData::read_variable_names( FILE* fp )
{
    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];

    while ( fscanf( fp, "%s", buffer ) != -1 )
    {
        if ( kvs::String::ToUpper( std::string( buffer ) ) == "VARIABLE" )
        {
            fscanf( fp, "%s", buffer );
            if ( kvs::String::ToUpper( std::string( buffer ) ) == "NAMES" )
            {
                int nvariables; fscanf( fp, "%d ", &nvariables );

                m_nvariables = static_cast<size_t>( nvariables );
                for ( size_t i = 0; i < m_nvariables; i++ )
                {
                    fgets( buffer, MaxLineLength, fp );
                    sscanf( buffer, "%s", buffer ); // remove '\n'
                    m_variable_names.push_back( std::string( buffer ) );
                }

                break;
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read node data from ascii file.
 *  @param  fp [in] file pointer
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_nodes( FILE* fp, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];

    while ( fscanf( fp, "%s", buffer ) != -1 )
    {
        if ( kvs::String::ToUpper( std::string( buffer ) ) == "NODES" )
        {
            int nnodes; fscanf( fp, "%d ", &nnodes );

            for ( int i = 0; i < nnodes; i++ )
            {
                Node node;
                fscanf( fp, "%f", &node.x );
                fscanf( fp, "%f", &node.y );
                fscanf( fp, "%f", &node.z );

                grid.nodes.push_back( node );
            }

            break;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read element data from ascii file.
 *  @param  fp [in] file pointer
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_elements( FILE* fp, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];

    // none-0, tet-4, hex-8, prism-6, pyramid-5
    const int NumberOfNodesPerElement[5] = { 0, 4, 8, 6, 5 };
    while ( fscanf( fp, "%s", buffer ) != -1 )
    {
        if ( kvs::String::ToUpper( std::string( buffer ) ) == "ELEMENTS" )
        {
            for ( ; ; )
            {
                fpos_t p; fgetpos( fp, &p );
                fscanf( fp, "%s", buffer );
                if ( isdigit( buffer[0] ) )
                {
                    Element element;

                    int type = atoi( buffer );
                    int subtype; fscanf( fp, "%d", &subtype );
                    int nnodes = NumberOfNodesPerElement[type];
                    element.type = type;
                    element.id.allocate( nnodes );
                    for ( int i = 0; i < nnodes; i++ )
                    {
                        int id; fscanf( fp, "%d", &id );
                        element.id[i] = id;
                    }

                    grid.nelements[0]++;
                    grid.nelements[type]++;
                    grid.elements.push_back( element );
                }
                else // "VARIABLES"
                {
                    fsetpos( fp, &p );
                    break;
                }
            }

            break;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read variable data from ascii file.
 *  @param  fp [in] file pointer
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_variables( FILE* fp, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    const size_t MaxLineLength = 256;
    char buffer[ MaxLineLength ];

    while ( fscanf( fp, "%s", buffer ) != -1 )
    {
        if ( kvs::String::ToUpper( std::string( buffer ) ) == "VARIABLES" )
        {
            for ( size_t i = 0; i < m_nvariables; i++ )
            {
                Variable variable;
                variable.data.allocate( grid.nodes.size() );
                for ( size_t j = 0; j < grid.nodes.size(); j++ )
                {
                    float data; fscanf( fp, "%f", &data );
                    variable.data[j] = data;
                }

                grid.variables.push_back( variable );
            }

            for ( size_t i = 0; i < m_nvariables; i++ )
            {
                bool is_vector = ( m_variable_names[i].find( ';', 0 ) != std::string::npos );
                if ( is_vector )
                {
                    grid.variables[i++].type = 3;
                    grid.variables[i++].type = 3;
                    grid.variables[i].type = 3;
                }
                else
                {
                    grid.variables[i].type = 1;
                }
            }

            break;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read magic number from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_magic( FILE* fp, bool swap, size_t offset )
{
    ::Record record( fp, swap, offset );
    int temp = record.readValue<int>();
    if ( temp != FV_MAGIC ) kvsMessageError( "Cannot find 'FV_MAGIC'." );
}

/*===========================================================================*/
/**
 *  @brief  Read header tile from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_title( FILE* fp, bool swap, size_t offset )
{
    ::Record record( fp, swap, offset );
    std::string temp = record.readString( 80 ); // "FIELDVIEW"
}

/*===========================================================================*/
/**
 *  @brief  Read version information from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_version( FILE* fp, bool swap, size_t offset )
{
    ::Record record( fp, swap, offset );
    kvs::ValueArray<int> temp = record.readValues<int>( 2 );
    m_version.major = temp[0];
    m_version.minor = temp[1];

    // In case that the version is composed of four numbers,
    int size = int( record.length() / sizeof(int) ) - 2;
    if ( size > 0 )
    {
        // The following integers are included in this record.
        //   temp[0]: File type code (new in version 2.7)
        //     - FV_GRIDS_FILE (1), FV_RESULTS_FILE (2), FV_COMBINED_FILE (3)
        //   temp[1]: Reserved field (new in version 2.6)
        //     - Always zero
        temp = record.readValues<int>( size );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read constant paramters from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for swap byte
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_constants( FILE* fp, bool swap, size_t offset )
{
    ::Record record( fp, swap, offset );
    kvs::ValueArray<float> temp = record.readValues<float>( 4 );
    m_constant.time = temp[0];
    m_constant.mach = temp[1];
    m_constant.alpha = temp[2];
    m_constant.re = temp[3];
}

/*===========================================================================*/
/**
 *  @brief  Read boundary conditions from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_boundary_condition( FILE* fp, bool swap, size_t offset )
{
    // Read number of boundary face types (m_boundary_condition.ntypes)
    {
        ::Record record( fp, swap, offset );
        int temp = record.readValue<int>();
        m_boundary_condition.ntypes = static_cast<size_t>( temp );
    }

    for ( size_t i = 0; i < m_boundary_condition.ntypes; i++ )
    {
        ::Record record( fp, swap, offset );
        kvs::ValueArray<int> values = record.readValues<int>( 2 );
        std::string name = record.readString( 80 );
        m_boundary_condition.results.push_back( values[0] );
        m_boundary_condition.normals.push_back( values[1] );
        m_boundary_condition.names.push_back( name );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read grid information from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_grids( FILE* fp, bool swap, size_t offset )
{
    ::Record record( fp, swap, offset );
    int ngrids = record.readValue<int>();
    m_ngrids = static_cast<size_t>( ngrids );
}

/*===========================================================================*/
/**
 *  @brief  Read variable names from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_variable_names( FILE* fp, bool swap, size_t offset )
{
    // Read number of variables (m_nvariables)
    {
        ::Record record( fp, swap, offset );
        int temp = record.readValue<int>();
        m_nvariables = static_cast<size_t>( temp );
    }

    for ( size_t i = 0; i < m_nvariables; i++ )
    {
        ::Record record( fp, swap, offset );
        std::string name = record.readString( 80 );
        m_variable_names.push_back( name );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read variable names on face from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 */
/*===========================================================================*/
void FieldViewData::read_variable_names_on_face( FILE* fp, bool swap, size_t offset )
{
    // Read number of variables on face (m_nvariables_on_face)
    {
        ::Record record( fp, swap, offset );
        int temp = record.readValue<int>();
        m_nvariables_on_face = static_cast<size_t>( temp );
    }

    for ( size_t i = 0; i < m_nvariables_on_face; i++ )
    {
        ::Record record( fp, swap, offset );
        std::string name = record.readString( 80 );
        m_variable_names_on_face.push_back( name );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read node information of a grid from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_nnodes( FILE* fp, bool swap, size_t offset, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    ::Record record( fp, swap, offset );
    kvs::ValueArray<int> temp = record.readValues<int>( 2 );
    if ( temp[0] != FV_NODES ) kvsMessageError( "Cannot find 'FV_NODES'." );

    grid.nnodes = static_cast<size_t>( temp[1] );
}

/*===========================================================================*/
/**
 *  @brief  Read node data of a grid from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_nodes( FILE* fp, bool swap, size_t offset, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    ::Record record( fp, swap, offset );
    kvs::ValueArray<float> xcoords = record.readValues<float>( grid.nnodes );
    kvs::ValueArray<float> ycoords = record.readValues<float>( grid.nnodes );
    kvs::ValueArray<float> zcoords = record.readValues<float>( grid.nnodes );

    for ( size_t i = 0; i < grid.nnodes; i++ )
    {
        Node node;
        node.x = xcoords[i];
        node.y = ycoords[i];
        node.z = zcoords[i];
        grid.nodes.push_back( node );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read face information of a grid from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_faces( FILE* fp, bool swap, size_t offset, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    while ( ::Record::PeekValue<int>( fp, swap, offset ) == FV_FACES )
    {
        int type = 0; // boundary type
        int nfaces = 0; // number of faces of this type
        {
            ::Record record( fp, swap, offset );
            kvs::ValueArray<int> temp = record.readValues<int>( 3 );
            if ( temp[0] != FV_FACES ) { kvsMessageError( "Cannot find 'FV_FACES'." ); }

            type = temp[1];
            nfaces = temp[2];
        }

        ::Record record( fp, swap, offset );
        kvs::ValueArray<int> id = record.readValues<int>( 4 * nfaces );

        Face face;
        face.type = type;
        face.id = id;
        grid.faces.push_back( face );
    }
}

/*===========================================================================*/
/**
 *  @brief  Read element information of a grid from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_elements( FILE* fp, bool swap, size_t offset, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    while ( ::Record::PeekValue<int>( fp, swap, offset ) == FV_ELEMENTS )
    {
        int total_count = 0;
        {
            ::Record record( fp, swap, offset );
            kvs::ValueArray<int> temp = record.readValues<int>( 5 );
            if ( temp[0] != FV_ELEMENTS ) kvsMessageError( "Cannot find 'FV_ELEMENTS'." );

            // Number of elements of each type
            //     temp[1]: tet count
            //     temp[2]: hex count
            //     temp[3]: prism count
            //     temp[4]: pyramid count
            total_count = temp[1] + temp[2] + temp[3] + temp[4];
        }

        ::Record record( fp, swap, offset );
        for ( int i = 0; i < total_count; i++ )
        {
            Element element;

            int header = record.readValue<int>();
            header = header >> 18;
            switch ( header )
            {
            case 1:
            {
                element.type = FieldViewData::Tet;
                element.id = record.readValues<int>( 4 );
                grid.nelements[0]++;
                grid.nelements[FieldViewData::Tet]++;
                break;
            }
            case 4:
            {
                element.type = FieldViewData::Hex;
                element.id = record.readValues<int>( 8 );
                grid.nelements[0]++;
                grid.nelements[FieldViewData::Hex]++;
                break;
            }
            case 3:
            {
                element.type = FieldViewData::Pri;
                element.id = record.readValues<int>( 6 );
                grid.nelements[0]++;
                grid.nelements[FieldViewData::Pri]++;
                break;
            }
            case 2:
            {
                element.type = FieldViewData::Pyr;
                element.id = record.readValues<int>( 5 );
                grid.nelements[0]++;
                grid.nelements[FieldViewData::Pyr]++;
                break;
            }
            default: break;
            }

            grid.elements.push_back( element );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read variable data of a grid from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @param  gindex [in] grid index
 */
/*===========================================================================*/
void FieldViewData::read_variables( FILE* fp, bool swap, size_t offset, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    {
        ::Record record( fp, swap, offset );
        int temp = record.readValue<int>();
        if ( temp != FV_VARIABLES ) kvsMessageError( "Cannot find 'FV_VARIABLES'." );
    }

    ::Record record( fp, swap, offset );
    for ( size_t i = 0; i < m_nvariables; i++ )
    {
        Variable variable;
        variable.data = record.readValues<float>( grid.nodes.size() );
        grid.variables.push_back( variable );
    }

    for ( size_t i = 0; i < m_nvariables; i++ )
    {
        bool is_vector = ( m_variable_names[i].find( ';', 0 ) != std::string::npos );
        if ( is_vector )
        {
            grid.variables[i++].type = 3;
            grid.variables[i++].type = 3;
            grid.variables[i].type = 3;
        }
        else
        {
            grid.variables[i].type = 1;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Read variable data on a face of a grid from binary file.
 *  @param  fp [in] file pointer
 *  @param  swap [in] flag for byte-swap
 *  @param  offset [in] offset byte
 *  @param  gindex [inde] grid index
 */
/*===========================================================================*/
void FieldViewData::read_variables_on_face( FILE* fp, bool swap, size_t offset, size_t gindex )
{
    Grid& grid = m_grids[ gindex ];

    size_t nvariables_on_face = m_nvariables_on_face;
    size_t nfaces = m_boundary_condition.ntypes;
    {
        ::Record record( fp, swap, offset );
        int temp = record.readValue<int>();
        if ( temp != FV_BNDRY_VARS ) kvsMessageError( "Cannot find 'FV_BNDRY_VARS'." );
    }

    if ( nvariables_on_face > 0 )
    {
        ::Record record( fp, swap, offset );
        for ( size_t i = 0; i < nvariables_on_face; i++ )
        {
            std::vector<float> data;
            for ( size_t j = 0; j < nfaces; j++ )
            {
                if ( m_boundary_condition.results[j] == 1 )
                {
                    float var = record.readValue<float>();
                    data.push_back( var );
                }
            }

            Variable variable;
            variable.data = kvs::ValueArray<float>( data );
            grid.variables_on_face.push_back( variable );
        }

        for ( size_t i = 0; i < m_nvariables_on_face; i++ )
        {
            bool is_vector = ( m_variable_names_on_face[i].find( ';', 0 ) != std::string::npos );
            if ( is_vector )
            {
                grid.variables_on_face[i++].type = 3;
                grid.variables_on_face[i++].type = 3;
                grid.variables_on_face[i].type = 3;
            }
            else
            {
                grid.variables_on_face[i].type = 1;
            }
        }
    }
}

} // end of namespace kvs
