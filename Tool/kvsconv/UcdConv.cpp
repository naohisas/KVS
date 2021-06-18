/*****************************************************************************/
/**
 *  @file   UcdConv.cpp
 *  @author Naohisa Sakamoto
 *  @brief  AVS UCD Data Converter
 */
/*****************************************************************************/
#include "UcdConv.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/AVSUcd>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>


namespace kvsconv
{

namespace UcdConv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a ucd2kvsml.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, UcdConv::CommandName )
{
    addOption( UcdConv::CommandName, UcdConv::Description, 0 );
    addOption( "e", "External data file. (optional)", 0, false );
    addOption( "b", "Data file as binary. (optional)", 0, false );
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
        // Default output filename: <basename_of_filename>.kvsml
        // e.g) avs_data.inp -> avs_data.kvsml
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
 *  @brief  Executes main process.
 */
/*===========================================================================*/
bool Main::exec()
{
    // Parse specified arguments.
    UcdConv::Argument arg( m_argc, m_argv );
    if( !arg.parse() ) { return false; }

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

    // Read AVS UCD data file.
    auto* data = new kvs::AVSUcd( m_input_name );
    if ( data->isFailure() )
    {
        kvsMessageError() << "Cannot read " << m_input_name << "." << std::endl;
        delete data;
        return false;
    }

    // Import AVS UCD data as unstructured volume object.
    auto* object = new kvs::UnstructuredVolumeImporter( data );
    if ( !object )
    {
        kvsMessageError() << "Cannot import " << m_input_name << "." << std::endl;
        delete data;
        return false;
    }
    delete data;

    // Write the unstructured volume object to a file in KVSML format.
    const bool ascii = !arg.hasOption("b");
    const bool external = arg.hasOption("e");
    object->write( m_output_name, ascii, external );
    delete object;

    return true;
}

} // end of namespace UcdConv

} // end of namespace kvsconv
