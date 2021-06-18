/*****************************************************************************/
/**
 *  @file   QuadraticTetrahedralCell.h
 *  @author Naohisa Sakamoto
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

    typedef kvs::CellBase BaseClass;

public:

    QuadraticTetrahedralCell( const kvs::UnstructuredVolumeObject* volume );

    void updateInterpolationFunctions( const kvs::Vec3& local ) const;
    void updateDifferentialFunctions( const kvs::Vec3& local ) const;
    kvs::Vec3 randomSampling() const;
    kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__QUADRATIC_TETRAHEDRAL_CELL_H_INCLUDE
