/*****************************************************************************/
/**
 *  @file   PrismaticCell.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__PRISMATIC_CELL_H_INCLUDE
#define KVS__PRISMATIC_CELL_H_INCLUDE

#include <kvs/Type>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/CellBase>


namespace kvs
{

class PrismaticCell : public kvs::CellBase
{
public:

    typedef kvs::CellBase BaseClass;

public:

    PrismaticCell( const kvs::UnstructuredVolumeObject* volume );

    void updateInterpolationFunctions( const kvs::Vec3& local ) const;
    void updateDifferentialFunctions( const kvs::Vec3& local ) const;
    bool containsLocalPoint( const kvs::Vec3& local ) const;
    kvs::Vec3 randomSampling() const;
    kvs::Real32 volume() const;
    kvs::Vec3 localCenter() const;
};

} // end of namespace kvs

#endif // KVS__PRISMATIC_CELL_H_INCLUDE
