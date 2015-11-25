/*****************************************************************************/
/**
 *  @file   fld2kvsml.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: fld2kvsml.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "fld2kvsml.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/AVSField>
#include <kvs/KVSMLStructuredVolumeObject>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/StructuredVolumeExporter>


namespace kvsconv
{

namespace fld2kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a fld2kvsml.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, kvsconv::fld2kvsml::CommandName )
{
    addOption( kvsconv::fld2kvsml::CommandName, kvsconv::fld2kvsml::Description, 0 );
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
    return( this->value<std::string>() );
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
        return( this->optionValue<std::string>("output") );
    }
    else
    {
        // Replace the extension as follows: xxxx.fld -> xxx.kvsml.
        const std::string basename = kvs::File( filename ).baseName();
        const std::string extension = "kvsml";
        return( basename + "." + extension );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type (ascii/external ascii/external binary).
 *  @return writing data type
 */
/*===========================================================================*/
const kvs::KVSMLStructuredVolumeObject::WritingDataType Argument::writingDataType( void )
{
    if ( this->hasOption("b") )
    {
        return( kvs::KVSMLStructuredVolumeObject::ExternalBinary );
    }
    else
    {
        if ( this->hasOption("e") )
        {
            return( kvs::KVSMLStructuredVolumeObject::ExternalAscii );
        }
    }

    return( kvs::KVSMLStructuredVolumeObject::Ascii );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Main class for a fld2kvsml.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Main::Main( int argc, char** argv )
{
    m_argc = argc;
    m_argv = argv;
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
const bool Main::exec( void )
{
    // Parse specified arguments.
    fld2kvsml::Argument arg( m_argc, m_argv );
    if( !arg.parse() ) return( false );

    // Set a input filename and a output filename.
    m_input_name = arg.inputFilename();
    m_output_name = arg.outputFilename( m_input_name );

    kvs::File file( m_input_name );
    if ( !file.isExisted() )
    {
        kvsMessageError("Input data file '%s' is not existed.",m_input_name.c_str());
        return( false );
    }

    // Read AVS Field data file.
    kvs::AVSField* input = new kvs::AVSField( m_input_name );
    if ( !input )
    {
        kvsMessageError("Cannot allocate for the AVS field data.");
        return( false );
    }

    if ( input->isFailure() )
    {
        kvsMessageError("Cannot read a file %s.", m_input_name.c_str() );
        delete input;
        return( false );
    }

    // Import AVS Field data as structured volume object.
    kvs::StructuredVolumeObject* object = new kvs::StructuredVolumeImporter( input );
    if ( !object )
    {
        kvsMessageError("Cannot import AVS Field data.");
        delete input;
        return( false );
    }

    delete input;

    // Export the structured volume object to KVSML data (structured volume).
    kvs::KVSMLStructuredVolumeObject* output =
        new kvs::StructuredVolumeExporter<kvs::KVSMLStructuredVolumeObject>( object );
    if ( !output )
    {
        kvsMessageError("Cannot export structured volume object.");
        delete object;
        return( false );
    }

    delete object;

    // Set the writing data type.
    output->setWritingDataType( arg.writingDataType() );

    // Write to KVSML data file.
    if ( !output->write( m_output_name ) )
    {
        kvsMessageError("Cannot write to KVSML data file %s.", m_output_name.c_str() );
        delete output;
        return( false );
    }

    delete output;

    return( true );
}

} // end of namespace fld2kvsml

} // end of namespace kvsconv
