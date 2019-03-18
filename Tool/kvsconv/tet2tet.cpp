/*****************************************************************************/
/**
 *  @file   tet2tet.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: tet2tet.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "tet2tet.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/UnstructuredVolumeExporter>


namespace kvsconv
{

namespace tet2tet
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Argument class for a tet2tet.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, kvsconv::tet2tet::CommandName )
{
    addOption( kvsconv::tet2tet::CommandName, kvsconv::tet2tet::Description, 0 );
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
    tet2tet::Argument arg( m_argc, m_argv );
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

    // Import the unstructured volume object.
    kvs::UnstructuredVolumeObject* volume = new kvs::UnstructuredVolumeImporter( m_input_name );
    if ( !volume )
    {
        kvsMessageError("Cannot import unstructured volume object.");
        return false;
    }

    // Convert quadratic tetrahedral volume to linear tetrahedral volume.
    const kvs::TetrahedraToTetrahedra::Method method = arg.conversionMethod();
    kvs::UnstructuredVolumeObject* object = new kvs::TetrahedraToTetrahedra( volume, method );
    if ( !object )
    {
        kvsMessageError("Cannot convert to tetrahedral volume dataset.");
        delete volume;
        return false;
    }
    delete volume;

    const bool ascii = !arg.hasOption("b");
    const bool external = arg.hasOption("e");
    object->write( m_output_name, ascii, external );
    delete object;

    return true;
}

} // end of namespace tet2tet

} // end of namespace kvsconv
