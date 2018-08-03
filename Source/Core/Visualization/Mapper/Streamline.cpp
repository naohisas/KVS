/*****************************************************************************/
/**
 *  @file   Streamline.cpp
 *  @author Yukio Yasuhra, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Streamline.cpp 1778 2014-05-28 10:16:57Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Streamline.h"
#include <kvs/Type>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Message>
#include <kvs/RGBColor>
#include <kvs/Vector3>
#include <kvs/VolumeObjectBase>
#include <kvs/UniformGrid>
#include <kvs/RectilinearGrid>
#include <kvs/TetrahedralCell>
#include <kvs/HexahedralCell>
#include <kvs/QuadraticTetrahedralCell>
#include <kvs/QuadraticHexahedralCell>
#include <kvs/PyramidalCell>
#include <kvs/PrismaticCell>
#include <kvs/CellTreeLocator>


namespace kvs
{

Streamline::StructuredVolumeInterpolator::StructuredVolumeInterpolator(
    const kvs::StructuredVolumeObject* volume )
{
    switch ( volume->gridType() )
    {
    case kvs::StructuredVolumeObject::Uniform:
        m_grid = new kvs::UniformGrid( volume );
        break;
    case kvs::StructuredVolumeObject::Rectilinear:
        m_grid = new kvs::RectilinearGrid( volume );
        break;
    default:
        m_grid = NULL;
        break;
    }
}

Streamline::StructuredVolumeInterpolator::~StructuredVolumeInterpolator()
{
    if ( m_grid ) { delete m_grid; }
}

kvs::Vec3 Streamline::StructuredVolumeInterpolator::interpolatedValue( const kvs::Vec3& point )
{
    m_grid->bind( point );
    return m_grid->vector();
}

bool Streamline::StructuredVolumeInterpolator::containsInVolume( const kvs::Vec3& point )
{
    const kvs::Vec3& min_coord = m_grid->referenceVolume()->minObjectCoord();
    const kvs::Vec3& max_coord = m_grid->referenceVolume()->maxObjectCoord();
    if ( point.x() < min_coord.x() || max_coord.x() <= point.x() ) return false;
    if ( point.y() < min_coord.y() || max_coord.y() <= point.y() ) return false;
    if ( point.z() < min_coord.z() || max_coord.z() <= point.z() ) return false;
    return true;
}

Streamline::UnstructuredVolumeInterpolator::UnstructuredVolumeInterpolator(
    const kvs::UnstructuredVolumeObject* volume )
{
    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
        m_cell = new kvs::TetrahedralCell( volume );
        break;
    case kvs::UnstructuredVolumeObject::Hexahedra:
        m_cell = new kvs::HexahedralCell( volume );
        break;
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
        m_cell = new kvs::QuadraticTetrahedralCell( volume );
        break;
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
        m_cell = new kvs::QuadraticHexahedralCell( volume );
        break;
    case kvs::UnstructuredVolumeObject::Pyramid:
        m_cell = new kvs::PyramidalCell( volume );
        break;
    case kvs::UnstructuredVolumeObject::Prism:
        m_cell = new kvs::PrismaticCell( volume );
        break;
    default:
        m_cell = NULL;
        break;
    }

    m_locator = new kvs::CellTreeLocator( volume );
}

Streamline::UnstructuredVolumeInterpolator::~UnstructuredVolumeInterpolator()
{
    if ( m_cell ) { delete m_cell; }
    if ( m_locator ) { delete m_locator; }
}

kvs::Vec3 Streamline::UnstructuredVolumeInterpolator::interpolatedValue( const kvs::Vec3& point )
{
    int index = m_locator->findCell( point );
    if ( index < 0 ) { return kvs::Vec3::Zero(); }

    m_cell->bindCell( kvs::UInt32( index ) );
    return m_cell->vector();
}

bool Streamline::UnstructuredVolumeInterpolator::containsInVolume( const kvs::Vec3& point )
{
    const kvs::Vec3& min_obj = m_cell->referenceVolume()->minObjectCoord();
    const kvs::Vec3& max_obj = m_cell->referenceVolume()->maxObjectCoord();
    if ( point.x() < min_obj.x() || max_obj.x() <= point.x() ) return false;
    if ( point.y() < min_obj.y() || max_obj.y() <= point.y() ) return false;
    if ( point.z() < min_obj.z() || max_obj.z() <= point.z() ) return false;
    return m_locator->findCell( point ) != -1;
}

kvs::Vec3 Streamline::EulerIntegrator::next( const kvs::Vec3& point )
{
    const kvs::Real32 k0 = step();
    const kvs::Vec3 v1 = point;

    const kvs::Vec3 k1 = direction( v1 ) * k0;
    return point + k1;
}

kvs::Vec3 Streamline::RungeKutta2ndIntegrator::next( const kvs::Vec3& point )
{
    const kvs::Real32 k0 = step();
    const kvs::Vec3 v1 = point;

    const kvs::Vec3 k1 = direction( v1 ) * k0;
    const kvs::Vec3 v2 = point + k1 / 2.0f;
    if ( !contains( v2 ) ) { return point; }

    const kvs::Vec3 k2 = direction( v2 ) * k0;
    return point + k2;
}

kvs::Vec3 Streamline::RungeKutta4thIntegrator::next( const kvs::Vec3& point )
{
    const kvs::Real32 k0 = step();
    const kvs::Vec3 v1 = point;

    const kvs::Vec3 k1 = direction( v1 ) * k0;
    const kvs::Vec3 v2 = point + k1 / 2.0f;
    if ( !contains( v2 ) ) { return point; }

    const kvs::Vec3 k2 = direction( v2 ) * k0;
    const kvs::Vec3 v3 = point + k2 / 2.0f;
    if ( !contains( v3 ) ) { return point; }

    const kvs::Vec3 k3 = direction( v3 ) * k0;
    const kvs::Vec3 v4 = point + k3;
    if ( !contains( v4 ) ) { return point; }

    const kvs::Vec3 k4 = direction( v4 ) * k0;
    return point + ( k1 + 2.0f * ( k2 + k3 ) + k4 ) / 6.0f;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new streamline class and executes this class.
 *  @param  volume [in] pointer to the input volume object
 *  @param  seed_points [in] pointer to the seed points
 */
/*===========================================================================*/
Streamline::Streamline(
    const kvs::VolumeObjectBase* volume,
    const kvs::PointObject* seed_points,
    const kvs::TransferFunction& transfer_function ):
    kvs::StreamlineBase()
{
    BaseClass::setTransferFunction( transfer_function );
    BaseClass::setSeedPoints( seed_points );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return line object
 */
/*===========================================================================*/
Streamline::BaseClass::SuperClass* Streamline::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    // Check whether the volume can be processed or not.
    if ( volume->veclen() != 3 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input volume is not vector field data.");
        return NULL;
    }

    // Attach the pointer to the volume object.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // set the min/max vector length.
    if ( !volume->hasMinMaxValues() )
    {
        volume->updateMinMaxValues();
    }

    Interpolator* interpolator = NULL;
    switch ( volume->volumeType() )
    {
    case kvs::VolumeObjectBase::Structured:
    {
        const kvs::StructuredVolumeObject* svolume = kvs::StructuredVolumeObject::DownCast( volume );
        interpolator = new StructuredVolumeInterpolator( svolume );
        break;
    }
    case kvs::VolumeObjectBase::Unstructured:
    {
        const kvs::UnstructuredVolumeObject* uvolume = kvs::UnstructuredVolumeObject::DownCast( volume );
        interpolator = new UnstructuredVolumeInterpolator( uvolume );
        break;
    }
    default:
        break;
    }

    Integrator* integrator = NULL;
    switch ( m_integration_method )
    {
    case BaseClass::Euler:
        integrator = new EulerIntegrator();
        break;
    case BaseClass::RungeKutta2nd:
        integrator = new RungeKutta2ndIntegrator();
        break;
    case BaseClass::RungeKutta4th:
        integrator = new RungeKutta4thIntegrator();
        break;
    default:
        break;
    }

    integrator->setInterpolator( interpolator );
    integrator->setStep( m_integration_interval * m_integration_direction );
    BaseClass::mapping( integrator );

    delete interpolator;
    delete integrator;

    return this;
}

} // end of namespace kvs
