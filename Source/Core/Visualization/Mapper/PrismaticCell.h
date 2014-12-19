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

    enum { NumberOfNodes = 6 };
    typedef kvs::CellBase BaseClass;

public:

    PrismaticCell( const kvs::UnstructuredVolumeObject* volume );
    virtual ~PrismaticCell();

    const kvs::Real32* interpolationFunctions( const kvs::Vec3& point ) const;
    const kvs::Real32* differentialFunctions( const kvs::Vec3& point ) const;
    const kvs::Vec3 randomSampling() const;
    const kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__PRISMATIC_CELL_H_INCLUDE
