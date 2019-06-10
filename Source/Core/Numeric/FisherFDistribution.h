#pragma once
#include <kvs/Type>
#include <kvs/ValueArray>


namespace kvs
{

class FisherFDistribution
{
private:
    size_t m_dof1; ///< degree of freedom 1
    size_t m_dof2; ///< degree of freedom 2
    kvs::Real64 m_coef; ///< coefficient

public:
    FisherFDistribution( const size_t dof1, const size_t dof2 );

    size_t dof1() const { return m_dof1; }
    size_t dof2() const { return m_dof2; }
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
