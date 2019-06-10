#pragma once
#include <kvs/Type>
#include <kvs/ValueArray>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Gauss distribution function.
 */
/*===========================================================================*/
class GaussDistribution
{
private:
    kvs::Real64 m_mean; ///< mean value
    kvs::Real64 m_sdev; ///< standard deviation

public:
    GaussDistribution( const kvs::Real64 mean, const kvs::Real64 sdev );

    kvs::Real64 mean() const { return m_mean; }
    kvs::Real64 sdev() const { return m_sdev; }
    kvs::Real64 pdf( const kvs::Real64 x ) const;
    kvs::Real64 cdf( const kvs::Real64 x ) const;

    template <typename T>
    kvs::ValueArray<T> pdf( const kvs::ValueArray<T>& x ) const
    {
        const size_t n = x.size();
        kvs::ValueArray<T> values( n );
        for ( size_t i = 0; i < n; i++ ) { values[i] = this->pdf( x[i] ); }
        return values;
    }

    template <typename T>
    kvs::ValueArray<T> cdf( const kvs::ValueArray<T>& x ) const
    {
        const size_t n = x.size();
        kvs::ValueArray<T> values( n );
        for ( size_t i = 0; i < n; i++ ) { values[i] = this->cdf( x[i] ); }
        return values;
    }
};

} // end of namespace kvs
