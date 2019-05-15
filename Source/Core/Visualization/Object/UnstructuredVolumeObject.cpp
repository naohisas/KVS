/****************************************************************************/
/**
 *  @file   UnstructuredVolumeObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeObject.cpp 1770 2014-05-12 10:54:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "UnstructuredVolumeObject.h"
#include <kvs/KVSMLUnstructuredVolumeObject>


namespace
{

std::string CellTypeName[9] = {
    "unknown cell type",
    "tetrahedra",
    "hexahedra",
    "quadratic tetrahedra",
    "quadratic hexahedra",
    "pyramid",
    "point",
    "prism"
};

size_t NumberOfCellNodes[9] = {
    0,  // UnknownCellType
    4,  // Tetrahedra
    8,  // Hexahedra
    10, // QuadraticTetrahedra
    20, // QuadraticHexahedra
    5,  // Pyramid
    1,  // Point
    6   // Prism
};

/*==========================================================================*/
/**
 *  @brief  Converts to the cell type from the given string.
 *  @param  cell_type [in] grid type string
 *  @return cell type
 */
/*==========================================================================*/
kvs::UnstructuredVolumeObject::CellType GetCellType( const std::string& cell_type )
{
    if (      cell_type == "tetrahedra" ) { return kvs::UnstructuredVolumeObject::Tetrahedra; }
    else if ( cell_type == "quadratic tetrahedra" ) { return kvs::UnstructuredVolumeObject::QuadraticTetrahedra; }
    else if ( cell_type == "hexahedra"  ) { return kvs::UnstructuredVolumeObject::Hexahedra;  }
    else if ( cell_type == "quadratic hexahedra"  ) { return kvs::UnstructuredVolumeObject::QuadraticHexahedra;  }
    else if ( cell_type == "pyramid"  ) { return kvs::UnstructuredVolumeObject::Pyramid;  }
    else if ( cell_type == "point"  ) { return kvs::UnstructuredVolumeObject::Point;  }
    else if ( cell_type == "prism"  ) { return kvs::UnstructuredVolumeObject::Prism;  }
    else
    {
        kvsMessageError( "Unknown cell type '%s'.", cell_type.c_str() );
        return kvs::UnstructuredVolumeObject::UnknownCellType;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type.
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLUnstructuredVolumeObject::WritingDataType GetWritingDataType( const bool ascii, const bool external )
{
    if ( ascii )
    {
        if ( external ) { return kvs::KVSMLUnstructuredVolumeObject::ExternalAscii; }
        else { return kvs::KVSMLUnstructuredVolumeObject::Ascii; }
    }
    else
    {
        return kvs::KVSMLUnstructuredVolumeObject::ExternalBinary;
    }
}

} // end of namespace


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new UnstructuredVolumeObject class.
 */
/*==========================================================================*/
UnstructuredVolumeObject::UnstructuredVolumeObject():
    kvs::VolumeObjectBase(),
    m_cell_type( UnknownCellType ),
    m_nnodes( 0 ),
    m_ncells( 0 ),
    m_connections()
{
    BaseClass::setVolumeType( Unstructured );
}

/*===========================================================================*/
/**
 *  @brief  Shallow copys.
 *  @param  object [in] unstructured volume object
 */
/*===========================================================================*/
void UnstructuredVolumeObject::shallowCopy( const UnstructuredVolumeObject& object )
{
    BaseClass::shallowCopy( object );
    m_cell_type = object.cellType();
    m_nnodes = object.numberOfNodes();
    m_ncells = object.numberOfCells();
    m_connections = object.connections();
}

/*===========================================================================*/
/**
 *  @brief  Deep copys.
 *  @param  object [in] unstructured volume object
 */
/*===========================================================================*/
void UnstructuredVolumeObject::deepCopy( const UnstructuredVolumeObject& object )
{
    BaseClass::deepCopy( object );
    m_cell_type = object.cellType();
    m_nnodes = object.numberOfNodes();
    m_ncells = object.numberOfCells();
    m_connections = object.connections().clone();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the unstructured volume object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void UnstructuredVolumeObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    if ( !this->hasMinMaxValues() ) this->updateMinMaxValues();
    os << indent << "Object type : " << "unstructured volume object" << std::endl;
    BaseClass::print( os, indent );
    os << indent << "Cell type : " << ::CellTypeName[ this->cellType() ] << std::endl;
    os << indent << "Number of nodes : " << this->numberOfNodes() << std::endl;
    os << indent << "Number of cells : " << this->numberOfCells() << std::endl;
    os << indent << "Min. value : " << this->minValue() << std::endl;
    os << indent << "Max. value : " << this->maxValue() << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Read a unstructured volume object from the specified file in KVSML.
 *  @param  filename [in] input filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool UnstructuredVolumeObject::read( const std::string& filename )
{
    if ( !kvs::KVSMLUnstructuredVolumeObject::CheckExtension( filename ) )
    {
        kvsMessageError("%s is not an unstructured volume object file in KVSML.", filename.c_str());
        return false;
    }

    kvs::KVSMLUnstructuredVolumeObject kvsml;
    if ( !kvsml.read( filename ) ) { return false; }

    this->setVeclen( kvsml.veclen() );
    this->setNumberOfNodes( kvsml.nnodes() );
    this->setNumberOfCells( kvsml.ncells() );
    this->setCellType( ::GetCellType( kvsml.cellType() ) );
    this->setCoords( kvsml.coords() );
    this->setConnections( kvsml.connections() );
    this->setValues( kvsml.values() );

    if ( kvsml.hasExternalCoord() )
    {
        const kvs::Vec3 min_coord( kvsml.minExternalCoord() );
        const kvs::Vec3 max_coord( kvsml.maxExternalCoord() );
        this->setMinMaxExternalCoords( min_coord, max_coord );
    }

    if ( kvsml.hasObjectCoord() )
    {
        const kvs::Vec3 min_coord( kvsml.minObjectCoord() );
        const kvs::Vec3 max_coord( kvsml.maxObjectCoord() );
        this->setMinMaxObjectCoords( min_coord, max_coord );
    }
    else
    {
        this->updateMinMaxCoords();
    }

    if ( kvsml.hasLabel() ) { this->setLabel( kvsml.label() ); }
    if ( kvsml.hasUnit() ) { this->setUnit( kvsml.unit() ); }

    if ( kvsml.hasMinValue() && kvsml.hasMaxValue() )
    {
        const double min_value = kvsml.minValue();
        const double max_value = kvsml.maxValue();
        this->setMinMaxValues( min_value, max_value );
    }
    else
    {
        this->updateMinMaxValues();
        const double min_value = kvsml.hasMinValue() ? kvsml.minValue() : this->minValue();
        const double max_value = kvsml.hasMaxValue() ? kvsml.maxValue() : this->maxValue();
        this->setMinMaxValues( min_value, max_value );
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the unstructured volume object to the specfied file in KVSML.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool UnstructuredVolumeObject::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::KVSMLUnstructuredVolumeObject kvsml;
    kvsml.setWritingDataType( ::GetWritingDataType( ascii, external ) );

    if ( this->label() != "" ) { kvsml.setLabel( this->label() ); }
    if ( this->unit() != "" ) { kvsml.setUnit( this->unit() ); }

    switch ( this->cellType() )
    {
    case kvs::UnstructuredVolumeObject::UnknownCellType:
    {
        kvsMessageError("Unknown cell type.");
        break;
    }
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        kvsml.setCellType("tetrahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        kvsml.setCellType("quadratic tetrahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        kvsml.setCellType("hexahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        kvsml.setCellType("quadratic hexahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::Pyramid:
    {
        kvsml.setCellType("pyramid");
        break;
    }
    case kvs::UnstructuredVolumeObject::Prism:
    {
        kvsml.setCellType("prism");
        break;
    }
    case kvs::UnstructuredVolumeObject::Point:
    {
        kvsml.setCellType("point");
        break;
    }
    default:
    {
        kvsMessageError("Not supported cell type.");
        break;
    }
    }

    kvsml.setVeclen( this->veclen() );
    kvsml.setNNodes( this->numberOfNodes() );
    kvsml.setNCells( this->numberOfCells() );
    kvsml.setValues( this->values() );
    kvsml.setCoords( this->coords() );
    kvsml.setConnections( this->connections() );

    if ( this->hasMinMaxValues() )
    {
        kvsml.setMinValue( this->minValue() );
        kvsml.setMaxValue( this->maxValue() );
    }

    if ( this->hasMinMaxObjectCoords() )
    {
        kvsml.setMinMaxObjectCoords( this->minObjectCoord(), this->maxObjectCoord() );
    }

    if ( this->hasMinMaxExternalCoords() )
    {
        kvsml.setMinMaxExternalCoords( this->minExternalCoord(), this->maxExternalCoord() );
    }

    return kvsml.write( filename );
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of cell nodes.
 *  @return number of cell nodes
 */
/*===========================================================================*/
size_t UnstructuredVolumeObject::numberOfCellNodes() const
{
    return ::NumberOfCellNodes[ size_t( m_cell_type ) ];
}

/*==========================================================================*/
/**
 *  @brief  Updates the min/max node coordinates.
 */
/*==========================================================================*/
void UnstructuredVolumeObject::updateMinMaxCoords()
{
    kvs::Vec3 min_coord( 0.0f, 0.0f, 0.0f );
    kvs::Vec3 max_coord( 0.0f, 0.0f, 0.0f );

    const float* coord = this->coords().data();
    const float* const end = coord + this->coords().size();

    float x = *( coord++ );
    float y = *( coord++ );
    float z = *( coord++ );

    min_coord.set( x, y, z );
    max_coord.set( x, y, z );

    while ( coord < end )
    {
        x = *( coord++ );
        y = *( coord++ );
        z = *( coord++ );

        min_coord.x() = kvs::Math::Min( min_coord.x(), x );
        min_coord.y() = kvs::Math::Min( min_coord.y(), y );
        min_coord.z() = kvs::Math::Min( min_coord.z(), z );

        max_coord.x() = kvs::Math::Max( max_coord.x(), x );
        max_coord.y() = kvs::Math::Max( max_coord.y(), y );
        max_coord.z() = kvs::Math::Max( max_coord.z(), z );
    }

    this->setMinMaxObjectCoords( min_coord, max_coord );

    if ( !( this->hasMinMaxExternalCoords() ) )
    {
        this->setMinMaxExternalCoords(
            this->minObjectCoord(),
            this->maxObjectCoord() );
    }
}

std::ostream& operator << ( std::ostream& os, const UnstructuredVolumeObject& object )
{
    if ( !object.hasMinMaxValues() ) object.updateMinMaxValues();

    os << "Object type:  " << "unstructured volume object" << std::endl;
#ifdef KVS_COMPILER_VC
#if KVS_COMPILER_VERSION_LESS_OR_EQUAL( 8, 0 )
    // @TODO Cannot instance the object that is a abstract class here (error:C2259).
#endif
#else
//    os << static_cast<const kvs::VolumeObjectBase&>( object ) << std::endl;
    static_cast<const kvs::VolumeObjectBase&>( object ).print( os );
#endif
    os << "Cell type:  " << ::CellTypeName[ object.cellType() ] << std::endl;
    os << "Number of nodes:  " << object.numberOfNodes() << std::endl;
    os << "Number of cells:  " << object.numberOfCells() << std::endl;
    os << "Min. value:  " << object.minValue() << std::endl;
    os << "Max. value:  " << object.maxValue();

    return os;
}

} // end of namespace kvs
