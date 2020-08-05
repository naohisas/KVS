/*****************************************************************************/
/**
 *  @file   TetConv.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Tetrahedral volume data converter
 */
/*****************************************************************************/
#include "TetConv.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/UnstructuredVolumeExporter>


namespace kvsconv
{

namespace TetConv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a tet2tet.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, TetConv::CommandName )
{
    addOption( TetConv::CommandName, TetConv::Description, 0 );
    addOption( "e", "External data file. (optional)", 0, false );
    addOption( "b", "Data file as binary. (optional)", 0, false );
    addOption( "m", "Conversion method. (default: 0)\n"
               "\t      0 = Subdivide a quadratic tetrahedron into eight tetrahedra\n"
               "\t      2 = Remove quadratic nodes", 1, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a input filename.
 *  @return input filename
 */
/*===========================================================================*/
std::string Argument::inputFilename()
{
    return this->value<std::string>();
}

/*===========================================================================*/
/**
 *  @brief  Returns a output filename.
 *  @param  filename [in] input filename
 *  @return output filename.
 */
/*===========================================================================*/
std::string Argument::outputFilename( const std::string& filename )
{
    if ( this->hasOption("output") )
    {
        return this->optionValue<std::string>("output");
    }
    else
    {
        // Replace the extension as follows: xxxx.inp -> xxx.kvsml.
        const std::string basename = kvs::File( filename ).baseName();
        const std::string extension = "kvsml";
        return basename + "." + extension;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type (ascii/external ascii/external binary).
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLUnstructuredVolumeObject::WritingDataType Argument::writingDataType()
{
    if ( this->hasOption("b") )
    {
        return kvs::KVSMLUnstructuredVolumeObject::ExternalBinary;
    }
    else
    {
        if ( this->hasOption("e") )
        {
            return kvs::KVSMLUnstructuredVolumeObject::ExternalAscii;
        }
    }

    return kvs::KVSMLUnstructuredVolumeObject::Ascii;
}

/*===========================================================================*/
/**
 *  @brief  Returns conversion method.
 *  @return conversion method
 */
/*===========================================================================*/
kvs::TetrahedraToTetrahedra::Method Argument::conversionMethod()
{
    kvs::TetrahedraToTetrahedra::Method method = kvs::TetrahedraToTetrahedra::Subdivision8;

    if ( this->hasOption("m") )
    {
        switch ( this->optionValue<int>("m") )
        {
        case 0: method = kvs::TetrahedraToTetrahedra::Subdivision8; break;
        case 1: method = kvs::TetrahedraToTetrahedra::Removal; break;
        default: break;
        }
    }

    return method;
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
bool Main::exec()
{
    // Parse specified arguments.
    TetConv::Argument arg( m_argc, m_argv );
    if ( !arg.parse() ) { return false; }

    // Set a input filename and a output filename.
    m_input_name = arg.inputFilename();
    m_output_name = arg.outputFilename( m_input_name );

    // Check input data file.
    if ( m_input_name.empty() )
    {
        kvsMessageError() << "Input file is not specified." << std::endl;
        return false;
    }

    kvs::File file( m_input_name );
    if ( !file.exists() )
    {
        kvsMessageError() << m_input_name << " is not found." << std::endl;
        return false;
    }

    // Import the unstructured volume object.
    auto* volume = new kvs::UnstructuredVolumeImporter( m_input_name );
    if ( !volume )
    {
        kvsMessageError() << "Cannot import " << m_input_name << "." << std::endl;
        return false;
    }

    // Convert quadratic tetrahedral volume to linear tetrahedral volume.
    const auto method = arg.conversionMethod();
    auto* object = new kvs::TetrahedraToTetrahedra( volume, method );
    if ( !object )
    {
        kvsMessageError() << "Cannot convert the tetrahedral volume dataset." << std::endl;
        delete volume;
        return false;
    }
    delete volume;

    // Write the unstructured volume object to a file in KVSML format.
    const bool ascii = !arg.hasOption("b");
    const bool external = arg.hasOption("e");
    object->write( m_output_name, ascii, external );
    delete object;

    return true;
}

} // end of namespace TetConv

} // end of namespace kvsconv
