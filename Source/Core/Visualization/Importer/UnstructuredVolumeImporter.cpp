/****************************************************************************/
/**
 *  @file   UnstructuredVolumeImporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeImporter.cpp 1811 2014-09-03 02:53:36Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "UnstructuredVolumeImporter.h"
#include <kvs/DebugNew>
#include <kvs/AVSUcd>
#include <kvs/Message>
#include <kvs/Vector3>


namespace
{

/*==========================================================================*/
/**
 *  @brief  Converts to the cell type from the given element type.
 *  @param  element_type [in] element type
 *  @return cell type
 */
/*==========================================================================*/
kvs::UnstructuredVolumeObject::CellType ElementTypeToCellType(
    const kvs::AVSUcd::ElementType element_type )
{
    if ( element_type == kvs::AVSUcd::Tetrahedra  )
    {
        return kvs::UnstructuredVolumeObject::Tetrahedra;
    }
    else if ( element_type == kvs::AVSUcd::Tetrahedra2 )
    {
        return kvs::UnstructuredVolumeObject::QuadraticTetrahedra;
    }
    else if ( element_type == kvs::AVSUcd::Hexahedra )
    {
        return kvs::UnstructuredVolumeObject::Hexahedra;
    }
    else if ( element_type == kvs::AVSUcd::Hexahedra2 )
    {
        return kvs::UnstructuredVolumeObject::QuadraticHexahedra;
    }
    else if ( element_type == kvs::AVSUcd::Point )
    {
        return kvs::UnstructuredVolumeObject::Point;
    }
    else if ( element_type == kvs::AVSUcd::Pyramid )
    {
        return kvs::UnstructuredVolumeObject::Pyramid;
    }
    else if ( element_type == kvs::AVSUcd::Prism )
    {
        return kvs::UnstructuredVolumeObject::Prism;
    }
    else
    {
        kvsMessageError( "Unknown element type." );
        return kvs::UnstructuredVolumeObject::UnknownCellType;
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new UnstructuredVolumeImporter class.
 */
/*===========================================================================*/
UnstructuredVolumeImporter::UnstructuredVolumeImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new UnstructuredVolumeImporter class.
 *  @param  filename [in] input filename
 */
/*===========================================================================*/
UnstructuredVolumeImporter::UnstructuredVolumeImporter( const std::string& filename )
{
    if ( kvs::KVSMLUnstructuredVolumeObject::CheckExtension( filename ) )
    {
        BaseClass::setSuccess( SuperClass::read( filename ) );
    }
    else if ( kvs::AVSUcd::CheckExtension( filename ) )
    {
        kvs::AVSUcd* file_format = new kvs::AVSUcd( filename );
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else if ( kvs::AVSField::CheckExtension( filename ) )
    {
        kvs::AVSField* file_format = new kvs::AVSField( filename );
        if( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Cannot import '%s'.",filename.c_str());
        return;
    }
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new UnstructuredVolumeImporter class.
 *  @param  file_format [in] pointer to the file format data
 */
/*==========================================================================*/
UnstructuredVolumeImporter::UnstructuredVolumeImporter( const kvs::FileFormatBase* file_format )
{
    this->exec( file_format );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the UnstructuredVolumeImporter class.
 */
/*===========================================================================*/
UnstructuredVolumeImporter::~UnstructuredVolumeImporter()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the import process.
 *  @param  file_format [in] pointer to the file format data
 *  @return pointer to the imported unstructured volume object
 */
/*===========================================================================*/
UnstructuredVolumeImporter::SuperClass* UnstructuredVolumeImporter::exec( const kvs::FileFormatBase* file_format )
{
    if ( !file_format )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is NULL.");
        return NULL;
    }

    if ( dynamic_cast<const kvs::KVSMLUnstructuredVolumeObject*>( file_format ) )
    {
        BaseClass::setSuccess( SuperClass::read( file_format->filename() ) );
    }
    else if ( const kvs::AVSUcd* volume = dynamic_cast<const kvs::AVSUcd*>( file_format ) )
    {
        this->import( volume );
    }
    else if ( const kvs::AVSField* volume = dynamic_cast<const kvs::AVSField*>( file_format ) )
    {
        this->import( volume );
    }
    else
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input file format is not supported.");
        return NULL;
    }

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Imports the AVS UCD format data.
 *  @param  ucd [in] pointer to the AVS UCD format data
 */
/*==========================================================================*/
void UnstructuredVolumeImporter::import( const kvs::AVSUcd* ucd )
{
    SuperClass::setVeclen( ucd->veclens()[ ucd->componentID() ] );
    SuperClass::setNumberOfNodes( ucd->nnodes() );
    SuperClass::setNumberOfCells( ucd->nelements() );
    SuperClass::setCellType( ::ElementTypeToCellType( ucd->elementType() ) );
    SuperClass::setCoords( ucd->coords() );
    SuperClass::setConnections( ucd->connections() );
    SuperClass::setValues( kvs::AnyValueArray( ucd->values() ) );
    SuperClass::updateMinMaxCoords();
    SuperClass::updateMinMaxValues();
}

/*===========================================================================*/
/**
 *  @brief  Imports the AVS Filed format data.
 *  @param  field [in] pointer to the AVS Filed format data
 */
/*===========================================================================*/
void UnstructuredVolumeImporter::import( const kvs::AVSField* field )
{
    if( field->fieldType() != kvs::AVSField::Irregular )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Cannot import uniform/rectilinear type AVS field data.");
        return;
    }

    const size_t line_size  = field->dim().x();
    const size_t slice_size = field->dim().y();
    const kvs::Vector3ui ncells( field->dim() - kvs::Vector3ui( 1, 1, 1 ) );
    SuperClass::Connections connections( ncells.x() * ncells.y() * ncells.z() * 8 );

    size_t vertex_index = 0;
    size_t connection_index = 0;

    for ( size_t z = 0; z < ncells.z(); ++z )
    {
        for ( size_t y = 0; y < ncells.y(); ++y )
        {
            for ( size_t x = 0; x < ncells.x(); ++x )
            {
                const size_t local_vertex_index[8] =
                {
                    vertex_index,
                    vertex_index + 1,
                    vertex_index + line_size,
                    vertex_index + line_size + 1,
                    vertex_index + slice_size,
                    vertex_index + slice_size + 1,
                    vertex_index + slice_size + line_size,
                    vertex_index + slice_size + line_size + 1
                };

                // hexahedra-1
                connections[ connection_index++ ] = local_vertex_index[ 0 ];
                connections[ connection_index++ ] = local_vertex_index[ 1 ];
                connections[ connection_index++ ] = local_vertex_index[ 3 ];
                connections[ connection_index++ ] = local_vertex_index[ 2 ];
                connections[ connection_index++ ] = local_vertex_index[ 4 ];
                connections[ connection_index++ ] = local_vertex_index[ 5 ];
                connections[ connection_index++ ] = local_vertex_index[ 7 ];
                connections[ connection_index++ ] = local_vertex_index[ 6 ];

                vertex_index++;
            }
            vertex_index++;
        }
        vertex_index += line_size;
    }

    SuperClass::setVeclen( field->veclen() );
    SuperClass::setNumberOfNodes( field->values().size() );
    SuperClass::setNumberOfCells( ncells.x() * ncells.y() * ncells.z() );
    SuperClass::setCellType( Hexahedra );
    SuperClass::setCoords( field->coords() );
    SuperClass::setConnections( connections );
    SuperClass::setValues( field->values() );
    SuperClass::updateMinMaxCoords();
    SuperClass::updateMinMaxValues();
}

} // end of namespace kvs
