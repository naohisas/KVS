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

    typedef kvs::CellBase BaseClass;

public:

    TetrahedralCell( const kvs::UnstructuredVolumeObject* volume );

    void updateInterpolationFunctions( const kvs::Vec3& local ) const;
    void updateDifferentialFunctions( const kvs::Vec3& local ) const;
    bool containsLocalPoint( const kvs::Vec3& local ) const;
    kvs::Vec3 randomSampling() const;
    kvs::Real32 volume() const;
    kvs::Vec3 localCenter() const;
    kvs::Vec3 globalPoint() const;
    kvs::Vec3 localToGlobal( const kvs::Vec3& global ) const;
    kvs::Vec3 globalToLocal( const kvs::Vec3& local ) const;
};

} // end of namespace kvs

#endif // KVS__TETRAHEDRAL_CELL_H_INCLUDE
