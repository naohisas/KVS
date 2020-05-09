#pragma once
#include <kvs/Type>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Gamma function class.
 */
/*===========================================================================*/
class GammaFunction
{
public:
    GammaFunction(){}

    kvs::Real64 operator () ( const kvs::Real64 x ) const;

    kvs::Real64 log( const kvs::Real64 x ) const;
    kvs::Real64 p( const kvs::Real64 a, const kvs::Real64 x ) const;
    kvs::Real64 q( const kvs::Real64 a, const kvs::Real64 x ) const;
    kvs::Real64 gamma( const kvs::Real64 x ) const;
    kvs::Real64 digamma( const kvs::Real64 x ) const;
    kvs::Real64 lowerIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const;
    kvs::Real64 upperIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const;
    kvs::Real64 regularizedLowerIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const;
    kvs::Real64 regularizedUpperIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const;

private:
    kvs::Real64 series( const kvs::Real64 a, const kvs::Real64 x ) const;
    kvs::Real64 continued_fraction( const kvs::Real64 a, const kvs::Real64 x ) const;
};

} // end of namespace kvs
