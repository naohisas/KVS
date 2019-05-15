/*****************************************************************************/
/**
 *  @file   CellByCellLayeredSampling.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellByCellLayeredSampling.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "CellByCellLayeredSampling.h"
#include <vector>
#include <kvs/OpenMP>
#include <kvs/DebugNew>
#include <kvs/Camera>
#include <kvs/ValueArray>
#include <kvs/CellBase>
#include <kvs/TetrahedralCell>
#include <kvs/MersenneTwister>
#include "CellByCellSampling.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Transformation matrices.
 */
/*===========================================================================*/
class Matrices
{
private:
    kvs::Vec3 m_v[4]; ///< vertices of tetrahedral cell
    kvs::Vec4 m_Lv[4]; ///< vertices transformed with L matrix
    kvs::Real32 m_smin; ///< minimum value
    kvs::Real32 m_smax; ///< maximum value
    kvs::Mat4 m_L; ///< transformation matrix L
    kvs::Mat4 m_A; ///< transformation matrix A

public:
    Matrices(
        const kvs::TetrahedralCell* cell,
        const kvs::Real32 min_value,
        const kvs::Real32 max_value )
    {
        const kvs::Vec3* v = cell->coords();
        m_v[0] = v[0];
        m_v[1] = v[1];
        m_v[2] = v[2];
        m_v[3] = v[3];

        const kvs::Real32* s = cell->values();
        const kvs::Real32 smin = kvs::Math::Min( s[0], s[1], s[2], s[3] );
        const kvs::Real32 smax = kvs::Math::Max( s[0], s[1], s[2], s[3] );
        m_smin = ( smin - min_value ) / ( max_value - min_value );
        m_smax = ( smax - min_value ) / ( max_value - min_value );

        this->calculate_L_matrix( cell );
        this->calculate_A_matrix( cell );
    }

    //kvs::Vec4 v( const size_t index ) const { return kvs::Vec4( m_v[index], 1.0f ); }
    kvs::Vec4 Lv( const size_t index ) const { return m_Lv[index]; }
    //const kvs::Mat4& L() const { return m_L; }
    const kvs::Mat4& A() const { return m_A; }

    kvs::Mat4 invL() const { return m_L.transposed(); }
    kvs::Mat4 invA() const
    {
        const kvs::Real32 a1 = m_A[0][0];
        const kvs::Real32 a2 = m_A[1][1];
        const kvs::Real32 a3 = m_A[2][2];
        const kvs::Real32 b1 = m_A[0][3];
        const kvs::Real32 b2 = m_A[1][3];
        const kvs::Real32 b3 = m_A[2][3];
        return kvs::Mat4(
            1.0f/a1, 0.0f,    0.0f,    -b1/a1,
            0.0f,    1.0f/a2, 0.0f,    -b2/a2,
            0.0f,    0.0f,    1.0f/a3, -b3/a3,
            0.0f,    0.0f,    0.0f,    1.0f    );
    }
    kvs::Real32 detA() const
    {
        const kvs::Real32 a1 = m_A[0][0];
        const kvs::Real32 a2 = m_A[1][1];
        const kvs::Real32 a3 = m_A[2][2];
        return kvs::Math::Abs( a1 * a2 * a3 );
    }

private:
    void calculate_L_matrix( const kvs::TetrahedralCell* cell )
    {
        const kvs::Vec3 g = cell->gradientVector().normalized();
        const kvs::Vec3 u = this->basis_vector( g );
        const kvs::Vec3 t1 = g.cross( u ).normalized();
        const kvs::Vec3 t2 = g.cross( t1 ).normalized();

        m_L = kvs::Mat4(
            t1.x(), t1.y(), t1.z(), 0.0f,
            t2.x(), t2.y(), t2.z(), 0.0f,
            g.x(),  g.y(),  g.z(),  0.0f,
            0.0f,   0.0f,   0.0f,   1.0f );
    }

    void calculate_A_matrix( const kvs::TetrahedralCell* cell )
    {
        const kvs::Vec4 v0 = m_L * kvs::Vec4( m_v[0], 1.0f );
        const kvs::Vec4 v1 = m_L * kvs::Vec4( m_v[1], 1.0f );
        const kvs::Vec4 v2 = m_L * kvs::Vec4( m_v[2], 1.0f );
        const kvs::Vec4 v3 = m_L * kvs::Vec4( m_v[3], 1.0f );

        const kvs::Real32 xmin = kvs::Math::Min( v0.x(), v1.x(), v2.x(), v3.x() );
        const kvs::Real32 xmax = kvs::Math::Max( v0.x(), v1.x(), v2.x(), v3.x() );
        const kvs::Real32 ymin = kvs::Math::Min( v0.y(), v1.y(), v2.y(), v3.y() );
        const kvs::Real32 ymax = kvs::Math::Max( v0.y(), v1.y(), v2.y(), v3.y() );
        const kvs::Real32 zmin = kvs::Math::Min( v0.z(), v1.z(), v2.z(), v3.z() );
        const kvs::Real32 zmax = kvs::Math::Max( v0.z(), v1.z(), v2.z(), v3.z() );

        const kvs::Real32 a1 = 1.0f / ( xmax - xmin );
        const kvs::Real32 a2 = 1.0f / ( ymax - ymin );
        const kvs::Real32 a3 = ( m_smax - m_smin ) / ( zmax - zmin );
        const kvs::Real32 b1 = -a1 * xmin;
        const kvs::Real32 b2 = -a2 * ymin;
        const kvs::Real32 b3 = -a3 * zmin + m_smin;

        m_Lv[0] = v0;
        m_Lv[1] = v1;
        m_Lv[2] = v2;
        m_Lv[3] = v3;

        m_A = kvs::Mat4(
            a1,   0.0f, 0.0f, b1,
            0.0f, a2,   0.0f, b2,
            0.0f, 0.0f, a3,   b3,
            0.0f, 0.0f, 0.0f, 1.0f );
    }

    size_t primary_axis( const kvs::Vec3& g )
    {
        size_t axis = 0;
        axis = kvs::Math::Abs( g[0] ) < kvs::Math::Abs( g[1] ) ? 0 : 1;
        axis = kvs::Math::Abs( g[axis] ) < kvs::Math::Abs( g[2] ) ? axis : 2;
        return axis;
    }

    kvs::Vec3 basis_vector( const kvs::Vec3& g )
    {
        kvs::Vec3 basis = kvs::Vec3::Zero();
        basis[ this->primary_axis( g ) ] = 1.0f;
        return basis;
    }
};

/*===========================================================================*/
/**
 *  @brief  Returns true if the specified four values are same.
 *  @param  s0 [in] scalar value 0
 *  @param  s1 [in] scalar value 1
 *  @param  s2 [in] scalar value 2
 *  @param  s3 [in] scalar value 3
 *  @return true if the specified four values are same.
 */
/*===========================================================================*/
inline bool Equal( kvs::Real32 s0, kvs::Real32 s1, kvs::Real32 s2, kvs::Real32 s3 )
{
    return kvs::Math::Equal( s0, s1 ) && kvs::Math::Equal( s1, s2 ) && kvs::Math::Equal( s2, s3 );
}

/*===========================================================================*/
/**
 *  @brief  Pregenerate particles in a unit cube.
 *  @param  nparticles [in] target number of particles
 *  @param  density_map [in] particle density map
 */
/*===========================================================================*/
inline kvs::PointObject* PregenerateParticles(
    const size_t nparticles,
    const kvs::CellByCellSampling::ParticleDensityMap& density_map,
    kvs::Real32* integral )
{
    const size_t resolution = density_map.resolution();
    const size_t nintervals = resolution - 1;

    // Integrate the density distribution function as piecewise linear function.
    kvs::ValueArray<float> m( nintervals ); m.fill( 0 );
    float M = 0.0f;
    for ( size_t i = 0; i < nintervals; i++ )
    {
        const float p0 = density_map.table().at(i);
        const float p1 = density_map.table().at(i+1);
        const float s0 = static_cast<float>(i) / nintervals;
        const float s1 = static_cast<float>(i+1) / nintervals;

        m[i] = 0.5f * ( p0 + p1 ) * ( s1 - s0 );
        M += m[i];
    }

    // Set the M value.
    *integral = M;

    // Calculate the number of particles in each interval.
    kvs::MersenneTwister R; R.setSeed();
    kvs::ValueArray<kvs::UInt32> n( nintervals ); n.fill( 0 );
    kvs::UInt32 N = 0; // total number of particles
    for ( size_t i = 0; i < nintervals; i++ )
    {
        const float tmp = static_cast<float>( nparticles ) * m[i] / M;

        n[i] = static_cast<kvs::UInt32>( tmp );
        if ( tmp - n[i] > R() ) n[i] = n[i] + 1;

        N += n[i];
    }

    // Generate particles by using rejection method.
    kvs::ValueArray<kvs::Real32> coords( N * 3 );
    kvs::Real32* pcoords = coords.data();
    for ( size_t i = 0; i < nintervals; i++ )
    {
        const float p0 = density_map.table().at(i);
        const float p1 = density_map.table().at(i+1);
        const float s0 = static_cast<float>(i) / nintervals;
        const float s1 = static_cast<float>(i+1) / nintervals;

        const float a = ( p1 - p0 ) / ( s1 - s0 );
        const float b = ( p0 * s1 - p1 * s0 ) / ( s1 - s0 );
        const float c = kvs::Math::Max( p0, p1 );

        size_t counter = 0;
        while ( counter < n[i] )
        {
            const float s = static_cast<float>( R() ) * ( s1 - s0 ) + s0;
            const float g = a * s + b;
            if ( R() > 1.0F / c * g ) { continue; } // Reject.
            else
            {
                // Accept.
                const float t1 = static_cast<float>( R() );
                const float t2 = static_cast<float>( R() );
                *(pcoords++) = t1;
                *(pcoords++) = t2;
                *(pcoords++) = s;
                counter++;
            }
        }
    }

    kvs::PointObject* particles = new kvs::PointObject();
    particles->setCoords( coords );
    return particles;
}

/*===========================================================================*/
/**
 *  @brief  Returns indices of particles in a cell.
 *  @param  cell [in] tetrahedral cell
 *  @param  particles [in] pre-generated particles
 *  @param  matrices [in] transformation matrices
 */
/*===========================================================================*/
inline kvs::ValueArray<kvs::UInt32> ParticlesInCell(
    const kvs::TetrahedralCell* cell,
    const kvs::PointObject* particles,
    const Matrices& matrices )
{
    const kvs::Vec4 g0 = matrices.A() * matrices.Lv(0);
    const kvs::Vec4 g1 = matrices.A() * matrices.Lv(1);
    const kvs::Vec4 g2 = matrices.A() * matrices.Lv(2);
    const kvs::Vec4 g3 = matrices.A() * matrices.Lv(3);
    const kvs::Vec3 d1 = ( g1 - g0 ).xyz();
    const kvs::Vec3 d2 = ( g3 - g0 ).xyz();
    const kvs::Vec3 d3 = ( g2 - g0 ).xyz();
    const kvs::Mat3 D = kvs::Mat3( d1, d2, d3 ).transposed().inverted();

    std::vector<kvs::UInt32> indices;
    const size_t nparticles = particles->numberOfVertices();
    for ( size_t i = 0, i3 = 0; i < nparticles; i++, i3 += 3 )
    {
        const kvs::Vec3 v = particles->coord(i) - g0.xyz();
        const kvs::Vec3 lambda = D * v;
        if ( lambda[0] >= 0.0f && lambda[1] >= 0.0f && lambda[2] >= 0.0f )
        {
            if ( lambda[0] + lambda[1] + lambda[2] <= 1.0f )
            {
                indices.push_back( i );
            }
        }
    }

    return kvs::ValueArray<kvs::UInt32>( indices );
}

/*===========================================================================*/
/**
 *  @brief  Returns actual number of particles.
 *  @param  nparticles_in_cell [in] number of particles in cell
 *  @param  nparticles [in] number of pregenerated particles
 *  @param  integral [in] integral of particle density function
 *  @param  matrices [in] transformation matrices
 *  @return required number of particles
 */
/*===========================================================================*/
inline size_t ActualNumberOfParticles(
    const size_t nparticles_in_cell,
    const size_t nparticles,
    const kvs::Real32 integral,
    const Matrices& matrices )
{
    const size_t N_in = nparticles_in_cell;
    const size_t N_all = nparticles;

    const float detA_inv = 1.0f / matrices.detA();
    const float N = detA_inv * integral * N_in / N_all;
    const float R = kvs::CellByCellSampling::RandomNumber();

    size_t n = static_cast<size_t>( N );
    if ( N - n > R ) { ++n; }

    return n;
}

/*===========================================================================*/
/**
 *  @brief  Uniform sampling.
 *  @param  nparticles [in] number of particles
 *  @param  sampler [int] sampler
 *  @param  particle_index_counter [in/out] reference to particle index counter
 *  @param  particles [out] colored particles
 */
/*===========================================================================*/
inline void UniformSampling(
    const size_t nparticles,
    kvs::CellByCellSampling::CellSampler& sampler,
    size_t& particle_index_counter,
    kvs::CellByCellSampling::ColoredParticles& particles )
{
    for ( size_t i = 0; i < nparticles; ++i )
    {
        sampler.sample();
        const kvs::CellByCellSampling::Particle& p = sampler.accept();
        const size_t particle_index = particle_index_counter++;
        particles.push( particle_index, p );
    }
}

/*===========================================================================*/
/**
 *  @brief  Rejection sampling.
 *  @param  nparticles [in] number of particles
 *  @param  sampler [int] sampler
 *  @param  particle_index_counter [in/out] reference to particle index counter
 *  @param  particles [out] colored particles
 */
/*===========================================================================*/
inline void RejectionSampling(
    const size_t nparticles,
    kvs::CellByCellSampling::CellSampler& sampler,
    size_t& particle_index_counter,
    kvs::CellByCellSampling::ColoredParticles& particles )
{
    const kvs::Real32 max_density = sampler.maxDensity();
    const kvs::Real32 pmax = max_density / nparticles;

    size_t counter = 0;
    while ( counter < nparticles )
    {
        const kvs::Real32 density = sampler.sample();
        const kvs::Real32 p = density / nparticles;
        const kvs::Real32 R = kvs::CellByCellSampling::RandomNumber();
        if ( p > pmax * R )
        {
            const kvs::CellByCellSampling::Particle& p = sampler.accept();
            const size_t particle_index = particle_index_counter++;
            particles.push( particle_index, p );
            counter++;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Roulette selection.
 *  @param  nparticles [in] number of particles
 *  @param  pregenerated_particles [in] pre-generated particles
 *  @param  matrices [in] transformation matrices
 *  @param  indices [in] indices of particles in a cell
 *  @param  sampler [int] sampler
 *  @param  particle_index_counter [in/out] reference to particle index counter
 *  @param  particles [out] colored particles
 */
/*===========================================================================*/
inline void RouletteSelection(
    const size_t nparticles,
    const kvs::PointObject* pregenerated_particles,
    const Matrices& matrices,
    const kvs::ValueArray<kvs::UInt32>& indices,
    kvs::CellByCellSampling::CellSampler& sampler,
    size_t& particle_index_counter,
    kvs::CellByCellSampling::ColoredParticles& particles )
{
    const kvs::Vec3 normal = -sampler.cell()->gradientVector().normalized();
    const kvs::Mat4 invLA = matrices.invL() * matrices.invA();
    const kvs::Real32 min_value = sampler.cell()->referenceVolume()->minValue();
    const kvs::Real32 max_value = sampler.cell()->referenceVolume()->maxValue();
    for ( size_t i = 0; i < nparticles; i++ )
    {
        const kvs::Real32 fid = kvs::CellByCellSampling::RandomNumber() * indices.size();
        const kvs::UInt32 id = indices[ int( fid ) ];
        const kvs::Vec4 selected_particle( pregenerated_particles->coord( id ), 1.0f  );

        const kvs::Vec4 coord( invLA * selected_particle );
        const kvs::Real32 scalar = kvs::Math::Mix( min_value, max_value, selected_particle.z() );

        // Set coord, color, and normal to the point object.
        sampler.sample( coord.xyz(), normal, scalar );
        const kvs::CellByCellSampling::Particle& p = sampler.accept();
        const size_t particle_index = particle_index_counter++;
        particles.push( particle_index, p );
    }
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellLayeredSampling class.
 */
/*===========================================================================*/
CellByCellLayeredSampling::CellByCellLayeredSampling():
    kvs::MapperBase(),
    kvs::PointObject(),
    m_camera( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellLayeredSampling class.
 *  @param  volume [in] pointer to the volume object
 *  @param  repetition_level [in] repetition level
 *  @param  sampling_step [in] sapling step
 *  @param  transfer_function [in] transfer function
 *  @param  object_depth [in] depth value of the input volume at the CoG
 */
/*===========================================================================*/
CellByCellLayeredSampling::CellByCellLayeredSampling(
    const kvs::VolumeObjectBase* volume,
    const size_t repetition_level,
    const float sampling_step,
    const kvs::TransferFunction& transfer_function,
    const float object_depth ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject(),
    m_camera( 0 )
{
    this->setRepetitionLevel( repetition_level );
    this->setSamplingStep( sampling_step );
    this->setObjectDepth( object_depth );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new CellByCellLayeredSampling class.
 *  @param  camera [in] pointer to the camera
 *  @param  volume [in] pointer to the volume object
 *  @param  repetition_level [in] repetition level
 *  @param  sampling_step [in] sapling step
 *  @param  transfer_function [in] transfer function
 *  @param  object_depth [in] depth value of the input volume at the CoG
 */
/*===========================================================================*/
CellByCellLayeredSampling::CellByCellLayeredSampling(
    const kvs::Camera* camera,
    const kvs::VolumeObjectBase* volume,
    const size_t repetition_level,
    const float sampling_step,
    const kvs::TransferFunction& transfer_function,
    const float object_depth ):
    kvs::MapperBase( transfer_function ),
    kvs::PointObject(),
    m_camera( 0 )
{
    this->attachCamera( camera );
    this->setRepetitionLevel( repetition_level );
    this->setSamplingStep( sampling_step );
    this->setObjectDepth( object_depth );
    this->exec( volume );
}

/*===========================================================================*/
/**
 *  @brief  Executes the mapper process.
 *  @param  object [in] pointer to the volume object
 *  @return pointer to the point object
 */
/*===========================================================================*/
CellByCellLayeredSampling::SuperClass* CellByCellLayeredSampling::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::VolumeObjectBase* volume = kvs::VolumeObjectBase::DownCast( object );
    if ( !volume )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume dat.");
        return NULL;
    }

    bool delete_camera = false;
    if ( !m_camera )
    {
        m_camera = new kvs::Camera();
        delete_camera = true;
    }

    const kvs::VolumeObjectBase::VolumeType volume_type = volume->volumeType();
    if ( volume_type == kvs::VolumeObjectBase::Unstructured )
    {
        this->mapping( kvs::UnstructuredVolumeObject::DownCast( object ) );
    }

    if ( delete_camera )
    {
        delete m_camera;
        m_camera = 0;
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Mapping for the unstructured volume object (tetrahedral cell data only).
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellLayeredSampling::mapping( const kvs::UnstructuredVolumeObject* volume )
{
    if ( volume->cellType() != kvs::UnstructuredVolumeObject::Tetrahedra )
    {
        BaseClass::setSuccess( false );
        kvsMessageError( "Cell type is not tetrahedra." );
        return;
    }

    // Attach the pointer to the volume object and set the min/max coordinates.
    BaseClass::attachVolume( volume );
    BaseClass::setRange( volume );
    BaseClass::setMinMaxCoords( volume, this );

    // Generate the particles.
    this->generate_particles( volume );
}

/*===========================================================================*/
/**
 *  @brief  Generates particles for the unstructured volume object.
 *  @param  volume [in] pointer to the input volume object
 */
/*===========================================================================*/
void CellByCellLayeredSampling::generate_particles( const kvs::UnstructuredVolumeObject* volume )
{
    CellByCellSampling::ParticleDensityMap density_map;
    density_map.setSamplingStep( m_sampling_step );
    density_map.attachCamera( m_camera );
    density_map.attachObject( volume );
    density_map.create( BaseClass::transferFunction().opacityMap() );

    kvs::Real32 integral = 0.0f;
    kvs::PointObject* pregenerated_particles = ::PregenerateParticles( 800000, density_map, &integral );

    const size_t resolution = density_map.resolution();
    const float tiny_value = 1.0f / resolution;

    const size_t ncells = volume->numberOfCells();
    const kvs::ColorMap color_map( BaseClass::transferFunction().colorMap() );

    // Calculate number of particles
    size_t N = 0;
    kvs::ValueArray<kvs::UInt32> nparticles( ncells );
    KVS_OMP_PARALLEL()
    {
        kvs::TetrahedralCell* cell = new kvs::TetrahedralCell( volume );
        CellByCellSampling::CellSampler sampler( cell, &density_map );

        KVS_OMP_FOR( reduction(+:N) )
        for ( size_t index = 0; index < ncells; ++index )
        {
            sampler.bind( index );

            size_t n = 0;
            const kvs::Real32* s = sampler.cell()->values();
            const kvs::Real32 smin = kvs::Math::Min( s[0], s[1], s[2], s[3] );
            const kvs::Real32 smax = kvs::Math::Max( s[0], s[1], s[2], s[3] );
            if ( ::Equal( s[0], s[1], s[2], s[3] ) )
            {
                n = sampler.numberOfParticles();
            }
            else if ( smax - smin < tiny_value )
            {
                n = sampler.numberOfParticles();
            }
            else
            {
                const kvs::Real32 min_value = density_map.minValue();
                const kvs::Real32 max_value = density_map.maxValue();
                ::Matrices matrices( cell, min_value, max_value );

                typedef kvs::ValueArray<kvs::UInt32> Indices;
                const Indices indices = ::ParticlesInCell( cell, pregenerated_particles, matrices );
                const size_t Nin = indices.size();
                const size_t Nall = pregenerated_particles->numberOfVertices();
                const size_t Ntet = ::ActualNumberOfParticles( Nin, Nall, integral, matrices );
                n = Ntet;
            }

            nparticles[index] = n;

            N += n;
        }

        delete cell;
    }

    // Generate particles for each cell.
    const kvs::UInt32 repetitions = m_repetition_level;
    CellByCellSampling::ColoredParticles particles( color_map );
    particles.allocate( N * repetitions );
    KVS_OMP_PARALLEL()
    {
        kvs::TetrahedralCell* cell = new kvs::TetrahedralCell( volume );
        CellByCellSampling::CellSampler sampler( cell, &density_map );

        KVS_OMP_FOR( schedule(dynamic) )
        for ( kvs::UInt32 r = 0; r < repetitions; ++r )
        {
            size_t particle_index_counter = N * r;
            for ( size_t index = 0; index < ncells; ++index )
            {
                const size_t n = nparticles[index];
                if ( n == 0 ) continue;

                sampler.bind( index );

                const kvs::Real32* s = sampler.cell()->values();
                const kvs::Real32 smin = kvs::Math::Min( s[0], s[1], s[2], s[3] );
                const kvs::Real32 smax = kvs::Math::Max( s[0], s[1], s[2], s[3] );

                if ( ::Equal( s[0], s[1], s[2], s[3] ) )
                {
                    ::UniformSampling( n, sampler, particle_index_counter, particles );
                }
                else if ( smax - smin < tiny_value )
                {
                    ::RejectionSampling( n, sampler, particle_index_counter, particles );
                }
                else
                {
                    const kvs::Real32 min_value = density_map.minValue();
                    const kvs::Real32 max_value = density_map.maxValue();
                    ::Matrices matrices( cell, min_value, max_value );

                    typedef kvs::ValueArray<kvs::UInt32> Indices;
                    const Indices indices = ::ParticlesInCell( cell, pregenerated_particles, matrices );
                    const size_t Nin = indices.size();
                    const size_t Ntet = n;
                    if ( Nin > Ntet )
                    {
                        ::RouletteSelection( Ntet, pregenerated_particles, matrices, indices, sampler,
                                             particle_index_counter, particles );
                    }
                    else
                    {
                        ::RouletteSelection( Nin, pregenerated_particles, matrices, indices, sampler,
                                             particle_index_counter, particles );
                        ::RejectionSampling( Ntet - Nin, sampler, particle_index_counter, particles );
                    }
                }
            }
        }

        delete cell;
    }

    delete pregenerated_particles;

    SuperClass::setCoords( particles.coords() );
    SuperClass::setColors( particles.colors() );
    SuperClass::setNormals( particles.normals() );
    SuperClass::setSize( 1.0f );
}

} // end of namespace kvs
