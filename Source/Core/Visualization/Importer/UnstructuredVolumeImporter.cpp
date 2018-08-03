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
        if ( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if ( file_format->isFailure() )
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
        if ( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if ( file_format->isFailure() )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            delete file_format;
            return;
        }

        this->import( file_format );
        delete file_format;
    }
    else if ( kvs::FieldViewData::CheckExtension( filename ) )
    {
        kvs::FieldViewData* file_format = new kvs::FieldViewData( filename );
        if ( !file_format )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot read '%s'.",filename.c_str());
            return;
        }

        if ( file_format->isFailure() )
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
    else if ( const kvs::FieldViewData* volume = dynamic_cast<const kvs::FieldViewData*>( file_format ) )
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

void UnstructuredVolumeImporter::import( const kvs::FieldViewData* const data )
{
    const size_t NumberOfNodesPerElement[5] = {
        0, // N/A
        4, // Tetra (1)
        8, // Hexa (2)
        6, // Prism (3)  not supported
        5, // Pyramid (4)
    };

    const kvs::UnstructuredVolumeObject::CellType CellType[5] = {
        kvs::UnstructuredVolumeObject::UnknownCellType, // N/A
        kvs::UnstructuredVolumeObject::Tetrahedra, // Tetra (1)
        kvs::UnstructuredVolumeObject::Hexahedra, // Hexa (2)
        kvs::UnstructuredVolumeObject::Prism, // Prism (3)
        kvs::UnstructuredVolumeObject::Pyramid // Pyramid (4)
    };

    if ( data->importingGridIndex() == data->numberOfGrids() )
    {
        const int etype = data->importingElementType();
        const size_t vindex = data->importingVariableIndex();

        KVS_ASSERT( FieldViewData::Tet <= etype && etype <= FieldViewData::Pyr );
        KVS_ASSERT( vindex < data->numberOfVariables() );

        const size_t veclen = data->grid(0).variables[ vindex ].type;
        const size_t nnodes_per_cell = NumberOfNodesPerElement[ etype ];
        const size_t total_nnodes = data->totalNumberOfNodes();
        const size_t total_ncells = data->totalNumberOfElements( etype );

        kvs::ValueArray<kvs::Real32> coords( total_nnodes * 3 );
        kvs::ValueArray<kvs::Real32> values( total_nnodes * veclen );
        kvs::ValueArray<kvs::UInt32> connections( total_ncells * nnodes_per_cell );
        kvs::Real32* pcoords = coords.data();
        kvs::Real32* pvalues = values.data();
        kvs::UInt32* pconnections = connections.data();

        const size_t ngrids = data->numberOfGrids();
        for ( size_t i = 0; i < ngrids; i++ )
        {
            // i-th grid.
            const FieldViewData::Grid& grid = data->grid(i);

            const size_t nnodes = data->grid(i).nnodes;
            for ( size_t j = 0; j < nnodes; j++ )
            {
                // Coordinate values.
                *(pcoords++) = grid.nodes[j].x;
                *(pcoords++) = grid.nodes[j].y;
                *(pcoords++) = grid.nodes[j].z;

                // Node values.
                for ( size_t k = 0; k < veclen; k++ )
                {
                    *(pvalues++) = grid.variables[ vindex + k ].data[j];
                }
            }

            if ( grid.nelements[etype] != 0 )
            {
                const size_t offset = nnodes * i;
                const size_t ncells = grid.nelements[0];
                for ( size_t j = 0; j < ncells; j++ )
                {
                    if ( grid.elements[j].type == etype )
                    {
                        if ( etype == 1 ) // tet
                        {
                            *(pconnections++) = offset + grid.elements[j].id[0] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[1] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[2] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[3] - 1;
                        }
                        else if ( etype == 2 ) // hex
                        {
                            *(pconnections++) = offset + grid.elements[j].id[0] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[1] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[3] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[2] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[4] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[5] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[7] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[6] - 1;
                        }
                        else if ( etype == 3 ) // prism
                        {
                            *(pconnections++) = offset + grid.elements[j].id[0] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[3] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[5] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[1] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[2] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[4] - 1;
                        }
                        else if ( etype == 4 ) // pyramid
                        {
                            *(pconnections++) = offset + grid.elements[j].id[4] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[0] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[1] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[2] - 1;
                            *(pconnections++) = offset + grid.elements[j].id[3] - 1;
                        }
                    }
                }
            }
        }

        SuperClass::setCellType( CellType[ etype ] );
        SuperClass::setVeclen( veclen );
        SuperClass::setNumberOfNodes( total_nnodes );
        SuperClass::setNumberOfCells( total_ncells );
        SuperClass::setCoords( coords );
        SuperClass::setConnections( connections );
        SuperClass::setValues( values );
        SuperClass::updateMinMaxValues();
        SuperClass::updateMinMaxCoords();
    }
    else
    {
        const int etype = data->importingElementType();
        const size_t gindex = data->importingGridIndex();
        const size_t vindex = data->importingVariableIndex();

        KVS_ASSERT( FieldViewData::Tet <= etype && etype <= FieldViewData::Pyr );
        KVS_ASSERT( gindex < data->numberOfGrids() );
        KVS_ASSERT( vindex < data->numberOfVariables() );

        const kvs::FieldViewData::Grid& grid = data->grid( gindex );
        if ( grid.nelements[ etype ] == 0 )
        {
            BaseClass::setSuccess( false );
            kvsMessageError( "Not found the specified element type." );
            return;
        }

        const size_t nnodes = grid.nodes.size();
        const size_t ncells = grid.nelements[ etype ];
        const size_t veclen = grid.variables[ vindex ].type;
        const size_t nnodes_per_cell = NumberOfNodesPerElement[ etype ];

        kvs::ValueArray<kvs::Real32> coords( nnodes * 3 );
        for ( size_t i = 0; i < nnodes; i++ )
        {
            coords[ 3 * i + 0 ] = grid.nodes[i].x;
            coords[ 3 * i + 1 ] = grid.nodes[i].y;
            coords[ 3 * i + 2 ] = grid.nodes[i].z;
        }

        kvs::ValueArray<kvs::UInt32> connections( ncells * nnodes_per_cell );
        for ( size_t i = 0, j = 0; i < grid.nelements[0]; i++ )
        {
            KVS_ASSERT( j < ncells * nnodes_per_cell );
            if ( grid.elements[i].type == etype )
            {
                KVS_ASSERT( grid.elements[i].id.size() == nnodes_per_cell );

                if ( etype == 1 ) // tet
                {
                    connections[ nnodes_per_cell * j + 0 ] = grid.elements[i].id[0] - 1;
                    connections[ nnodes_per_cell * j + 1 ] = grid.elements[i].id[1] - 1;
                    connections[ nnodes_per_cell * j + 2 ] = grid.elements[i].id[2] - 1;
                    connections[ nnodes_per_cell * j + 3 ] = grid.elements[i].id[3] - 1;
                }
                else if ( etype == 2 ) // hex
                {
                    connections[ nnodes_per_cell * j + 0 ] = grid.elements[i].id[0] - 1;
                    connections[ nnodes_per_cell * j + 1 ] = grid.elements[i].id[1] - 1;
                    connections[ nnodes_per_cell * j + 2 ] = grid.elements[i].id[3] - 1;
                    connections[ nnodes_per_cell * j + 3 ] = grid.elements[i].id[2] - 1;
                    connections[ nnodes_per_cell * j + 4 ] = grid.elements[i].id[4] - 1;
                    connections[ nnodes_per_cell * j + 5 ] = grid.elements[i].id[5] - 1;
                    connections[ nnodes_per_cell * j + 6 ] = grid.elements[i].id[7] - 1;
                    connections[ nnodes_per_cell * j + 7 ] = grid.elements[i].id[6] - 1;
                }
                else if ( etype == 3 ) // prism
                {
                    connections[ nnodes_per_cell * j + 0 ] = grid.elements[i].id[0] - 1;
                    connections[ nnodes_per_cell * j + 1 ] = grid.elements[i].id[3] - 1;
                    connections[ nnodes_per_cell * j + 2 ] = grid.elements[i].id[5] - 1;
                    connections[ nnodes_per_cell * j + 3 ] = grid.elements[i].id[1] - 1;
                    connections[ nnodes_per_cell * j + 4 ] = grid.elements[i].id[2] - 1;
                    connections[ nnodes_per_cell * j + 5 ] = grid.elements[i].id[4] - 1;
                }
                else if ( etype == 4 ) // pyramid
                {
                    connections[ nnodes_per_cell * j + 0 ] = grid.elements[i].id[4] - 1;
                    connections[ nnodes_per_cell * j + 1 ] = grid.elements[i].id[0] - 1;
                    connections[ nnodes_per_cell * j + 2 ] = grid.elements[i].id[1] - 1;
                    connections[ nnodes_per_cell * j + 3 ] = grid.elements[i].id[2] - 1;
                    connections[ nnodes_per_cell * j + 4 ] = grid.elements[i].id[3] - 1;
                }
                j++;
            }
        }

        kvs::ValueArray<kvs::Real32> values( nnodes * veclen );
        if ( veclen == 1 )
        {
            KVS_ASSERT( nnodes * veclen == grid.variables[ vindex ].data.size() );
            values = grid.variables[ vindex ].data;
        }
        else if ( veclen == 3 )
        {
            KVS_ASSERT( vindex + 2 < data->numberOfVariables() );
            KVS_ASSERT( grid.variables[ vindex + 0 ].type == grid.variables[ vindex + 1 ].type );
            KVS_ASSERT( grid.variables[ vindex + 1 ].type == grid.variables[ vindex + 2 ].type );
            for ( size_t i = 0; i < nnodes; i++ )
            {
                values[ 3 * i + 0 ] = grid.variables[ vindex + 0 ].data[ i ];
                values[ 3 * i + 1 ] = grid.variables[ vindex + 1 ].data[ i ];
                values[ 3 * i + 2 ] = grid.variables[ vindex + 2 ].data[ i ];
            }
        }

        SuperClass::setCellType( CellType[ etype ] );
        SuperClass::setVeclen( veclen );
        SuperClass::setNumberOfNodes( nnodes );
        SuperClass::setNumberOfCells( ncells );
        SuperClass::setCoords( coords );
        SuperClass::setConnections( connections );
        SuperClass::setValues( values );
        SuperClass::updateMinMaxValues();
        SuperClass::updateMinMaxCoords();
    }
}

} // end of namespace kvs
