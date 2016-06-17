/*****************************************************************************/
/**
 *  @file   CellByCellSampling.h
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
#pragma once

#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/Camera>
#include <kvs/ObjectBase>
#include <kvs/OpacityMap>
#include <kvs/CellBase>
#include <kvs/ColorMap>
#include <kvs/TetrahedralCell>
#include <kvs/QuadraticTetrahedralCell>
#include <kvs/HexahedralCell>
#include <kvs/QuadraticHexahedralCell>
#include <kvs/PyramidalCell>
#include <kvs/PrismaticCell>
#include <kvs/TrilinearInterpolator>
#include <kvs/CellBase>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

namespace CellByCellSampling
{

/*===========================================================================*/
/**
 *  @brief  Returns an averaged scalar value of the cell.
 *  @param  cell [in] pointer to cell
 *  @return averaged scalar value
 */
/*===========================================================================*/
inline kvs::Real32 AveragedScalar( const kvs::CellBase* cell )
{
    const size_t nnodes = cell->numberOfCellNodes();;
    const kvs::Real32* S = cell->values();
    kvs::Real32 Sa = 0; for ( size_t i = 0; i < nnodes; i++ ) { Sa += S[i]; }
    return Sa / nnodes;
}

/*===========================================================================*/
/**
 *  @brief  Returns a random number in [0,1].
 *  @return random number
 */
/*===========================================================================*/
inline kvs::Real32 RandomNumber()
{
    static kvs::UInt32 x=123456789,y=362436069,z=521288629,w=88675123;
    kvs::UInt32 t;
    t=(x^(x<<11));
    x=y;y=z;z=w;
    w=(w^(w>>19))^(t^(t>>8));
    return w * ( 1.0f / 4294967296.0f );
}

/*===========================================================================*/
/**
 *  @brief  Returns a position of a randomly sampled point in the grid.
 *  @param  base_index [in] base index of the grid
 *  @return poisition of the sampling point
 */
/*===========================================================================*/
inline const kvs::Vec3 RandomSamplingInCube( const kvs::Vec3ui& base_index )
{
    const kvs::Real32 x = RandomNumber();
    const kvs::Real32 y = RandomNumber();
    const kvs::Real32 z = RandomNumber();
    return kvs::Vec3( base_index.x() + x, base_index.y() + y, base_index.z() + z );
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of particles.
 *  @param  density [in] particle density
 *  @param  volume [in] volume of cell
 *  @return number of particles
 */
/*===========================================================================*/
inline size_t NumberOfParticles( const kvs::Real32 density, const kvs::Real32 volume )
{
    const kvs::Real32 R = RandomNumber();
    const kvs::Real32 N = density * volume;
    size_t n = static_cast<size_t>( N );
    if ( N - n > R ) { ++n; }
    return n;
}

/*===========================================================================*/
/**
 *  @brief  Returns cell class.
 *  @param  volume [in] pointer to volume
 *  @return cell class
 */
/*===========================================================================*/
inline kvs::CellBase* Cell( const kvs::UnstructuredVolumeObject* volume )
{
    switch ( volume->cellType() )
    {
    case kvs::UnstructuredVolumeObject::Tetrahedra:
    {
        return new kvs::TetrahedralCell( volume );
    }
    case kvs::UnstructuredVolumeObject::QuadraticTetrahedra:
    {
        return new kvs::QuadraticTetrahedralCell( volume );
    }
    case kvs::UnstructuredVolumeObject::Hexahedra:
    {
        return new kvs::HexahedralCell( volume );
    }
    case kvs::UnstructuredVolumeObject::QuadraticHexahedra:
    {
        return new kvs::QuadraticHexahedralCell( volume );
    }
    case kvs::UnstructuredVolumeObject::Pyramid:
    {
        return new kvs::PyramidalCell( volume );
    }
    case kvs::UnstructuredVolumeObject::Prism:
    {
        return new kvs::PrismaticCell( volume );
    }
    default: break;
    }

    return NULL;
}

/*===========================================================================*/
/**
 *  @brief  Particle class.
 */
/*===========================================================================*/
struct Particle
{
    kvs::Vec3 coord; ///< coordinate value
    kvs::Vec3 normal; ///< normal vector
    kvs::Real32 scalar; ///< scalar value
};

/*===========================================================================*/
/**
 *  @brief  Particles class.
 */
/*===========================================================================*/
class Particles
{
private:
    std::vector<kvs::Real32> m_coords; ///< coorinate value array
    std::vector<kvs::Real32> m_normals; ///< normal vector array
    std::vector<kvs::UInt8> m_colors; ///< color value array

public:
    Particles() {}
    kvs::ValueArray<kvs::Real32> coords() const { return kvs::ValueArray<kvs::Real32>( m_coords ); }
    kvs::ValueArray<kvs::Real32> normals() const { return kvs::ValueArray<kvs::Real32>( m_normals ); }
    kvs::ValueArray<kvs::UInt8> colors() const { return kvs::ValueArray<kvs::UInt8>( m_colors ); }
    void push( const Particle& particle, const kvs::ColorMap& color_map )
    {
        const kvs::RGBColor color = color_map.at( particle.scalar );
        m_coords.push_back( particle.coord.x() );
        m_coords.push_back( particle.coord.y() );
        m_coords.push_back( particle.coord.z() );
        m_normals.push_back( particle.normal.x() );
        m_normals.push_back( particle.normal.y() );
        m_normals.push_back( particle.normal.z() );
        m_colors.push_back( color.r() );
        m_colors.push_back( color.g() );
        m_colors.push_back( color.b() );
    }
};

/*===========================================================================*/
/**
 *  @brief  Particle density map class.
 */
/*===========================================================================*/
class ParticleDensityMap
{
public:
    typedef kvs::ValueArray<kvs::Real32> Table;

private:
    size_t m_resolution; ///< table resolution
    kvs::Real32 m_min_value; ///< min. value
    kvs::Real32 m_max_value; ///< max. value
    Table m_table; ///< value table
    kvs::Real32 m_sampling_step; ///< length of the ray segment (dt)
    kvs::UInt32 m_subpixel_level; ///< subpixel level
    const kvs::Camera* m_camera; ///< pointer to the referenced camera
    const kvs::ObjectBase* m_object; ///< pointer to the referenced object

public:
    void setSamplingStep( const kvs::Real32 step ) { m_sampling_step = step; }
    void setSubpixelLevel( const kvs::UInt32 level ) { m_subpixel_level = level; }
    void attachCamera( const kvs::Camera* camera ) { m_camera = camera; }
    void attachObject( const kvs::ObjectBase* object ) { m_object = object; }
    size_t resolution() const { return m_resolution; }
    kvs::Real32 minValue() const { return m_min_value; }
    kvs::Real32 maxValue() const { return m_max_value; }
    const Table& table() const { return m_table; }
    kvs::Real32 at( const kvs::Real32 value ) const;
    void create( const kvs::OpacityMap& omap );
    template <typename DataType>
    kvs::Real32 maxValueInGrid(
        const kvs::TrilinearInterpolator& grid,
        const kvs::StructuredVolumeObject* volume ) const;
    kvs::Real32 maxValueInCell(
        const kvs::CellBase* cell,
        const kvs::UnstructuredVolumeObject* volume ) const;

private:
    kvs::Real32 max_density( const kvs::Real32 s0, const kvs::Real32 s1 ) const;
};

/*===========================================================================*/
/**
 *  @brief  Returns a maximum density value in the grid.
 *  @param  grid [in] grid
 *  @param  volume [in] pointer to the volume object
 *  @return maximum density
 */
/*===========================================================================*/
template <typename DataType>
inline kvs::Real32 ParticleDensityMap::maxValueInGrid(
    const kvs::TrilinearInterpolator& grid,
    const kvs::StructuredVolumeObject* volume ) const
{
    const DataType* const values = volume->values().asValueArray<DataType>().data();
    const kvs::UInt32* const indices = grid.indices();
    kvs::Real32 smin = static_cast<kvs::Real32>( values[ indices[0] ] );
    kvs::Real32 smax = static_cast<kvs::Real32>( values[ indices[0] ] );
    for ( size_t i = 1; i < 8; i++ )
    {
        const kvs::Real32 s = static_cast<kvs::Real32>( values[ indices[i] ] );
        smin = kvs::Math::Min( smin, s );
        smax = kvs::Math::Max( smax, s );
    }

    return this->max_density( smin, smax );
}

/*===========================================================================*/
/**
 *  @brief  Returns a maximum density value in the cell.
 *  @param  cell [in] cell
 *  @param  volume [in] pointer to the volume object
 *  @return maximum density
 */
/*===========================================================================*/
inline kvs::Real32 ParticleDensityMap::maxValueInCell(
    const kvs::CellBase* cell,
    const kvs::UnstructuredVolumeObject* volume ) const
{
    const kvs::Real32* s = cell->values();
    kvs::Real32 smin = s[0];
    kvs::Real32 smax = s[0];
    const size_t nnodes = volume->numberOfCellNodes();
    for ( size_t i = 1; i < nnodes; i++ )
    {
        smin = kvs::Math::Min( smin, s[i] );
        smax = kvs::Math::Max( smax, s[i] );
    }

    return this->max_density( smin, smax );
}

/*===========================================================================*/
/**
 *  @brief  Sampling class for each grid.
 */
/*===========================================================================*/
template <typename T>
class GridSampler
{
private:
    kvs::TrilinearInterpolator* m_grid; ///< trilinear interpolator
    ParticleDensityMap* m_density_map; ///< particle density map
    Particles m_particles; ///< particles
    Particle m_current; ///< current sampled point
    Particle m_trial; ///< trial point
    kvs::Vec3ui m_base_index; ///< base index of grid

public:
    GridSampler(
        kvs::TrilinearInterpolator* grid,
        ParticleDensityMap* density_map ):
        m_grid( grid ),
        m_density_map( density_map ) {}

    const kvs::TrilinearInterpolator* grid() const { return m_grid; }
    const Particles& particles() const { return m_particles; }

    void bind( const kvs::Vec3ui& base_index ) { m_base_index = base_index; }

    size_t numberOfParticles()
    {
        const kvs::Real32 x = m_base_index.x() + 0.5f;
        const kvs::Real32 y = m_base_index.y() + 0.5f;
        const kvs::Real32 z = m_base_index.z() + 0.5f;
        m_grid->attachPoint( kvs::Vec3( x, y, z ) );

        const kvs::Real32 scalar = m_grid->template scalar<T>();
        const kvs::Real32 density = m_density_map->at( scalar );
        const kvs::Real32 volume = 1.0f;
        return NumberOfParticles( density, volume );
    }

    kvs::Real32 sample()
    {
        m_current.coord = RandomSamplingInCube( m_base_index );
        m_grid->attachPoint( m_current.coord );
        m_current.normal = m_grid->template gradient<T>();
        m_current.scalar = m_grid->template scalar<T>();
        return m_density_map->at( m_current.scalar );
    }

    kvs::Real32 sample( const size_t max_loops )
    {
        kvs::Real32 density = this->sample();
        if ( kvs::Math::IsZero( density ) )
        {
            for ( size_t i = 0; i < max_loops; i++ )
            {
                density = this->sample();
                if ( !kvs::Math::IsZero( density ) ) { break; }
            }
        }

        return density;
    }

    kvs::Real32 trySample()
    {
        m_trial.coord = RandomSamplingInCube( m_base_index );
        m_grid->attachPoint( m_trial.coord );
        m_trial.normal = m_grid->template gradient<T>();
        m_trial.scalar = m_grid->template scalar<T>();
        return m_density_map->at( m_trial.scalar );
    }

    void accept( const kvs::ColorMap& cmap )
    {
        m_particles.push( m_current, cmap );
    }

    void acceptTrial( const kvs::ColorMap& cmap )
    {
        m_particles.push( m_trial, cmap );
        m_current.coord = m_trial.coord;
        m_current.normal = m_trial.normal;
        m_current.scalar = m_trial.scalar;
    }
};

/*===========================================================================*/
/**
 *  @brief  Sampling class for each cell.
 */
/*===========================================================================*/
class CellSampler
{
private:
    kvs::CellBase* m_cell; ///< cell interpolator
    ParticleDensityMap* m_density_map; ///< particle density map
    Particles m_particles; ///< particles
    Particle m_current; ///< current sampled point
    Particle m_trial; ///< trial point

public:
    CellSampler(
        kvs::CellBase* cell,
        ParticleDensityMap* density_map ):
        m_cell( cell ),
        m_density_map( density_map ) {}

    const kvs::CellBase* cell() const { return m_cell; }
    const Particles& particles() const { return m_particles; }
    kvs::Real32 maxDensity() const
    {
        return m_density_map->maxValueInCell( m_cell, m_cell->referenceVolume() );
    }

    void bind( const size_t index ) { m_cell->bindCell( index ); }

    size_t numberOfParticles()
    {
        const kvs::Real32 scalar = AveragedScalar( m_cell );
        const kvs::Real32 density = m_density_map->at( scalar );
        const kvs::Real32 volume = m_cell->volume();
        return NumberOfParticles( density, volume );
    }

    kvs::Real32 sample()
    {
        m_current.coord = m_cell->randomSampling();
        m_current.normal = -m_cell->gradientVector();
        m_current.scalar = m_cell->scalar();
        return m_density_map->at( m_current.scalar );
    }

    kvs::Real32 sample( const kvs::Vec3& coord, const kvs::Vec3& normal, const kvs::Real32 scalar )
    {
        m_current.coord = coord;
        m_current.normal = normal;
        m_current.scalar = scalar;
        return m_density_map->at( m_current.scalar );
    }

    kvs::Real32 sample( const size_t max_loops )
    {
        kvs::Real32 density = this->sample();
        if ( kvs::Math::IsZero( density ) )
        {
            for ( size_t i = 0; i < max_loops; i++ )
            {
                density = this->sample();
                if ( !kvs::Math::IsZero( density ) ) { break; }
            }
        }

        return density;
    }

    kvs::Real32 trySample()
    {
        m_trial.coord = m_cell->randomSampling();
        m_trial.normal = -m_cell->gradientVector();
        m_trial.scalar = m_cell->scalar();
        return m_density_map->at( m_trial.scalar );
    }

    void accept( const kvs::ColorMap& cmap )
    {
        m_particles.push( m_current, cmap );
    }

    void acceptTrial( const kvs::ColorMap& cmap )
    {
        m_particles.push( m_trial, cmap );
        m_current.coord = m_trial.coord;
        m_current.normal = m_trial.normal;
        m_current.scalar = m_trial.scalar;
    }
};

} // end of namespace CellByCellSampling

} // end of namespace kvs
