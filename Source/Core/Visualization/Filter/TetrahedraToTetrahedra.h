/*****************************************************************************/
/**
 *  @file   TetrahedraToTetrahedra.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TetrahedraToTetrahedra.h 1721 2014-03-12 15:27:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__TETRAHEDRA_TO_TETRAHEDRA_H_INCLUDE
#define KVS__TETRAHEDRA_TO_TETRAHEDRA_H_INCLUDE

#include <kvs/UnstructuredVolumeObject>
#include <kvs/FilterBase>
#include <kvs/Module>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  TetrahedraToTetrahedra class.
 */
/*===========================================================================*/
class TetrahedraToTetrahedra : public kvs::FilterBase, public kvs::UnstructuredVolumeObject
{
    kvsModule( kvs::TetrahedraToTetrahedra, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::UnstructuredVolumeObject );

public:

    enum Method
    {
        Subdivision8, ///< subdivide a quadratic tetrahedron into eight linear tetrahedra
        Removal ///< remove the quadratic nodes
    };

private:

    Method m_method; ///< conversion method

public:

    TetrahedraToTetrahedra();
    TetrahedraToTetrahedra( const kvs::UnstructuredVolumeObject* volume, const Method method = Subdivision8 );
    virtual ~TetrahedraToTetrahedra();

    SuperClass* exec( const kvs::ObjectBase* object );

    void setMethod( const Method method );

protected:

    template <typename T>
    void remove_quadratic_nodes( const kvs::UnstructuredVolumeObject* volume );

    template <typename T>
    void subdivide_8_tetrahedra( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs

#endif // KVS__TETRAHEDRA_TO_TETRAHEDRA_H_INCLUDE
