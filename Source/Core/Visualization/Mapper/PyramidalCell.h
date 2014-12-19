/****************************************************************************/
/**
 *  @file   PyramidalCell.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PyramidalCell.h 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__PYRAMIDAL_CELL_H_INCLUDE
#define KVS__PYRAMIDAL_CELL_H_INCLUDE

#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/CellBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Pyramidal cell class.
 */
/*===========================================================================*/
class PyramidalCell : public kvs::CellBase
{
public:

    enum { NumberOfNodes = 5 };
    typedef kvs::CellBase BaseClass;

private:

    kvs::Vec3 m_pyramid; // x,y: length of bottom plane, z: height of pyramid

public:

    PyramidalCell( const kvs::UnstructuredVolumeObject* volume );
    virtual ~PyramidalCell();

    const kvs::Real32* interpolationFunctions( const kvs::Vec3& point ) const;
    const kvs::Real32* differentialFunctions( const kvs::Vec3& point ) const;
    void bindCell( const kvs::UInt32 cell );
    const kvs::Vec3 randomSampling() const;
    const kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__PYRAMIDAL_CELL_H_INCLUDE
