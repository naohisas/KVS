/*****************************************************************************/
/**
 *  @file   UnstructuredVolumeExporter.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: UnstructuredVolumeExporter.cpp 1824 2014-12-12 08:49:03Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "UnstructuredVolumeExporter.h"
#include <kvs/ObjectBase>
#include <kvs/VolumeObjectBase>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a KVSMLUnstructuredVolumeObject data from given object.
 *  @param  object [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
UnstructuredVolumeExporter<kvs::KVSMLUnstructuredVolumeObject>::UnstructuredVolumeExporter(
    const kvs::UnstructuredVolumeObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Exports to a KVSMLUnstructuredVolumeObject data.
 *  @param  object [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
kvs::KVSMLUnstructuredVolumeObject* UnstructuredVolumeExporter<kvs::KVSMLUnstructuredVolumeObject>::exec(
    const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    // Cast to the structured volume object.
    const kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not structured volume object.");
        return NULL;
    }

    if ( volume->label() != "" ) { this->setLabel( volume->label() ); }
    if ( volume->unit() != "" ) { this->setUnit( volume->unit() ); }

    // Check the cell type of the given unstructured volume object.
    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::UnknownCellType:
    {
        kvsMessageError("Unknown cell type.");
        break;
    }
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        this->setCellType("tetrahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        this->setCellType("quadratic tetrahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        this->setCellType("hexahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        this->setCellType("quadratic hexahedra");
        break;
    }
    case kvs::UnstructuredVolumeObject::Pyramid:
    {
        this->setCellType("pyramid");
        break;
    }
    case kvs::UnstructuredVolumeObject::Prism:
    {
        this->setCellType("prism");
        break;
    }
    case kvs::UnstructuredVolumeObject::Point:
    {
        this->setCellType("point");
        break;
    }
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Not supported cell type.");
        break;
    }
    }

    this->setVeclen( volume->veclen() );
    this->setNNodes( volume->numberOfNodes() );
    this->setNCells( volume->numberOfCells() );
    this->setValues( volume->values() );
    this->setCoords( volume->coords() );
    this->setConnections( volume->connections() );

    if ( volume->hasMinMaxValues() )
    {
        this->setMinValue( volume->minValue() );
        this->setMaxValue( volume->maxValue() );
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a AVS UCD data from given object.
 *  @param  object [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
UnstructuredVolumeExporter<kvs::AVSUcd>::UnstructuredVolumeExporter(
    const kvs::UnstructuredVolumeObject* object )
{
    this->exec( object );
}

/*===========================================================================*/
/**
 *  @brief  Exports to a AVS UCD data.
 *  @param  object [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
kvs::AVSUcd* UnstructuredVolumeExporter<kvs::AVSUcd>::exec(
    const kvs::ObjectBase* object )
{
    BaseClass::setSuccess( true );

    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    // Cast to the structured volume object.
    const kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not structured volume object.");
        return NULL;
    }

    // Check the cell type of the given unstructured volume object.
    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::UnknownCellType:
    {
        kvsMessageError("Unknown cell type.");
        break;
    }
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        this->setElementType( kvs::AVSUcd::Tetrahedra );
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        this->setElementType( kvs::AVSUcd::Tetrahedra2 );
        break;
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        this->setElementType( kvs::AVSUcd::Hexahedra );
        break;
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        this->setElementType( kvs::AVSUcd::Hexahedra2 );
        break;
    }
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Not supported cell type.");
        break;
    }
    }

    std::vector<size_t> veclens; veclens.push_back( volume->veclen() );
    std::vector<std::string> component_names; component_names.push_back( "value" );
    std::vector<std::string> component_units; component_units.push_back( "unit" );
    kvs::ValueArray<kvs::Real32> values( volume->values().size() );
    for ( size_t i = 0; i < values.size(); i++ ) values[i] = volume->values()[i].to<kvs::Real32>();

    this->setNSteps( 1 );
    this->setNComponentsPerNode( 1 );
    this->setNValuesPerNode( 1 );
    this->setNNodes( volume->numberOfNodes() );
    this->setNElements( volume->numberOfCells() );
    this->setVeclens( veclens );
    this->setComponentNames( component_names );
    this->setComponentUnits( component_units );
    this->setCoords( volume->coords() );
    this->setConnections( volume->connections() );
    this->setValues( values );

    return this;
}

} // end of namespace kvs
