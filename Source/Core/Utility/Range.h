/*****************************************************************************/
/**
 *  @file   Range.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Assert>
#include <kvs/Value>
#include <kvs/ValueArray>
#include <kvs/AnyValueArray>
#include <kvs/Math>


namespace kvs
{

class Range
{
private:
    double m_lower = kvs::Value<double>::Max();
    double m_upper = kvs::Value<double>::Min();

public:
    Range() = default;

    Range( double lower, double upper )
    {
        KVS_ASSERT( lower <= upper );
        m_lower = lower;
        m_upper = upper;
    }

    bool contains( double x, bool proper = false ) const
    {
        if ( proper )
        {
            return m_lower < x && x < m_upper;
        }
        else
        {
            return m_lower <= x && x <= m_upper;
        }
    }

    bool equals( const Range& other ) const
    {
        return
            kvs::Math::Equal( m_lower, other.lower() ) &&
            kvs::Math::Equal( m_upper, other.upper() );
    }

    void extend( double x )
    {
        m_lower = m_lower < x ? m_lower : x;
        m_upper = m_upper > x ? m_upper : x;
    }

    void extend( const Range& other )
    {
        m_lower = m_lower < other.m_lower ? m_lower : other.m_lower;
        m_upper = m_upper > other.m_upper ? m_upper : other.m_upper;
    }

    bool hasLength() const
    {
        return m_lower < m_upper;
    }

    double lower() const
    {
        return m_lower;
    }

    double upper() const
    {
        return m_upper;
    }
};

template <typename T>
kvs::Range CalculateValueRange( const kvs::ValueArray<T>& ary, int dim );

kvs::Range CalculateValueRange( const kvs::AnyValueArray& ary, int dim );

} // end of namespace kvs
