/*****************************************************************************/
/**
 *  @file   QuadraticTetrahedralCell.h
 *  @author Takuma Kawamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QuadraticHexahedralCell.h 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__QUADRATIC_HEXAHEDRAL_CELL_H_INCLUDE
#define KVS__QUADRATIC_HEXAHEDRAL_CELL_H_INCLUDE

#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include "CellBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  QuadraticHexahedralCell class.
 */
/*===========================================================================*/
class QuadraticHexahedralCell : public kvs::CellBase
{
public:

    enum { NumberOfNodes = kvs::UnstructuredVolumeObject::QuadraticHexahedra };
    typedef kvs::CellBase BaseClass;

public:

    QuadraticHexahedralCell( const kvs::UnstructuredVolumeObject* volume );
    virtual ~QuadraticHexahedralCell();

    const kvs::Real32* interpolationFunctions( const kvs::Vec3& point ) const;
    const kvs::Real32* differentialFunctions( const kvs::Vec3& point ) const;
    const kvs::Vec3 randomSampling() const;
    const kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__QUADRATIC_HEXAHEDRAL_CELL_H_INCLUDE
