/****************************************************************************/
/**
 *  @file   TetrahedralCell.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TetrahedralCell.h 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TETRAHEDRAL_CELL_H_INCLUDE
#define KVS__TETRAHEDRAL_CELL_H_INCLUDE

#include <kvs/ClassName>
#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/IgnoreUnusedVariable>
#include "CellBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Tetrahedral cell class.
 */
/*===========================================================================*/
class TetrahedralCell : public kvs::CellBase
{
public:

    enum { NumberOfNodes = kvs::UnstructuredVolumeObject::Tetrahedra };
    typedef kvs::CellBase BaseClass;

public:

    TetrahedralCell( const kvs::UnstructuredVolumeObject* volume );
    virtual ~TetrahedralCell();

    const kvs::Real32* interpolationFunctions( const kvs::Vec3& local ) const;
    const kvs::Real32* differentialFunctions( const kvs::Vec3& local ) const;
    const kvs::Vec3 randomSampling() const;
    const kvs::Real32 volume() const;
    const kvs::Vec3 transformGlobalToLocal( const kvs::Vec3& global ) const;
    const kvs::Vec3 transformLocalToGlobal( const kvs::Vec3& local ) const;
};

} // end of namespace kvs

#endif // KVS__TETRAHEDRAL_CELL_H_INCLUDE
