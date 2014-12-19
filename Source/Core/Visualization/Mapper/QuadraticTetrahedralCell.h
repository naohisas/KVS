/*****************************************************************************/
/**
 *  @file   QuadraticTetrahedralCell.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QuadraticTetrahedralCell.h 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__QUADRATIC_TETRAHEDRAL_CELL_H_INCLUDE
#define KVS__QUADRATIC_TETRAHEDRAL_CELL_H_INCLUDE

#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include "CellBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  QuadraticTetrahedralCell class.
 */
/*===========================================================================*/
class QuadraticTetrahedralCell : public kvs::CellBase
{
public:

    enum { NumberOfNodes = kvs::UnstructuredVolumeObject::QuadraticTetrahedra };
    typedef kvs::CellBase BaseClass;

public:

    QuadraticTetrahedralCell( const kvs::UnstructuredVolumeObject* volume );
    virtual ~QuadraticTetrahedralCell();

    const kvs::Real32* interpolationFunctions( const kvs::Vec3& point ) const;
    const kvs::Real32* differentialFunctions( const kvs::Vec3& point ) const;
    const kvs::Vec3 randomSampling() const;
    const kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__QUADRATIC_TETRAHEDRAL_CELL_H_INCLUDE
