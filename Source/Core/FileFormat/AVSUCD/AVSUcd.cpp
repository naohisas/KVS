/****************************************************************************/
/**
 *  @file AVSUcd.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: AVSUcd.cpp 1823 2014-12-12 08:44:28Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "AVSUcd.h"
#include <kvs/File>
#include <kvs/Message>
#include <kvs/ValueArray>
#include <kvs/IgnoreUnusedVariable>
#include <cstdlib>
#include <cstring>


namespace
{

const size_t MaxLineLength = 512;

const char* const Delimiter = " \n\r";

const std::string CycleTypeToString[ kvs::AVSUcd::CycleTypeSize ] =
{
    "unknown",
    "data",
    "geom",
    "data_geom"
};

const std::string ElementTypeToString[ kvs::AVSUcd::ElementTypeSize ] =
{
    "unknown",
    "point",
    "line",
    "triangle",
    "quadrangle",
    "tetrahedra",
    "tetrahedra2",
    "pyramid",
    "prism",
    "hexahedra",
    "hexahedra2"
};

}

namespace
{

kvs::AVSUcd::FormatType CheckFormatType( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    kvs::AVSUcd::FormatType format_type = kvs::AVSUcd::SingleStep;

    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '#' )
        {
            // Skip comment line.
            continue;
        }
        else
        {
            // Skip first token.
            strtok( buffer, ::Delimiter );

            const char* const second_token = strtok( 0, ::Delimiter );

            if ( second_token != 0 )
            {
                format_type = kvs::AVSUcd::SingleStep;
            }
            else
            {
                format_type = kvs::AVSUcd::MultiStep;
            }

            break;
        }
    }

    // Go back file-pointer to head.
    fseek( ifs, 0, SEEK_SET );

    return format_type;
}

bool IsControlFile( FILE* const ifs )
{
    fseek( ifs, 0, SEEK_SET );
    char buffer[ ::MaxLineLength ];

    bool result = false;
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        // Skip comment lines.
        if ( buffer[0] == '#' )
        {
            continue;
        }

        // In case of a control file, cycle type must describe in the first line.
        if ( strncmp( buffer, "data", 4 ) == 0 ||
             strncmp( buffer, "geom", 4 ) == 0 ||
             strncmp( buffer, "data_geom", 9 ) == 0 )
        {
            result = true;
        }
    }

    fseek( ifs, 0, SEEK_SET );
    return result;
}

int StepNumber( const std::string& filename )
{
    FILE* const ifs = fopen( filename.c_str(), "rb" );
    if ( !ifs )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return -1;
    }

    int result = -1;
    fseek( ifs, 81, SEEK_SET );
    fread( &result, sizeof(int), 1, ifs );
    fclose( ifs );

    return result;
}

}


namespace kvs
{

bool AVSUcd::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "ucd" || file.extension() == "UCD" ||
         file.extension() == "inp" || file.extension() == "INP" )
    {
        return true;
    }

    return false;
}

/*==========================================================================*/
/**
 *  Constructs a new empty AVSUcd.
 */
/*==========================================================================*/
AVSUcd::AVSUcd():
    FileFormatBase(),
    m_nsteps( 0 ),
    m_cycle_type( kvs::AVSUcd::CycleTypeUnknown ),
    m_element_type( kvs::AVSUcd::ElementTypeUnknown ),
    m_step_id( 0 ),
    m_step_comment( "" ),
    m_nnodes( 0 ),
    m_nelements( 0 ),
    m_nvalues_per_node( 0 ),
    m_ncomponents_per_node( 0 ),
    m_component_id( 0 )
{
}

/*==========================================================================*/
/**
 *  Constructs a new AVSUcd and reads the component of the step in the file.
 *
 *  @param filename     [in] Filename.
 *  @param step_id      [in] StepID.
 *  @param component_id [in] ComponentID.
 */
/*==========================================================================*/
AVSUcd::AVSUcd( const std::string& filename, const size_t step_id, const size_t component_id ):
    FileFormatBase(),
    m_nsteps( 0 ),
    m_cycle_type( kvs::AVSUcd::CycleTypeUnknown ),
    m_element_type( kvs::AVSUcd::ElementTypeUnknown ),
    m_step_id( step_id ),
    m_step_comment( "" ),
    m_nnodes( 0 ),
    m_nelements( 0 ),
    m_nvalues_per_node( 0 ),
    m_ncomponents_per_node( 0 ),
    m_component_id( component_id )
{
    this->read( filename );
}

/*==========================================================================*/
/**
 *  Destroys the AVSUcd.
 */
/*==========================================================================*/
AVSUcd::~AVSUcd()
{
}

size_t AVSUcd::nsteps() const
{
    return m_nsteps;
}

AVSUcd::CycleType AVSUcd::cycleType() const
{
    return m_cycle_type;
}

AVSUcd::ElementType AVSUcd::elementType() const
{
    return m_element_type;
}

size_t AVSUcd::stepID() const
{
    return m_step_id;
}

const std::string& AVSUcd::stepComment() const
{
    return m_step_comment;
}

size_t AVSUcd::nnodes() const
{
    return m_nnodes;
}

size_t AVSUcd::nelements() const
{
    return m_nelements;
}

size_t AVSUcd::nvaluesPerNode() const
{
    return m_nvalues_per_node;
}

size_t AVSUcd::ncomponentsPerNode() const
{
    return m_ncomponents_per_node;
}

const std::vector<size_t>& AVSUcd::veclens() const
{
    return m_veclens;
}

const std::vector<std::string>& AVSUcd::componentNames() const
{
    return m_component_names;
}

const std::vector<std::string>& AVSUcd::componentUnits() const
{
    return m_component_units;
}

size_t AVSUcd::componentID() const
{
    return m_component_id;
}

const std::string& AVSUcd::componentName() const
{
    return m_component_names[ m_component_id ];
}

const std::string& AVSUcd::componentUnit() const
{
    return m_component_units[ m_component_id ];
}

const AVSUcd::Coords& AVSUcd::coords() const
{
    return m_coords;
}

const AVSUcd::Connections& AVSUcd::connections() const
{
    return m_connections;
}

const AVSUcd::Values& AVSUcd::values() const
{
    return m_values;
}

void AVSUcd::setNSteps( const size_t nsteps )
{
    m_nsteps = nsteps;
}

void AVSUcd::setCycleType( const CycleType cycle_type )
{
    m_cycle_type = cycle_type;
}

void AVSUcd::setElementType( const ElementType element_type )
{
    m_element_type = element_type;
}

void AVSUcd::setStepID( const size_t step_id )
{
    m_step_id = step_id;
}

void AVSUcd::setStepComment( const std::string& step_comment )
{
    m_step_comment = step_comment;
}

void AVSUcd::setNNodes( const size_t nnodes )
{
    m_nnodes = nnodes;
}

void AVSUcd::setNElements( const size_t nelements )
{
    m_nelements = nelements;
}

void AVSUcd::setNValuesPerNode( const size_t nvalues_per_node )
{
    m_nvalues_per_node = nvalues_per_node;
}

void AVSUcd::setNComponentsPerNode( const size_t ncomponents_per_node )
{
    m_ncomponents_per_node = ncomponents_per_node;
}

void AVSUcd::setVeclens( const std::vector<size_t>& veclens )
{
    m_veclens = veclens;
}

void AVSUcd::setComponentNames( const std::vector<std::string>& component_names )
{
    m_component_names.clear();

    const size_t size = component_names.size();
    for ( size_t i = 0; i < size; ++i )
    {
        m_component_names.push_back( component_names[ i ] );
    }
}

void AVSUcd::setComponentUnits( const std::vector<std::string>& component_units )
{
    m_component_units.clear();

    const size_t size = component_units.size();
    for ( size_t i = 0; i < size; ++i )
    {
        m_component_units.push_back( component_units[ i ] );
    }
}

void AVSUcd::setComponentID( const size_t component_id )
{
    m_component_id = component_id;
}

void AVSUcd::setCoords( const Coords& coords )
{
    m_coords = coords;
}

void AVSUcd::setConnections( const Connections& connections )
{
    m_connections = connections;
}

void AVSUcd::setValues( const Values& values )
{
    m_values = values;
}

void AVSUcd::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Number of steps : " << m_nsteps << std::endl;
    os << indent << "Cycle type : " << ::CycleTypeToString[m_cycle_type] << std::endl;
    os << indent << "Element type : " << ::ElementTypeToString[m_element_type] << std::endl;
    os << indent << "Step ID : " << m_step_id << std::endl;
    os << indent << "Step comment : " << m_step_comment << std::endl;
    os << indent << "Number of nodes : " << m_nnodes << std::endl;
    os << indent << "Number of elements : " << m_nelements << std::endl;
    os << indent << "Number of values per node : " << m_nvalues_per_node << std::endl;
    os << indent << "Number of components per node : " << m_ncomponents_per_node << std::endl;

    os << indent << "Veclens of each component : ";
    for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
    {
        os << m_veclens[ i ] << " ";
    }
    os << std::endl;

    os << indent << "Names of each component : ";
    for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
    {
        os << m_component_names[ i ] << " ";
    }
    os << std::endl;

    os << indent << "Units of each component : ";
    for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
    {
        os << m_component_units[ i ] << " ";
    }
    os << std::endl;

    os << indent << "Component ID : " << m_component_id << std::endl;
}

/*==========================================================================*/
/**
 *  Reads AVS UCD data from the file.
 *
 *  @param filename [in] Filename.
 */
/*==========================================================================*/
bool AVSUcd::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    FILE* const ifs = fopen( filename.c_str(), "rb" );
    if ( !ifs )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    try
    {
        if ( ::IsControlFile( ifs ) )
        {
            this->read_control_file( ifs );
        }
        else
        {
            const FormatType format_type = ::CheckFormatType( ifs );
            if ( format_type == SingleStep )
            {
                this->read_single_step_format( ifs );
            }
            else if ( format_type == MultiStep )
            {
                this->read_multi_step_format( ifs );
            }
        }
    }
    catch ( const char* const error )
    {
        kvsMessageError( "%s: %s", filename.c_str(), error );
        BaseClass::setSuccess( false );
        return false;
    }

    fclose( ifs );

    return true;
}

/*==========================================================================*/
/**
 *  Writes AVS UCD data to the file.
 *
 *  @param filename [in] Filename.
 */
/*==========================================================================*/
bool AVSUcd::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    FILE* const ofs = fopen( filename.c_str(), "wb" );
    if ( !ofs )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    try
    {
        this->write_single_step_format( ofs );
    }
    catch ( const char* const error )
    {
        kvsMessageError( "%s: %s", filename.c_str(), error );
        BaseClass::setSuccess( false );
        return false;
    }

    fclose( ofs );

    return true;
}

void AVSUcd::read_control_file( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        // Skip comment lines.
        if ( buffer[0] == '#' )
        {
            continue;
        }

        // In case of a control file, cycle type must describe in the first line.
        if ( strncmp( buffer, "data_geom", 9 ) == 0 )
        {
            m_cycle_type = DataGeom;
        }
        else if ( strncmp( buffer, "data", 4 ) == 0 )
        {
            m_cycle_type = Data;
        }
        else if ( strncmp( buffer, "geom", 4 ) == 0 )
        {
            m_cycle_type = Geom;
        }
        else
        {
            m_cycle_type = CycleTypeUnknown;
        }
        break;
    }

    const std::string path = kvs::File( BaseClass::filename() ).pathName( true );
    std::vector<std::string> filenames;
    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '#' ) continue;

        size_t last = strlen(buffer) - 1;
        if ( buffer[last] == '\n' ) buffer[last] = '\0';

        const std::string filename = path + kvs::File::Separator() + std::string( buffer );
        filenames.push_back( filename );
    }

    m_nsteps = filenames.size();
    this->read_binary_files( filenames );
}

void AVSUcd::read_binary_files( const std::vector<std::string>& filenames )
{
    for ( size_t i = 0; i < filenames.size(); i++ )
    {
        const std::string& filename = filenames[i];
        const int step_id = ::StepNumber( filename ) - 1;
        if ( step_id == static_cast<int>( m_step_id ) )
        {
            this->read_binary_file( filename );
            return;
        }
    }
}

void AVSUcd::read_binary_file( const std::string& filename )
{
    FILE* const ifs = fopen( filename.c_str(), "rb" );
    if ( !ifs )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return;
    }

    // File information.
    char keyword[8] = {'\0'};
    fread( keyword, 7, 1, ifs );
    KVS_ASSERT( std::string( keyword ) == "AVS UCD" || std::string( keyword ) == "AVSUC64" );

    float version = 0.0f;
    fread( &version, 4, 1, ifs );
    KVS_ASSERT( version == 1.0f );

    // Step information.
    char title[71] = {'\0'};
    fread( title, 70, 1, ifs );

    int step_number = 0;
    fread( &step_number, 4, 1, ifs );
    KVS_ASSERT( step_number > 0 );

    float step_time = 0.0f;
    fread( &step_time, 4, 1, ifs );

    // Node information.
    if ( std::string( keyword ) == "AVS UCD" )
    {
        // 32bit data.
        int nnodes = 0;
        fread( &nnodes, 4, 1, ifs );
        m_nnodes = static_cast<size_t>( nnodes );
    }
    else if ( std::string( keyword ) == "AVSUC64" )
    {
        // 64bit data.
        long nnodes = 0;
        fread( &nnodes, 8, 1, ifs );
        m_nnodes = static_cast<size_t>( nnodes );
    }

    int description_type = 0;
    fread( &description_type, 4, 1, ifs );

    m_coords.allocate( m_nnodes * 3 );
    kvs::Real32* pcoords = m_coords.data();
    if ( description_type == 1 )
    {
        if ( std::string( keyword ) == "AVS UCD" )
        {
            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                int node_number = 0; // 1, 2, 3, ...
                fread( &node_number, 4, 1, ifs );

                int node_id = node_number - 1; // 0, 1, 2, ...
                fread( pcoords + 3 * node_id, 4, 3, ifs );
            }
        }
        else if ( std::string( keyword ) == "AVSUC64" )
        {
            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                long node_number = 0; // 1, 2, 3, ...
                fread( &node_number, 8, 1, ifs );

                long node_id = node_number - 1; // 0, 1, 2, ...
                fread( pcoords + 3 * node_id, 4, 3, ifs );
            }
        }
    }
    else if ( description_type == 2 )
    {
        if ( std::string( keyword ) == "AVS UCD" )
        {
            kvs::ValueArray<int> node_ids( m_nnodes );
            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                int node_number = 0;
                fread( &node_number, 4, 1, ifs );

                int node_id = node_number - 1;
                node_ids[i] = node_id;
            }

            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                int node_id = node_ids[i];
                fread( pcoords + 3 * node_id, 4, 1, ifs );
            }

            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                int node_id = node_ids[i];
                fread( pcoords + 3 * node_id + 1, 4, 1, ifs );
            }

            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                int node_id = node_ids[i];
                fread( pcoords + 3 * node_id + 2, 4, 1, ifs );
            }
        }
        else if ( std::string( keyword ) == "AVSUC64" )
        {
            kvs::ValueArray<long> node_ids( m_nnodes );
            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                long node_number = 0;
                fread( &node_number, 8, 1, ifs );

                long node_id = node_number - 1;
                node_ids[i] = node_id;
            }

            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                long node_id = node_ids[i];
                fread( pcoords + 3 * node_id, 4, 1, ifs );
            }

            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                long node_id = node_ids[i];
                fread( pcoords + 3 * node_id + 1, 4, 1, ifs );
            }

            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                long node_id = node_ids[i];
                fread( pcoords + 3 * node_id + 2, 4, 1, ifs );
            }
        }
    }

    // Element information.
    if ( std::string( keyword ) == "AVS UCD" )
    {
        // 32bit data.
        int nelements = 0;
        fread( &nelements, 4, 1, ifs );
        m_nelements = static_cast<size_t>( nelements );

        kvs::ValueArray<int> element_ids( m_nelements );
        for ( size_t i = 0; i < m_nelements; i++ )
        {
            int element_number = 0;
            fread( &element_number, 4, 1, ifs );

            int element_id = element_number - 1;
            element_ids[i] = element_id;
        }

        // Material numbers.
        fseek( ifs, m_nelements * 4, SEEK_CUR );

        char element_type;
        fread( &element_type, 1, 1, ifs );
        fseek( ifs, m_nelements - 1, SEEK_CUR );

        size_t nnodes_per_element = 0;
        if ( element_type == 0 ) { m_element_type = Point; nnodes_per_element = 1; }
        else if ( element_type == 4 ) { m_element_type = Tetrahedra; nnodes_per_element = 4; }
        else if ( element_type == 11 ) { m_element_type = Tetrahedra2; nnodes_per_element = 10; }
        else if ( element_type == 7 ) { m_element_type = Hexahedra; nnodes_per_element = 8; }
        else if ( element_type == 14 ) { m_element_type = Hexahedra2; nnodes_per_element = 20; }
        else if ( element_type == 5 ) { m_element_type = Pyramid; nnodes_per_element = 5; }
        else if ( element_type == 6 ) { m_element_type = Prism; nnodes_per_element = 6; }
        else m_element_type = ElementTypeUnknown;

        m_connections.allocate( m_nelements * nnodes_per_element );
        kvs::UInt32* pconnections = m_connections.data();
        for ( size_t i = 0; i < m_nelements; i++ )
        {
            int element_id = element_ids[i];
            kvs::UInt32* target = pconnections + nnodes_per_element * element_id;
            for ( size_t j = 0; j < nnodes_per_element; j++ )
            {
                int connection_number = 0;
                fread( &connection_number, 4, 1, ifs );

                int connection_id = connection_number - 1;
                *(target++) = static_cast<kvs::UInt32>( connection_id );
            }
        }
    }
    else if ( std::string( keyword ) == "AVSUC64" )
    {
        // 64bit data.
        long nelements = 0;
        fread( &nelements, 8, 1, ifs );
        m_nelements = static_cast<size_t>( nelements );

        kvs::ValueArray<long> element_ids( m_nelements );
        for ( size_t i = 0; i < m_nelements; i++ )
        {
            long element_number = 0;
            fread( &element_number, 8, 1, ifs );

            long element_id = element_number - 1;
            element_ids[i] = element_id;
        }

        // Material numbers.
        fseek( ifs, m_nelements * 4, SEEK_CUR );

        char element_type;
        fread( &element_type, 1, 1, ifs );
        fseek( ifs, m_nelements - 1, SEEK_CUR );

        size_t nnodes_per_element = 0;
        if ( element_type == 0 ) { m_element_type = Point; nnodes_per_element = 1; }
        else if ( element_type == 4 ) { m_element_type = Tetrahedra; nnodes_per_element = 4; }
        else if ( element_type == 11 ) { m_element_type = Tetrahedra2; nnodes_per_element = 10; }
        else if ( element_type == 7 ) { m_element_type = Hexahedra; nnodes_per_element = 8; }
        else if ( element_type == 14 ) { m_element_type = Hexahedra2; nnodes_per_element = 20; }
        else if ( element_type == 5 ) { m_element_type = Pyramid; nnodes_per_element = 5; }
        else if ( element_type == 6 ) { m_element_type = Prism; nnodes_per_element = 6; }
        else m_element_type = ElementTypeUnknown;

        m_connections.allocate( m_nelements * nnodes_per_element );
        kvs::UInt32* pconnections = m_connections.data();
        for ( size_t i = 0; i < m_nelements; i++ )
        {
            long element_id = element_ids[i];
            kvs::UInt32* target = pconnections + nnodes_per_element * element_id;
            for ( size_t j = 0; j < nnodes_per_element; j++ )
            {
                long connection_number = 0;
                fread( &connection_number, 8, 1, ifs );

                long connection_id = connection_number - 1;
                *(target++) = static_cast<kvs::UInt32>( connection_id );
            }
        }
    }

    // Node data.
    int ncomponents = 0;
    fread( &ncomponents, 4, 1, ifs );
    m_ncomponents_per_node = static_cast<size_t>( ncomponents );

    if ( ncomponents > 0 )
    {
        int data_type = 0; // 1, 2, 3, or 4
        fread( &data_type, 4, 1, ifs );

        if ( data_type == 1 )
        {
            for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
            {
                char component_name[17] = {'\0'};
                fread( component_name, 16, 1, ifs );

                char unit[17] = {'\0'};
                fread( unit, 16, 1, ifs );

                int veclen = 0;
                fread( &veclen, 4, 1, ifs );

                int null_data_flag = 0;
                fread( &null_data_flag, 4, 1, ifs );

                float null_data = 0.0f;
                fread( &null_data, 4, 1, ifs );

                m_component_names.push_back( std::string( component_name ) );
                m_component_units.push_back( std::string( unit ) );
                m_veclens.push_back( static_cast<size_t>( veclen ) );
            }

            m_nvalues_per_node = 0;
            for ( size_t i = 0; i < m_ncomponents_per_node; i++ )
            {
                m_nvalues_per_node += m_veclens[i];
            }

            size_t veclen = m_veclens[ m_component_id ];
            size_t offset = 0; for ( size_t i = 0; i < m_component_id; i++ ) { offset += m_veclens[i]; }
            float* one_node = new float [ m_nvalues_per_node ];
            m_values.allocate( veclen * m_nnodes );
            kvs::Real32* pvalues = m_values.data();
            for ( size_t i = 0; i < m_nnodes; i++ )
            {
                fread( one_node, 4, m_nvalues_per_node, ifs );
                float* v = one_node + offset;
                for ( size_t j = 0; j < veclen; j++ )
                {
                    *(pvalues++) = *(v++);
                }
            }
            delete [] one_node;
        }
        else
        {
            kvsMessageError("Not supported format.");
        }
    }

    // Element data. (Currently ignored)

    fclose( ifs );
}

void AVSUcd::read_single_step_format( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    m_nsteps     = 1;
    m_cycle_type = DataGeom;

    m_step_id      = 0;
    m_step_comment = "";

    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '#' )
        {
            // Skip header comment line.
            continue;
        }
        else
        {
            m_nnodes           = atoi( strtok( buffer, ::Delimiter ) );
            m_nelements        = atoi( strtok( 0,      ::Delimiter ) );
            m_nvalues_per_node = atoi( strtok( 0,      ::Delimiter ) );

            this->read_coords( ifs );
            this->read_connections( ifs );
            this->read_components( ifs );
            this->read_values( ifs );

            return;
        }
    }

    throw "Unexpected EOF in skipping header comments.";
}

void AVSUcd::read_multi_step_format( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        if ( buffer[0] == '#' )
        {
            // Skip comment line.
            continue;
        }
        else
        {
            m_nsteps = atoi( strtok( buffer, ::Delimiter ) );

            if ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
            {
                const char* const cycle_type = strtok( buffer, ::Delimiter );

                m_cycle_type =
                    !strncmp( cycle_type, "data", 4 )      ? Data     :
                    !strncmp( cycle_type, "geom", 4 )      ? Geom     :
                    !strncmp( cycle_type, "data_geom", 9 ) ? DataGeom : CycleTypeUnknown;

                if ( m_nsteps == 1 )
                {
                    // In case of 'm_nsteps == 1', the data is read as 'data_geom'
                    // despite the specified cycle type.
                    this->read_multi_step_format_data_geom( ifs );
                }
                else
                {
                    if ( m_cycle_type == Data )
                    {
                        throw "Cycle type \"data\" is not supported.";
                        //this->read_multi_step_format_data( ifs );
                    }
                    else if ( m_cycle_type == Geom )
                    {
                        throw "Cycle type \"geom\" is not supported.";
                        //this->read_multi_step_format_geom( ifs );
                    }
                    else if ( m_cycle_type == DataGeom )
                    {
                        this->read_multi_step_format_data_geom( ifs );
                    }
                    else
                    {
                        throw "Unknown cycle type.";
                    }
                }
            }

            return;
        }
    }

    throw "Unexpected EOF in skipping header comments.";
}

void AVSUcd::read_multi_step_format_data( FILE* const )
{
}

void AVSUcd::read_multi_step_format_geom( FILE* const )
{
}

void AVSUcd::read_multi_step_format_data_geom( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    this->seek_target_step( ifs );

    if ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        m_nnodes    = atoi( strtok( buffer, ::Delimiter ) );
        m_nelements = atoi( strtok( 0,   ::Delimiter ) );
    }
    else
    {
        throw "Unexpected EOF in reading nnodes and nelements.";
    }

    this->read_coords( ifs );
    this->read_connections( ifs );

    if ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        m_nvalues_per_node = atoi( strtok( buffer, ::Delimiter ) );
    }
    else
    {
        throw "Unexpected EOF in reading nvalues per node.";
    }

    this->read_components( ifs );
    this->read_values( ifs );
}

void AVSUcd::seek_target_step( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    char target_step[ ::MaxLineLength ];
    sprintf( target_step, "step%u", static_cast<unsigned int>( m_step_id + 1 ) );

    while ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        const char* const first_token = strtok( buffer, ::Delimiter );
        if ( strcmp( first_token, target_step ) == 0 )
        {
            const char* const step_comment = strtok( 0, ::Delimiter );
            if ( step_comment != 0 )
            {
                m_step_comment = std::string( step_comment );
            }
            else
            {
                m_step_comment = std::string( "" );
            }

            return;
        }
    }

    throw "Unexpected EOF in seeking target step.";
}

void AVSUcd::read_coords( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    m_coords.allocate( 3 * m_nnodes );

    kvs::Real32* coord = m_coords.data();

    for ( size_t i = 0; i < m_nnodes; i++ )
    {
        if ( fgets( buffer, ::MaxLineLength, ifs ) )
        {
            // Node index.
            const int index = atoi( strtok( buffer, ::Delimiter ) ) - 1;

            coord[ index * 3 + 0 ] = static_cast<float>( atof( strtok( 0, ::Delimiter ) ) );
            coord[ index * 3 + 1 ] = static_cast<float>( atof( strtok( 0, ::Delimiter ) ) );
            coord[ index * 3 + 2 ] = static_cast<float>( atof( strtok( 0, ::Delimiter ) ) );
        }
    }
}

void AVSUcd::read_connections( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    // Read first line.
    if ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        strtok( buffer, ::Delimiter ); // Skip element index.
        strtok( 0, ::Delimiter ); // Skip material index.

        const char* const element_type = strtok( 0, ::Delimiter );

        m_element_type =
            !strcmp( element_type, "pt"  ) ? Point :
            !strcmp( element_type, "tet"  ) ? Tetrahedra :
            !strcmp( element_type, "tet2" ) ? Tetrahedra2 :
            !strcmp( element_type, "hex2" ) ? Hexahedra2 :
            !strcmp( element_type, "hex" ) ? Hexahedra :
            !strcmp( element_type, "pyr" ) ? Pyramid :
            !strcmp( element_type, "prism" ) ? Prism : ElementTypeUnknown;

        if ( m_element_type == Point )
        {
            m_connections.allocate( 1 * m_nelements );
        }
        else if ( m_element_type == Tetrahedra )
        {
            m_connections.allocate( 4 * m_nelements );
        }
        else if ( m_element_type == Tetrahedra2 )
        {
            m_connections.allocate( 10 * m_nelements );
        }
        else if ( m_element_type == Hexahedra )
        {
            m_connections.allocate( 8 * m_nelements );
        }
        else if ( m_element_type == Hexahedra2 )
        {
            m_connections.allocate( 20 * m_nelements );
        }
        else if ( m_element_type == Pyramid )
        {
            m_connections.allocate( 5 * m_nelements );
        }
        else if ( m_element_type == Prism )
        {
            m_connections.allocate( 6 * m_nelements );
        }
        else
        {
            throw "Unknown element type.";
        }
    }
    else
    {
        throw "Unexpected EOF in reading first line of connections.";
    }

    kvs::UInt32*       connection = m_connections.data();
    kvs::UInt32* const end        = connection + m_connections.size();

    if ( m_element_type == Point )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // skip element index.
                strtok( buffer, ::Delimiter );

                // skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "pt" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else if ( m_element_type == Tetrahedra )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // skip element index.
                strtok( buffer, ::Delimiter );

                // skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "tet" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else if ( m_element_type == Tetrahedra2 )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // skip element index.
                strtok( buffer, ::Delimiter );

                // skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "tet2" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else if ( m_element_type == Hexahedra )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // Skip element index.
                strtok( buffer, ::Delimiter );

                // Skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "hex" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else if ( m_element_type == Hexahedra2 )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // Skip element index.
                strtok( buffer, ::Delimiter );

                // Skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "hex2" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else if ( m_element_type == Pyramid )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // skip element index.
                strtok( buffer, ::Delimiter );

                // skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "pyr" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else if ( m_element_type == Prism )
    {
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
        *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;

        while ( connection < end )
        {
            if ( fgets( buffer, ::MaxLineLength, ifs ) )
            {
                // skip element index.
                strtok( buffer, ::Delimiter );

                // skip material index.
                strtok( 0, ::Delimiter );

                const char* const element_type = strtok( 0, ::Delimiter );
                if ( strcmp( element_type, "prism" ) != 0 )
                {
                    throw "Multi-element type is not supported.";
                }

                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
                *( connection++ ) = atoi( strtok( 0, ::Delimiter ) ) - 1;
            }
        }
    }
    else
    {
        throw "Unknown element type.";
    }
}

void AVSUcd::read_components( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    if ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
    {
        m_ncomponents_per_node = atoi( strtok( buffer, ::Delimiter ) );

        for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
        {
            m_veclens.push_back( atoi( strtok( 0, ::Delimiter ) ) );
        }
    }
    else
    {
        throw "Unexpected EOF in reading ncomponents per node";
    }

    for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
    {
        if ( fgets( buffer, ::MaxLineLength, ifs ) != 0 )
        {
            const char* const component_name = strtok( buffer, ",\n\r" );
            if ( component_name != 0 )
            {
                m_component_names.push_back( std::string( component_name ) );
            }
            else
            {
                m_component_names.push_back( "" );
            }

            const char* const component_unit = strtok( 0, ",\n\r" );
            if ( component_unit != 0 )
            {
                m_component_units.push_back( std::string( component_unit ) );
            }
            else
            {
                m_component_units.push_back( "" );
            }
        }
        else
        {
            throw "Unexpected EOF in reading component name and unit";
        }
    }
}

void AVSUcd::read_values( FILE* const ifs )
{
    char buffer[ ::MaxLineLength ];

    const size_t veclen = m_veclens[ m_component_id ];
    m_values.allocate( veclen * m_nnodes );

    kvs::Real32* value = m_values.data();

    size_t nskips = 0;
    for ( size_t i = 0; i < m_component_id; ++i )
    {
        nskips += m_veclens[ i ];
    }

    for ( size_t i = 0; i < m_nnodes; i++ )
    {
        if ( fgets( buffer, ::MaxLineLength, ifs ) )
        {
            // Node index
            const int index = atoi( strtok( buffer, ::Delimiter ) ) - 1;

            // Skip other components
            for ( size_t j = 0; j < nskips; ++j )
            {
                strtok( 0, ::Delimiter );
            }

            for ( size_t j = 0; j < veclen; ++j )
            {
                value[ index * veclen + j ] = static_cast<kvs::Real32>( atof( strtok( 0, ::Delimiter ) ) );
            }
        }
    }
}

void AVSUcd::write_single_step_format( FILE* const ofs ) const
{
    fprintf( ofs, "%u %u %u 0 0\n",
             static_cast<unsigned int>( m_nnodes ),
             static_cast<unsigned int>( m_nelements ),
             static_cast<unsigned int>( m_nvalues_per_node ) );

    this->write_coords( ofs );
    this->write_connections( ofs );
    this->write_components( ofs );
    this->write_values( ofs );
}

void AVSUcd::write_coords( FILE* const ofs ) const
{
    size_t node_id = 1;

    const kvs::Real32*       coord = m_coords.data();
    const kvs::Real32* const end   = coord + m_coords.size();

    while ( coord < end )
    {
        const kvs::Real32 x = *( coord++ );
        const kvs::Real32 y = *( coord++ );
        const kvs::Real32 z = *( coord++ );

        fprintf( ofs, "%u %f %f %f\n",
                 static_cast<unsigned int>( node_id++ ),
                 x,
                 y,
                 z );
    }
}

void AVSUcd::write_connections( FILE* const ofs ) const
{
    size_t element_id = 1;

    const kvs::UInt32*       connection = m_connections.data();
    const kvs::UInt32* const end        = connection + m_connections.size();

    if ( m_element_type == Point )
    {
        while ( connection < end )
        {
            const kvs::UInt32 node_id0 = *( connection++ ) + 1;

            fprintf( ofs, "%u 0 pt %u\n",
                     static_cast<unsigned int>( element_id++ ),
                     node_id0 );
        }
    }
    else if ( m_element_type == Tetrahedra )
    {
        while ( connection < end )
        {
            const kvs::UInt32 node_id0 = *( connection++ ) + 1;
            const kvs::UInt32 node_id1 = *( connection++ ) + 1;
            const kvs::UInt32 node_id2 = *( connection++ ) + 1;
            const kvs::UInt32 node_id3 = *( connection++ ) + 1;

            fprintf( ofs, "%u 0 tet %u %u %u %u\n",
                     static_cast<unsigned int>( element_id++ ),
                     node_id0,
                     node_id1,
                     node_id2,
                     node_id3 );
        }
    }
    else if ( m_element_type == Tetrahedra2 )
    {
        while ( connection < end )
        {
            const kvs::UInt32 node_id0  = *( connection++ ) + 1;
            const kvs::UInt32 node_id1  = *( connection++ ) + 1;
            const kvs::UInt32 node_id2  = *( connection++ ) + 1;
            const kvs::UInt32 node_id3  = *( connection++ ) + 1;
            const kvs::UInt32 node_id4  = *( connection++ ) + 1;
            const kvs::UInt32 node_id5  = *( connection++ ) + 1;
            const kvs::UInt32 node_id6  = *( connection++ ) + 1;
            const kvs::UInt32 node_id7  = *( connection++ ) + 1;
            const kvs::UInt32 node_id8  = *( connection++ ) + 1;
            const kvs::UInt32 node_id9  = *( connection++ ) + 1;

            fprintf( ofs, "%u 0 tet %u %u %u %u %u %u %u %u %u %u\n",
                     static_cast<unsigned int>( element_id++ ),
                     node_id0,
                     node_id1,
                     node_id2,
                     node_id3,
                     node_id4,
                     node_id5,
                     node_id6,
                     node_id7,
                     node_id8,
                     node_id9 );
        }
    }
    else if ( m_element_type == Hexahedra )
    {
        while ( connection < end )
        {
            const kvs::UInt32 node_id0 = *( connection++ ) + 1;
            const kvs::UInt32 node_id1 = *( connection++ ) + 1;
            const kvs::UInt32 node_id2 = *( connection++ ) + 1;
            const kvs::UInt32 node_id3 = *( connection++ ) + 1;
            const kvs::UInt32 node_id4 = *( connection++ ) + 1;
            const kvs::UInt32 node_id5 = *( connection++ ) + 1;
            const kvs::UInt32 node_id6 = *( connection++ ) + 1;
            const kvs::UInt32 node_id7 = *( connection++ ) + 1;

            fprintf( ofs, "%u 0 hex %u %u %u %u %u %u %u %u\n",
                     static_cast<unsigned int>( element_id++ ),
                     node_id0,
                     node_id1,
                     node_id2,
                     node_id3,
                     node_id4,
                     node_id5,
                     node_id6,
                     node_id7 );
        }
    }
    else if ( m_element_type == Hexahedra2 )
    {
        while ( connection < end )
        {
            fprintf( ofs, "%u 0 hex2", static_cast<unsigned int>( element_id++ ) );

            const size_t nnodes = 20;
            for ( size_t i = 0; i < nnodes; i++ )
            {
                const kvs::UInt32 node_id = *( connection++ ) + 1;
                fprintf( ofs, " %u", node_id );
            }

            fprintf( ofs, "\n" );
        }
    }
    else if ( m_element_type == Pyramid )
    {
        while ( connection < end )
        {
            const kvs::UInt32 node_id0 = *( connection++ ) + 1;
            const kvs::UInt32 node_id1 = *( connection++ ) + 1;
            const kvs::UInt32 node_id2 = *( connection++ ) + 1;
            const kvs::UInt32 node_id3 = *( connection++ ) + 1;
            const kvs::UInt32 node_id4 = *( connection++ ) + 1;

            fprintf( ofs, "%u 0 tet %u %u %u %u %u\n",
                     static_cast<unsigned int>( element_id++ ),
                     node_id0,
                     node_id1,
                     node_id2,
                     node_id3,
                     node_id4 );
        }
    }
    else if ( m_element_type == Prism )
    {
        while ( connection < end )
        {
            const kvs::UInt32 node_id0 = *( connection++ ) + 1;
            const kvs::UInt32 node_id1 = *( connection++ ) + 1;
            const kvs::UInt32 node_id2 = *( connection++ ) + 1;
            const kvs::UInt32 node_id3 = *( connection++ ) + 1;
            const kvs::UInt32 node_id4 = *( connection++ ) + 1;
            const kvs::UInt32 node_id5 = *( connection++ ) + 1;

            fprintf( ofs, "%u 0 tet %u %u %u %u %u %u\n",
                     static_cast<unsigned int>( element_id++ ),
                     node_id0,
                     node_id1,
                     node_id2,
                     node_id3,
                     node_id4,
                     node_id5 );
        }
    }
    else
    {
        throw "Unknown element type.";
    }
}

void AVSUcd::write_components( FILE* ofs ) const
{
    fprintf( ofs, "%u", static_cast<unsigned int>( m_ncomponents_per_node ) );
    for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
    {
        fprintf( ofs, " %u", static_cast<unsigned int>( m_veclens[ i ] ) );
    }
    fprintf( ofs, "\n" );

    for ( size_t i = 0; i < m_ncomponents_per_node; ++i )
    {
        fprintf( ofs, "%s,%s\n", m_component_names[ i ].c_str(), m_component_units[ i ].c_str() );
    }
}

void AVSUcd::write_values( FILE* ofs ) const
{
    size_t node_id = 1;

    const kvs::Real32*       value = m_values.data();
    const kvs::Real32* const end   = value + m_values.size();

    while ( value < end )
    {
        fprintf( ofs, "%u %f\n", static_cast<unsigned int>( node_id++ ), *( value++ ) );
    }
}

} // end of namespace kvs
