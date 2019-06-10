#pragma once
#include <kvs/Type>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Beta function class.
 */
/*===========================================================================*/
class BetaFunction
{
public:
    BetaFunction(){}

    kvs::Real64 operator () ( const kvs::Real64 a, const kvs::Real64 b ) const;

    kvs::Real64 log( const kvs::Real64 a, const kvs::Real64 b ) const;
    kvs::Real64 p( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;
    kvs::Real64 q( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;
    kvs::Real64 beta( const kvs::Real64 a, const kvs::Real64 b ) const;
    kvs::Real64 lowerIncompleteBeta( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;
    kvs::Real64 upperIncompleteBeta( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;
    kvs::Real64 regularizedUpperIncompleteBeta( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;
    kvs::Real64 regularizedLowerIncompleteBeta( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;

private:
    kvs::Real64 continued_fraction( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const;
};

} // end of namespace kvs
