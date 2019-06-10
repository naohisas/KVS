#pragma once
#include <kvs/Type>
#include <kvs/ValueArray>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Exponential distribution class.
 */
/*===========================================================================*/
class ExponentialDistribution
{
private:
    kvs::Real64 m_lambda; ///< lambda value

public:
    ExponentialDistribution( const kvs::Real64 lambda );

    kvs::Real64 lambda() const { return m_lambda; }
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
