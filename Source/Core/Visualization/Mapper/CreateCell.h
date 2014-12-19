/*****************************************************************************/
/**
 *  @file   CreateCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CreateCell.h 1332 2012-10-13 09:36:23Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/SharedPointer>
#include <kvs/Assert>


namespace kvs
{

namespace cell
{

namespace detail
{

template <typename CellType>
struct CellTraits
{
    static const int NumberOfNodes = CellType::NumberOfNodes;
    static const int CoordsSize = NumberOfNodes * 3;
    static const int InterpolationFactorsSize = NumberOfNodes;
    static const int DifferentialFactorsSize = NumberOfNodes * 3;
};

template <typename T>
struct CellTraits<BasicTetrahedralCell<T> >
{
    static const int NumberOfNodes = BasicTetrahedralCell<T>::NumberOfNodes;
    static const int CoordsSize = NumberOfNodes * 3;
    static const int InterpolationFactorsSize = NumberOfNodes;
    static const int DifferentialFactorsSize = 0;
};

template <typename T>
struct CellTraits<BasicCubicCell<T> >
{
    static const int NumberOfNodes = BasicCubicCell<T>::NumberOfNodes;
    static const int CoordsSize = 3;
    static const int InterpolationFactorsSize = NumberOfNodes;
    static const int DifferentialFactorsSize = NumberOfNodes * 3;
};

template <typename CellType>
struct CellGenerator
{
    static CellType* Generate( const kvs::VolumeObjectBase* volume )
    {
        typedef typename CellType::ValueType ValueType;
        const int nnodes = CellType::NumberOfNodes;

        const int base_size                  = ( sizeof( CellType ) - 1 ) / sizeof( ValueType ) + 1;
        const int coords_size                = CellTraits<CellType>::CoordsSize;
        const int interpolation_factors_size = CellTraits<CellType>::InterpolationFactorsSize;
        const int differential_factors_size  = CellTraits<CellType>::DifferentialFactorsSize;
        const int values_size                = nnodes * volume->veclen();

        const int total_size = base_size + coords_size + interpolation_factors_size + differential_factors_size + values_size;

        ValueType* buffer = new ValueType [ total_size ];
        CellType* cell = new ( buffer ) CellType( volume );
        KVS_ASSERT( (void*)cell == (void*)buffer );

        cell->m_coords                = buffer + base_size;
        cell->m_interpolation_factors = cell->m_coords + coords_size;
        cell->m_differential_factors  = cell->m_interpolation_factors + interpolation_factors_size;
        cell->m_values                = cell->m_differential_factors + differential_factors_size;
        return cell;
    }

    static void Dispose( CellType* cell )
    {
        typedef typename CellType::ValueType ValueType;
        cell->~CellType();
        delete [] ( (ValueType*)(void*)cell );
    }
};

} // end of namespace detail

template <typename CellType>
kvs::SharedPointer<CellType> CreateCell( const kvs::VolumeObjectBase* volume )
{
    typedef kvs::cell::detail::CellGenerator<CellType> generator;
    kvs::SharedPointer<CellType> ret( generator::Generate( volume ), generator::Dispose );
    return ret;
}

} // end of namespace cell

} // end of namespace kvs
