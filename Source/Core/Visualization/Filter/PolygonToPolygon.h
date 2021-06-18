/****************************************************************************/
/**
 *  @file   PolygonToPolygon.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/PolygonObject>
#include <kvs/Module>
#include <kvs/FilterBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PolygonToPolygon class
 */
/*===========================================================================*/
class PolygonToPolygon : public kvs::FilterBase, public kvs::PolygonObject
{
    kvsModule( kvs::PolygonToPolygon, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::PolygonObject );

public:
    PolygonToPolygon();
    PolygonToPolygon( const kvs::PolygonObject* object );
    virtual ~PolygonToPolygon();

    SuperClass* exec( const kvs::ObjectBase* object );

private:
    void calculate_triangle_connections( const kvs::PolygonObject* object );
    void calculate_triangle_normals();
};

} // end of namespace kvs
