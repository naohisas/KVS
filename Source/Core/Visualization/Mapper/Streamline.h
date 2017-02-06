/*****************************************************************************/
/**
 *  @file   Streamline.h
 *  @author Yukio Yasuhara, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Streamline.h 1778 2014-05-28 10:16:57Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__STREAMLINE_H_INCLUDE
#define KVS__STREAMLINE_H_INCLUDE

#include <kvs/Module>
#include <kvs/GridBase>
#include <kvs/CellBase>
#include <kvs/CellLocator>
#include "StreamlineBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Streamline class.
 */
/*===========================================================================*/
class Streamline : public kvs::StreamlineBase
{
    kvsModule( kvs::Streamline, Mapper );
    kvsModuleBaseClass( kvs::StreamlineBase );

public:

    class StructuredVolumeInterpolator : public Interpolator
    {
    private:
        kvs::GridBase* m_grid;
    public:
        StructuredVolumeInterpolator( const kvs::StructuredVolumeObject* volume );
        ~StructuredVolumeInterpolator();
        kvs::Vec3 interpolatedValue( const kvs::Vec3& point );
        bool containsInVolume( const kvs::Vec3& point );
    };

    class UnstructuredVolumeInterpolator : public Interpolator
    {
    private:
        kvs::CellBase* m_cell;
        kvs::CellLocator* m_locator;
    public:
        UnstructuredVolumeInterpolator( const kvs::UnstructuredVolumeObject* volume );
        ~UnstructuredVolumeInterpolator();
        kvs::Vec3 interpolatedValue( const kvs::Vec3& point );
        bool containsInVolume( const kvs::Vec3& point );
    };

    class EulerIntegrator : public Integrator
    {
    public:
        kvs::Vec3 next( const kvs::Vec3& point );
    };

    class RungeKutta2ndIntegrator : public Integrator
    {
    public:
        kvs::Vec3 next( const kvs::Vec3& point );
    };

    class RungeKutta4thIntegrator : public Integrator
    {
    public:
        kvs::Vec3 next( const kvs::Vec3& point );
    };

public:

    Streamline() {}

    Streamline(
        const kvs::VolumeObjectBase* volume,
        const kvs::PointObject* seed_points,
        const kvs::TransferFunction& transfer_function );

    BaseClass::SuperClass* exec( const kvs::ObjectBase* object );
};

} // end of namespace kvs

#endif // KVS__STREAMLINE_BASE_H_INCLUDE
