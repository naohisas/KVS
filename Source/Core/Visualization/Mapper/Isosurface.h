/****************************************************************************/
/**
 *  @file   Isosurface.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/PolygonObject>
#include <kvs/VolumeObjectBase>
#include <kvs/MapperBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Isosurface class.
 */
/*==========================================================================*/
class Isosurface : public kvs::MapperBase, public kvs::PolygonObject
{
    kvsModule( kvs::Isosurface, Mapper );
    kvsModuleBaseClass( kvs::MapperBase );
    kvsModuleSuperClass( kvs::PolygonObject );

private:
    double m_isolevel = 0.0; ///< isosurface level
    bool m_duplication = true; ///< duplication flag

public:
    Isosurface() = default;
    Isosurface(
        const kvs::VolumeObjectBase* volume,
        const SuperClass::NormalType normal_type = SuperClass::PolygonNormal );
    Isosurface(
        const kvs::VolumeObjectBase* volume,
        const double isolevel,
        const SuperClass::NormalType normal_type = SuperClass::PolygonNormal );
    Isosurface(
        const kvs::VolumeObjectBase* volume,
        const double isolevel,
        const SuperClass::NormalType normal_type,
        const bool duplication,
        const kvs::TransferFunction& transfer_function );
    virtual ~Isosurface() = default;

    void setIsolevel( const double isolevel ) { m_isolevel = isolevel; }

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    void mapping( const kvs::VolumeObjectBase* volume );
};

} // end of namespace kvs
