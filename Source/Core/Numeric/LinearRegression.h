#pragma once
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/Vector>
#include <kvs/Type>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Linear regression class.
 */
/*===========================================================================*/
template <typename T>
class LinearRegression
{
private:
    size_t m_dof; ///< degree of freedom
    kvs::Vector<T> m_coef; ///< coefficients (m_eof[0]: intersect)
    kvs::Real64 m_r2; ///< R square
    kvs::Real64 m_adjusted_r2; ///< adjusted R square
    kvs::Vector<T> m_standard_errors; ///< standard errors
    kvs::Vector<T> m_t_values; ///< t-values
    kvs::Vector<T> m_p_values; ///< p-values

public:
    LinearRegression();
    LinearRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep );

    const kvs::Vector<T>& coef() const { return m_coef; }
    kvs::Real64 r2() const { return m_r2; }
    kvs::Real64 adjustedR2() const { return m_adjusted_r2; }
    const kvs::Vector<T>& standardErrors() const { return m_standard_errors; }
    const kvs::Vector<T>& tValues() const { return m_t_values; }
    const kvs::Vector<T>& pValues() const { return m_p_values; }

    void fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep );
    void test();
};

} // end of namespace kvs
