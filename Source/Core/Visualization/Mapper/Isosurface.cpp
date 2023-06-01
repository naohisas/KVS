/****************************************************************************/
/**
 *  @file   Isosurface.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Isosurface.h"
#include <kvs/DebugNew>
#include <kvs/MarchingCubes>
#include <kvs/MarchingTetrahedra>
#include <kvs/MarchingHexahedra>
#include <kvs/MarchingPyramid>
#include <kvs/MarchingPrism>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Isosurface class.
 *  @param  volume [in] input volume object
 *  @param  normal_type [in] normal vector type
 */
/*===========================================================================*/
Isosurface::Isosurface(
    const kvs::VolumeObjectBase* volume,
    const SuperClass::NormalType normal_type ):
    kvs::MapperBase(),
    kvs::PolygonObject()
{
    SuperClass::setNormalType( normal_type );

    if ( !volume->hasMinMaxValues() ) { volume->updateMinMaxValues(); }
    this->setIsolevel( ( volume->maxValue() + volume->minValue() ) * 0.5 );

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( normal_type == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
    }

    // Extract the surfaces.
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Isosurface class.
 *  @param  volume [in] pointer to the input volume object
 *  @param  isolevel [in] isolevel
 *  @param  normal_type [in] normal vector type
 */
/*===========================================================================*/
Isosurface::Isosurface(
    const kvs::VolumeObjectBase* volume,
    const double isolevel,
    const NormalType normal_type ):
    kvs::MapperBase(),
    kvs::PolygonObject(),
    m_isolevel( isolevel )
{
    SuperClass::setNormalType( normal_type );

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( normal_type == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
    }

    // Extract the surfaces.
    this->exec( volume );
}

/*==========================================================================*/
/**
 *  @brief  Constructs and creates a polygon object.
 *  @param  volume [in] pointer to the volume object
 *  @param  isolevel [in] level of the isosurfaces
 *  @param  normal_type [in] type of the normal vector
 *  @param  duplication [in] duplication flag
 *  @param  transfer_function [in] transfer function
 */
/*==========================================================================*/
Isosurface::Isosurface(
    const kvs::VolumeObjectBase* volume,
    const double isolevel,
    const NormalType normal_type,
    const bool duplication,
    const kvs::TransferFunction& transfer_function ):
    kvs::MapperBase( transfer_function ),
    kvs::PolygonObject(),
    m_isolevel( isolevel ),
    m_duplication( duplication )
{
    SuperClass::setNormalType( normal_type );

    // In the case of VertexNormal-type, the duplicated vertices are forcibly deleted.
    if ( normal_type == kvs::PolygonObject::VertexNormal )
    {
        m_duplication = false;
    }

    // Extract the surfaces.
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the input volume object
 *  @return pointer to the polygon object
 */
/*===========================================================================*/
Isosurface::SuperClass* Isosurface::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const auto* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    this->mapping( volume );

    return this;
}

/*==========================================================================*/
/**
 *  @brief  Extracts the surfaces.
 *  @param  volume [in] pointer to the volume object
 */
/*==========================================================================*/
void Isosurface::mapping( const kvs::VolumeObjectBase* volume )
{
    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 1 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not scalar field dat.");
        return;
    }

    if ( volume->volumeType() == kvs::VolumeObjectBase::Structured )
    {
        const auto* structured_volume = kvs::StructuredVolumeObject::DownCast( volume );

        kvs::PolygonObject* polygon = new kvs::MarchingCubes(
            structured_volume,
            m_isolevel,
            SuperClass::normalType(),
            m_duplication,
            BaseClass::transferFunction() );
        if ( !polygon )
        {
            BaseClass::setSuccess( false );
            kvsMessageError("Cannot create isosurfaces.");
            return;
        }

        // Shallow copy.
        SuperClass::setCoords( polygon->coords() );
        SuperClass::setColors( polygon->colors() );
        SuperClass::setNormals( polygon->normals() );
        SuperClass::setConnections( polygon->connections() );
        SuperClass::setOpacities( polygon->opacities() );
        SuperClass::setPolygonType( polygon->polygonType() );
        SuperClass::setColorType( polygon->colorType() );
        SuperClass::setNormalType( polygon->normalType() );

        SuperClass::setMinMaxObjectCoords(
            polygon->minObjectCoord(),
            polygon->maxObjectCoord() );
        SuperClass::setMinMaxExternalCoords(
            polygon->minExternalCoord(),
            polygon->maxExternalCoord() );

        delete polygon;
    }
    else // volume->volumeType() == kvs::VolumeObjectBase::Unstructured
    {
        const auto* unstructured_volume = kvs::UnstructuredVolumeObject::DownCast( volume );

        switch ( unstructured_volume->cellType() )
        {
        case kvs::UnstructuredVolumeObject::Tetrahedra:
        {
            kvs::PolygonObject* polygon = new kvs::MarchingTetrahedra(
                unstructured_volume,
                m_isolevel,
                SuperClass::normalType(),
                m_duplication,
                BaseClass::transferFunction() );
            if ( !polygon )
            {
                BaseClass::setSuccess( false );
                kvsMessageError("Cannot create isosurfaces.");
                return;
            }

            // Shallow copy.
            SuperClass::setCoords( polygon->coords() );
            SuperClass::setColors( polygon->colors() );
            SuperClass::setNormals( polygon->normals() );
            SuperClass::setConnections( polygon->connections() );
            SuperClass::setOpacities( polygon->opacities() );
            SuperClass::setPolygonType( polygon->polygonType() );
            SuperClass::setColorType( polygon->colorType() );
            SuperClass::setNormalType( polygon->normalType() );

            SuperClass::setMinMaxObjectCoords(
                polygon->minObjectCoord(),
                polygon->maxObjectCoord() );
            SuperClass::setMinMaxExternalCoords(
                polygon->minExternalCoord(),
                polygon->maxExternalCoord() );

            delete polygon;
            break;
        }
        case kvs::UnstructuredVolumeObject::Hexahedra:
        {
            kvs::PolygonObject* polygon = new kvs::MarchingHexahedra(
                unstructured_volume,
                m_isolevel,
                SuperClass::normalType(),
                m_duplication,
                BaseClass::transferFunction() );
            if ( !polygon )
            {
                kvsMessageError("Cannot create isosurfaces.");
                return;
            }

            // Shallow copy.
            SuperClass::setCoords( polygon->coords() );
            SuperClass::setColors( polygon->colors() );
            SuperClass::setNormals( polygon->normals() );
            SuperClass::setConnections( polygon->connections() );
            SuperClass::setOpacities( polygon->opacities() );
            SuperClass::setPolygonType( polygon->polygonType() );
            SuperClass::setColorType( polygon->colorType() );
            SuperClass::setNormalType( polygon->normalType() );

            SuperClass::setMinMaxObjectCoords(
                polygon->minObjectCoord(),
                polygon->maxObjectCoord() );
            SuperClass::setMinMaxExternalCoords(
                polygon->minExternalCoord(),
                polygon->maxExternalCoord() );

            delete polygon;

            break;
        }
        case kvs::UnstructuredVolumeObject::Pyramid:
        {
            kvs::PolygonObject* polygon = new kvs::MarchingPyramid(
                unstructured_volume,
                m_isolevel,
                SuperClass::normalType(),
                m_duplication,
                BaseClass::transferFunction() );
            if ( !polygon )
            {
                BaseClass::setSuccess( false );
                kvsMessageError("Cannot create isosurfaces.");
                return;
            }

            // Shallow copy.
            SuperClass::setCoords( polygon->coords() );
            SuperClass::setColors( polygon->colors() );
            SuperClass::setNormals( polygon->normals() );
            SuperClass::setConnections( polygon->connections() );
            SuperClass::setOpacities( polygon->opacities() );
            SuperClass::setPolygonType( polygon->polygonType() );
            SuperClass::setColorType( polygon->colorType() );
            SuperClass::setNormalType( polygon->normalType() );

            SuperClass::setMinMaxObjectCoords(
                polygon->minObjectCoord(),
                polygon->maxObjectCoord() );
            SuperClass::setMinMaxExternalCoords(
                polygon->minExternalCoord(),
                polygon->maxExternalCoord() );

            delete polygon;
            break;
        }
        case kvs::UnstructuredVolumeObject::Prism:
        {
            kvs::PolygonObject* polygon = new kvs::MarchingPrism(
                unstructured_volume,
                m_isolevel,
                SuperClass::normalType(),
                m_duplication,
                BaseClass::transferFunction() );
            if ( !polygon )
            {
                kvsMessageError("Cannot create isosurfaces.");
                return;
            }

            // Shallow copy.
            SuperClass::setCoords( polygon->coords() );
            SuperClass::setColors( polygon->colors() );
            SuperClass::setNormals( polygon->normals() );
            SuperClass::setConnections( polygon->connections() );
            SuperClass::setOpacities( polygon->opacities() );
            SuperClass::setPolygonType( polygon->polygonType() );
            SuperClass::setColorType( polygon->colorType() );
            SuperClass::setNormalType( polygon->normalType() );

            SuperClass::setMinMaxObjectCoords(
                polygon->minObjectCoord(),
                polygon->maxObjectCoord() );
            SuperClass::setMinMaxExternalCoords(
                polygon->minExternalCoord(),
                polygon->maxExternalCoord() );

            delete polygon;

            break;
        }
        default: break;
        }
    }
}

} // end of namespace kvs
