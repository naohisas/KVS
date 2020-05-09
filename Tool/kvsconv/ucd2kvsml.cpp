/*****************************************************************************/
/**
 *  @file   ucd2kvsml.cpp
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
#include "ucd2kvsml.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/AVSUcd>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>


namespace kvsconv
{

namespace ucd2kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a ucd2kvsml.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, kvsconv::ucd2kvsml::CommandName )
{
    addOption( kvsconv::ucd2kvsml::CommandName, kvsconv::ucd2kvsml::Description, 0 );
    addOption( "e", "External data file. (optional)", 0, false );
    addOption( "b", "Data file as binary. (optional)", 0, false );
}

/*===========================================================================*/
/**
 *  @brief  Returns a input filename.
 *  @return input filename
 */
/*===========================================================================*/
const std::string Argument::inputFilename( void )
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
const std::string Argument::outputFilename( const std::string& filename )
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
 *  @brief  Executes main process.
 */
/*===========================================================================*/
bool Main::exec()
{
    // Parse specified arguments.
    ucd2kvsml::Argument arg( m_argc, m_argv );
    if( !arg.parse() ) { return false; }

    // Set a input filename and a output filename.
    m_input_name = arg.inputFilename();
    m_output_name = arg.outputFilename( m_input_name );

    kvs::File file( m_input_name );
    if ( !file.exists() )
    {
        kvsMessageError("Input data file '%s' is not existed.",m_input_name.c_str());
        return false;
    }

    // Read AVS UCD data file.
    kvs::AVSUcd* input = new kvs::AVSUcd( m_input_name );
    if ( input->isFailure() )
    {
        kvsMessageError("Cannot read a file %s.", m_input_name.c_str() );
        delete input;
        return false;
    }

    // Import AVS UCD data as unstructured volume object.
    kvs::UnstructuredVolumeObject* object = new kvs::UnstructuredVolumeImporter( input );
    delete input;

    const bool ascii = !arg.hasOption("b");
    const bool external = arg.hasOption("e");
    object->write( m_output_name, ascii, external );
    delete object;

    return true;
}

} // end of namespace ucd2kvsml

} // end of namespace kvsconv
