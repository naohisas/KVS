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
    os << static_cast<const kvs::VolumeObjectBase&>( object ) << std::endl;
#endif
    os << "Cell type:  " << ::CellTypeName[ object.cellType() ] << std::endl;
    os << "Number of nodes:  " << object.numberOfNodes() << std::endl;
    os << "Number of cells:  " << object.numberOfCells() << std::endl;
    os << "Min. value:  " << object.minValue() << std::endl;
    os << "Max. value:  " << object.maxValue();

    return os;
}

} // end of namespace kvs
