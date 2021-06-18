/****************************************************************************/
/**
 *  @file   UnstructuredVolumeObject.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "UnstructuredVolumeObject.h"
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/Range>


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

template<typename T>
kvs::Range GetMinMaxValues( const kvs::UnstructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->values().size() != 0 );

    const auto* values = reinterpret_cast<const T*>( volume->values().data() );
    const auto* connections = volume->connections().data();

    if ( volume->veclen() == 1 )
    {
        T min_value = *values;
        T max_value = *values;
        const size_t ncells = volume->numberOfCells();
        const size_t cell_nnodes = volume->numberOfCellNodes();
        for ( size_t i = 0; i < ncells; ++i )
        {
            for ( size_t j = 0; j < cell_nnodes; ++j )
            {
                const auto index = *connections++;
                const T value = values[ index ];
                min_value = kvs::Math::Min( value, min_value );
                max_value = kvs::Math::Max( value, max_value );
            }
        }
        return kvs::Range( static_cast<double>( min_value ), static_cast<double>( max_value ) );
    }
    else
    {
        kvs::Real64 min_value = kvs::Value<kvs::Real64>::Max();
        kvs::Real64 max_value = kvs::Value<kvs::Real64>::Min();
        const size_t veclen = volume->veclen();
        const size_t ncells = volume->numberOfCells();
        const size_t cell_nnodes = volume->numberOfCellNodes();
        for ( size_t i = 0; i < ncells; ++i )
        {
            for ( size_t j = 0; j < cell_nnodes; ++j )
            {
                const auto index = *connections++;
                const T* value = values + veclen * index;
                kvs::Real64 magnitude = 0.0;
                for ( size_t i = 0; i < veclen; ++i )
                {
                    magnitude += static_cast<kvs::Real64>( ( *value ) * ( *value ) );
                    ++value;
                }
                min_value = kvs::Math::Min( magnitude, min_value );
                max_value = kvs::Math::Max( magnitude, max_value );
            }
        }
        return kvs::Range( std::sqrt( min_value ), std::sqrt( max_value ) );
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
    const auto* coords = this->coords().data();
    const auto* connections = this->connections().data();

    const auto c0 = connections[0];
    kvs::Vec3 min_coord( coords[ 3 * c0 ], coords[ 3 * c0 + 1 ], coords[ 3 * c0 + 2 ] );
    kvs::Vec3 max_coord( coords[ 3 * c0 ], coords[ 3 * c0 + 1 ], coords[ 3 * c0 + 2 ] );

    const size_t ncells = this->numberOfCells();
    const size_t cell_nnodes = this->numberOfCellNodes();
    for ( size_t i = 0; i < ncells; ++i )
    {
        for ( size_t j = 0; j < cell_nnodes; ++j )
        {
            const auto index = *connections++;
            const auto x = coords[ 3 * index ];
            const auto y = coords[ 3 * index + 1 ];
            const auto z = coords[ 3 * index + 2 ];
            min_coord.x() = kvs::Math::Min( min_coord.x(), x );
            min_coord.y() = kvs::Math::Min( min_coord.y(), y );
            min_coord.z() = kvs::Math::Min( min_coord.z(), z );
            max_coord.x() = kvs::Math::Max( max_coord.x(), x );
            max_coord.y() = kvs::Math::Max( max_coord.y(), y );
            max_coord.z() = kvs::Math::Max( max_coord.z(), z );
        }
    }

    this->setMinMaxObjectCoords( min_coord, max_coord );

    if ( !( this->hasMinMaxExternalCoords() ) )
    {
        this->setMinMaxExternalCoords(
            this->minObjectCoord(),
            this->maxObjectCoord() );
    }
}

void UnstructuredVolumeObject::updateMinMaxValues() const
{
    kvs::Range range;
    switch ( this->values().typeID() )
    {
    case kvs::Type::TypeInt8:   { range = ::GetMinMaxValues<kvs::Int8  >( this ); break; }
    case kvs::Type::TypeInt16:  { range = ::GetMinMaxValues<kvs::Int16 >( this ); break; }
    case kvs::Type::TypeInt32:  { range = ::GetMinMaxValues<kvs::Int32 >( this ); break; }
    case kvs::Type::TypeInt64:  { range = ::GetMinMaxValues<kvs::Int64 >( this ); break; }
    case kvs::Type::TypeUInt8:  { range = ::GetMinMaxValues<kvs::UInt8 >( this ); break; }
    case kvs::Type::TypeUInt16: { range = ::GetMinMaxValues<kvs::UInt16>( this ); break; }
    case kvs::Type::TypeUInt32: { range = ::GetMinMaxValues<kvs::UInt32>( this ); break; }
    case kvs::Type::TypeUInt64: { range = ::GetMinMaxValues<kvs::UInt64>( this ); break; }
    case kvs::Type::TypeReal32: { range = ::GetMinMaxValues<kvs::Real32>( this ); break; }
    case kvs::Type::TypeReal64: { range = ::GetMinMaxValues<kvs::Real64>( this ); break; }
    default: break;
    }

    this->setMinMaxValues( range.lower(), range.upper() );
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
