/*****************************************************************************/
/**
 *  @file   ProbabilisticMarchingCubes.cpp
 *  @author Go Tamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Kai Pöthkow, Britta Weber, Hans-Christian Hege, Probabilistic Marching Cubes,
 *  Computer Graphics Forum, Volume 30, Issue 3, pp.931-940, 2011.
 */
/*****************************************************************************/
#include "ProbabilisticMarchingCubes.h"
#include <kvs/Vector3>
#include <kvs/InverseDistanceWeighting>
#include <random>


namespace
{

class OnlineCov
{
public:
    using Value = float;

private:
    double m_mean1 = 0.0;
    double m_mean2 = 0.0;
    double m_cov = 0.0;
    size_t m_dim = 0;

public:
    OnlineCov() = default;
    void add( const Value value1, const Value value2 )
    {
        m_dim++;
        const double factor = 1.0 / static_cast<double>( m_dim );
        const double delta1 = ( value1 - m_mean1 ) * factor;
        const double delta2 = ( value2 - m_mean2 ) * factor;
        m_mean1 += delta1;
        m_mean2 += delta2;
        m_cov += ( m_dim - 1 ) * delta1 * delta2 - m_cov * factor;
    }
    Value average() const { return m_mean2; }
    Value covariance() const { return m_cov * m_dim / static_cast<double>( m_dim - 1 ); }
};

class OnlineCovMatrix
{
public:
    using Cov = std::vector<OnlineCov>;
    using Array = std::vector<OnlineCov::Value>;
    static size_t Size() { return 8; }

private:
    const size_t m_dim = Size();
    Cov m_cov{ ( m_dim + 1 ) * m_dim / 2 };

public:
    OnlineCovMatrix() = default;

    void add( const Array& a )
    {
        /*
         *  Covariance index
         *    a0  a1  a2  a3
         * a0  0   1   2   3
         * a1      4   5   6
         * a2          7   8
         * a3              9
         */
        for ( size_t i = 0, index = 0; i < m_dim; ++i )
        {
            for ( size_t j = i; j < m_dim; ++j, ++index )
            {
                m_cov[index].add( a[i], a[j] );
            }
        }
    }

    Array average() const
    {
        Array ret( m_dim );
        for ( size_t i = 0; i < m_dim; ++i)
        {
            ret[i] = m_cov[i].average();
        }
        return ret;
    }

    Array covariance() const
    {
        Array ret( m_dim * m_dim );
        for ( size_t i = 0, index = 0; i < m_dim; ++i )
        {
            for ( size_t j = i; j < m_dim; ++j, ++index )
            {
                ret[ m_dim * i + j ] = m_cov[index].covariance();
                ret[ m_dim * j + i ] = ret[ m_dim * i + j ];
            }
        }
        return ret;
    }
};

class OnlineCovVolume
{
public:
    using Cov = std::vector<OnlineCovMatrix>;
    using Matrix = std::vector<OnlineCovMatrix::Array>;

private:
    const kvs::Vec3ui m_dim{ 0, 0, 0 };
    Cov m_cov{};

public:
    OnlineCovVolume( const kvs::Vec3ui& dim ):
        m_dim( dim ),
        m_cov( dim.x() * dim.y() * dim.z() ) {}

    void add( const kvs::AnyValueArray& array )
    {
        switch ( array.typeID() )
        {
        case kvs::Type::TypeInt8:   { this->add( array.asValueArray<kvs::Int8>()   ); return; }
        case kvs::Type::TypeInt16:  { this->add( array.asValueArray<kvs::Int16>()  ); return; }
        case kvs::Type::TypeInt32:  { this->add( array.asValueArray<kvs::Int32>()  ); return; }
        case kvs::Type::TypeInt64:  { this->add( array.asValueArray<kvs::Int64>()  ); return; }
        case kvs::Type::TypeUInt8:  { this->add( array.asValueArray<kvs::UInt8>()  ); return; }
        case kvs::Type::TypeUInt16: { this->add( array.asValueArray<kvs::UInt16>() ); return; }
        case kvs::Type::TypeUInt32: { this->add( array.asValueArray<kvs::UInt32>() ); return; }
        case kvs::Type::TypeUInt64: { this->add( array.asValueArray<kvs::UInt64>() ); return; }
        case kvs::Type::TypeReal32: { this->add( array.asValueArray<kvs::Real32>() ); return; }
        case kvs::Type::TypeReal64: { this->add( array.asValueArray<kvs::Real64>() ); return; }
        default: break;
        }
    }

    kvs::ValueArray<float> average0() const
    {
        const size_t size = m_dim.x() * m_dim.y() * m_dim.z();
        kvs::ValueArray<float> ret( size );
        for ( size_t i = 0; i < size; ++i)
        {
            ret[i] = m_cov[i].average()[0];
        }
        return ret;
    }

    kvs::ValueArray<float> covariance0() const
    {
        const size_t size = m_dim.x() * m_dim.y() * m_dim.z();
        kvs::ValueArray<float> ret( size );
        for ( size_t i = 0; i < size; ++i)
        {
            ret[i] = m_cov[i].covariance()[0];
        }
        return ret;
    }

    Matrix average() const
    {
        const size_t size = m_dim.x() * m_dim.y() * m_dim.z();
        Matrix ret( size );
        for ( size_t  i = 0; i < size; ++i )
        {
            ret[i] = m_cov[i].average();
        }
        return ret;
    }

    Matrix covariance() const
    {
        const size_t n = OnlineCovMatrix::Size();
        const size_t size = m_dim.x() * m_dim.y() * m_dim.z();
        Matrix ret( size, OnlineCovMatrix::Array( n * n ) );
        size_t index = 0;
        for ( auto&& i : ret )
        {
            i = m_cov[index++].covariance();
        }
        return ret;
    }

    Matrix choleskyCovariance()
    {
        const size_t n = OnlineCovMatrix::Size();
        const size_t size = m_dim.x() * m_dim.y() * m_dim.z();
        Matrix ret( size, OnlineCovMatrix::Array( n * n ) );
        size_t index = 0;
        for ( auto&& cov : ret )
        {
            cov = m_cov[index++].covariance();
            cov = this->cholesky_decomposition( cov );
        }
        return ret;
    }

private:
    template <typename T>
    void add( const kvs::ValueArray<T>& array )
    {
        const auto dimx = m_dim.x() + 1;
        const auto dimy = m_dim.y() + 1;
        const auto dimxy = dimx * dimy;
        for ( size_t i = 0, index = 0; i < m_dim.z(); ++i )
        {
            for ( size_t j = 0; j < m_dim.y(); ++j )
            {
                for ( size_t k = 0; k < m_dim.x(); ++k, ++index )
                {
                    OnlineCovMatrix::Array values = {
                        static_cast<float>( array[ dimxy * i       + dimx * j       + k     ] ),
                        static_cast<float>( array[ dimxy * i       + dimx * j       + k + 1 ] ),
                        static_cast<float>( array[ dimxy * i       + dimx * (j + 1) + k     ] ),
                        static_cast<float>( array[ dimxy * i       + dimx * (j + 1) + k + 1 ] ),
                        static_cast<float>( array[ dimxy * (i + 1) + dimx * j       + k     ] ),
                        static_cast<float>( array[ dimxy * (i + 1) + dimx * j       + k + 1 ] ),
                        static_cast<float>( array[ dimxy * (i + 1) + dimx * (j + 1) + k     ] ),
                        static_cast<float>( array[ dimxy * (i + 1) + dimx * (j + 1) + k + 1 ] ),
                    };
                    m_cov[index].add( values );
                }
            }
        }
    }

    OnlineCovMatrix::Array cholesky_decomposition( const OnlineCovMatrix::Array& cov )
    {
        const size_t n = OnlineCovMatrix::Size();
        const float epsilon = 0.000001;

        OnlineCovMatrix::Array l( n * n );
        for ( size_t i = 0; i < n; ++i )
        {
            for ( size_t k = 0; k < (i + 1); ++k )
            {
                double sum = 0;
                for ( size_t j = 0; j < k; ++j )
                {
                    sum += l[ i * n + j ] * l[ k * n + j ];
                }
                if ( i == k )
                {
                    l[ i * n + k ] = std::sqrt( cov[ i * n + i ] + epsilon - sum );
                }
                else
                {
                    l[ i * n + k ] = 1.0 / l[ k * n + k ] * ( cov[ i * n + k ] - sum );
                }
            }
        }
        return l;
    }
};

inline bool Crossing( const std::vector<float>& y, const float threshold )
{
    if ( y[0] < threshold &&
         y[1] < threshold &&
         y[2] < threshold &&
         y[3] < threshold &&
         y[4] < threshold &&
         y[5] < threshold &&
         y[6] < threshold &&
         y[7] < threshold) return false;

    if ( y[0] > threshold &&
         y[1] > threshold &&
         y[2] > threshold &&
         y[3] > threshold &&
         y[4] > threshold &&
         y[5] > threshold &&
         y[6] > threshold &&
         y[7] > threshold) return false;

    return true;
}

inline std::vector<float> MultivariateDistribution(
    const std::vector<float>& dist,
    const OnlineCovMatrix::Array& cov,
    const OnlineCovMatrix::Array& mean )
{
    std::vector<float> ret(8);
    for ( size_t i = 0; i < 8; ++i )
    {
        for ( size_t j = 0; j < 8; ++j )
        {
            ret[i] += cov[ i * 8 + j ] * dist[j];
        }
        ret[i] += mean[i];
    }
    return ret;
}

inline kvs::ValueArray<float> ProbabilityDensityFunction(
    const OnlineCovVolume::Matrix& cov,
    const OnlineCovVolume::Matrix& mean,
    const float threshold,
    const size_t samples )
{
    std::random_device seed_gen;
    std::mt19937 engine( seed_gen() );
    std::normal_distribution<> normal_dist( 0.0, 1.0 );

    const size_t size = mean.size();
    kvs::ValueArray<float> prob( size );
    for ( size_t i = 0; i < size; ++i )
    {
        int crossings = 0;
        std::vector<float> dist(8);
        for ( size_t j = 0; j < samples; ++j )
        {
            std::generate( dist.begin(), dist.end(), [&]() { return normal_dist(engine); } );

            dist = MultivariateDistribution( dist, cov[i], mean[i] );
            if ( Crossing( dist, threshold) ) crossings++;
        }
        prob[i] = (float)crossings / (float)samples;
    }
    return prob;
}

inline kvs::ValueArray<float> MeanValues(
    const OnlineCovVolume::Matrix& average,
    const kvs::Vec3ui resolution )
{
    const auto Nx = resolution.x();
    const auto Ny = resolution.y();
    const auto Nz = resolution.z();
    kvs::ValueArray<float> values( Nx * Ny * Nz );

    for ( size_t k = 0; k < Nz-1; ++k )
    {
        for ( size_t j = 0; j < Ny-1; ++j )
        {
            for ( size_t i = 0; i < Nx-1; ++i )
            {
                auto index = (Nx-1)*(Ny-1)* k + (Nx-1)*j + i;

                auto index0 = Nx*Ny * (k+0) + Nx * (j+0) + (i+0);
                auto index1 = Nx*Ny * (k+0) + Nx * (j+0) + (i+1);
                auto index2 = Nx*Ny * (k+0) + Nx * (j+1) + (i+0);
                auto index3 = Nx*Ny * (k+0) + Nx * (j+1) + (i+1);
                auto index4 = Nx*Ny * (k+1) + Nx * (j+0) + (i+0);
                auto index5 = Nx*Ny * (k+1) + Nx * (j+0) + (i+1);
                auto index6 = Nx*Ny * (k+1) + Nx * (j+1) + (i+0);
                auto index7 = Nx*Ny * (k+1) + Nx * (j+1) + (i+1);

                values[ index0 ] = average[index][0];
                values[ index1 ] = average[index][1];
                values[ index2 ] = average[index][2];
                values[ index3 ] = average[index][3];
                values[ index4 ] = average[index][4];
                values[ index5 ] = average[index][5];
                values[ index6 ] = average[index][6];
                values[ index7 ] = average[index][7];
            }
        }
    }

    return values;
}

} // end of namespace


namespace kvs
{

using ThisClass = kvs::ProbabilisticMarchingCubes;

ThisClass::SuperClass* ThisClass::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return nullptr;
    }

    using ObjectList = kvs::StructuredVolumeObjectList;
    const auto* object_list = ObjectList::DownCast( object );
    if ( !object_list )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume a object list.");
        return nullptr;
    }

    size_t index = 0;
    auto volume0 = object_list->load( index );
    if ( volume0.veclen() != 1 )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not a scalar volume.");
        return nullptr;
    }

    ::OnlineCovVolume cov_volume( volume0.resolution() - kvs::Vec3ui( 1, 1, 1 ) );
    const auto size = object_list->size();
    auto volume = volume0;
    do
    {
        cov_volume.add( volume.values() );
        volume = object_list->load( index++ );
    } while ( index < size );

    const auto average = cov_volume.average();
    const auto covariance = cov_volume.choleskyCovariance();
    const auto pdf = ::ProbabilityDensityFunction( covariance, average, m_isolevel, m_nsamples );

    m_mean_values = ::MeanValues( average, volume0.resolution() );

    switch ( volume0.gridType() )
    {
    case kvs::StructuredVolumeObject::GridType::Uniform:
    {
        this->filtering_uniform( volume0, pdf );
        break;
    }
    case kvs::StructuredVolumeObject::GridType::Rectilinear:
    {
        this->filtering_rectilinear( volume0, pdf );
        break;
    }
    default:
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Not supported grid type.");
        return nullptr;
    }
    }

    return this;
}

void ThisClass::filtering_uniform(
    const kvs::StructuredVolumeObject& volume,
    const kvs::ValueArray<float>& values )
{
    kvs::InverseDistanceWeighting<float> idw( volume.numberOfNodes() );

    auto IndexOf = [&] ( const kvs::Vec3ui& ijk )
    {
        const auto dimx = volume.resolution().x();
        const auto dimy = volume.resolution().y();
        return ijk.z() * dimx * dimy + ijk.y() * dimx + ijk.x();
    };

    auto Distance = [] ()
    {
        return kvs::Vec3( 0.5f, 0.5f, 0.5f ).length();
    };

    auto Insert = [&] ( const kvs::Vec3ui& base_ijk, const float value )
    {
        const auto distance = Distance();
        for ( size_t k = 0; k < 2; ++k )
        {
            for ( size_t j = 0; j < 2; ++j )
            {
                for ( size_t i = 0; i < 2; ++i )
                {
                    const auto ijk = kvs::Vec3ui( i, j, k );
                    const auto index = IndexOf( base_ijk + ijk );
                    idw.insert( index, value, distance );
                }
            }
        }
    };

    const auto dims = volume.resolution() - kvs::Vec3ui( 1, 1, 1 );
    for ( size_t k = 0, grid_index = 0; k < dims.z(); ++k )
    {
        for ( size_t j = 0; j < dims.y(); ++j )
        {
            for ( size_t i = 0; i < dims.x(); ++i, ++grid_index )
            {
                const auto ijk = kvs::Vec3ui( i, j, k );
                const auto value = values[ grid_index ];
                Insert( ijk, value );
            }
        }
    }

    SuperClass::setGridTypeToUniform();
    SuperClass::setVeclen( volume.veclen() );
    SuperClass::setResolution( volume.resolution() );
    SuperClass::setValues( idw.serialize() );
    SuperClass::updateMinMaxValues();
    SuperClass::updateMinMaxCoords();
}

void ThisClass::filtering_rectilinear(
    const kvs::StructuredVolumeObject& volume,
    const kvs::ValueArray<float>& values )
{
    kvs::InverseDistanceWeighting<float> idw( volume.numberOfNodes() );
    auto IndexOf = [&] ( const kvs::Vec3ui& ijk )
    {
        const auto dimx = volume.resolution().x();
        const auto dimy = volume.resolution().y();
        return ijk.z() * dimx * dimy + ijk.y() * dimx + ijk.x();
    };
    auto Distance = [&] ( const kvs::Vec3ui base_ijk )
    {
        const auto* xcoords = volume.coords().data();
        const auto* ycoords = xcoords + volume.resolution().x();
        const auto* zcoords = ycoords + volume.resolution().y();
        const auto x0 = xcoords[ base_ijk.x() + 0 ];
        const auto x1 = xcoords[ base_ijk.x() + 1 ];
        const auto y0 = ycoords[ base_ijk.y() + 0 ];
        const auto y1 = ycoords[ base_ijk.y() + 1 ];
        const auto z0 = zcoords[ base_ijk.z() + 0 ];
        const auto z1 = zcoords[ base_ijk.z() + 1 ];
        return kvs::Vec3( x1 - x0, y1 - y0, z1 - z0 ).length() * 0.5f;
    };
    auto Insert = [&] ( const kvs::Vec3ui& base_ijk, const float value )
    {
        const auto distance = Distance( base_ijk );
        for ( size_t k = 0; k < 2; ++k )
        {
            for ( size_t j = 0; j < 2; ++j )
            {
                for ( size_t i = 0; i < 2; ++i )
                {
                    const auto local_ijk = kvs::Vec3ui( i, j, k );
                    const auto index = IndexOf( base_ijk + local_ijk );
                    idw.insert( index, value, distance );
                }
            }
        }
    };

    const auto dims = volume.resolution() - kvs::Vec3ui( 1, 1, 1 );
    for ( size_t k = 0, grid_index = 0; k < dims.z(); ++k )
    {
        for ( size_t j = 0; j < dims.y(); ++j )
        {
            for ( size_t i = 0; i < dims.x(); ++i, ++grid_index )
            {
                const auto ijk = kvs::Vec3ui( i, j, k );
                const auto value = values[ grid_index ];
                Insert( ijk, value );
            }
        }
    }

    SuperClass::setGridTypeToRectilinear();
    SuperClass::setVeclen( volume.veclen() );
    SuperClass::setResolution( volume.resolution() );
    SuperClass::setCoords( volume.coords() );
    SuperClass::setValues( idw.serialize() );
    SuperClass::updateMinMaxValues();
    SuperClass::updateMinMaxCoords();
}

} // end of namespace kvs
