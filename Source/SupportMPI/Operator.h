/*****************************************************************************/
/**
 *  @file   Operator.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "MPI.h"


namespace kvs
{

namespace mpi
{

namespace Operator
{

template <typename T>
struct Max
{
    const T& operator () ( const T& x, const T& y ) const { return x < y ? y : x; }
};

template <typename T>
struct Min
{
    const T& operator () ( const T& x, const T& y ) const { return x < y ? x : y; }
};

template <typename T>
struct Sum
{
    T operator () ( const T& x, const T& y ) const { return x + y; }
};

template <typename T>
struct Prod
{
    T operator () ( const T& x, const T& y ) const { return x * y; }
};

template <typename T>
struct LogicalAND
{
    bool operator () ( const T& x, const T& y ) const { return x && y; }
};

template <typename T>
struct BitwiseAND
{
    T operator () ( const T& x, const T& y ) const { return x & y; }
};

template <typename T>
struct LogicalOR
{
    bool operator () ( const T& x, const T& y ) const { return x || y; }
};

template <typename T>
struct BitwiseOR
{
    T operator () ( const T& x, const T& y ) const { return x | y; }
};

template <typename T>
struct LogicalXOR
{
    bool operator () ( const T& x, const T& y ) const { return ( x || y ) && !( x && y ); }
};

template <typename T>
struct BitwiseXOR
{
    T operator () ( const T& x, const T& y ) const { return x ^ y; }
};

} // end of namespace Operator

template <typename Op, typename T>
struct OperatorType { static MPI_Op Enum() { return MPI_OP_NULL; } };

template <typename T>
struct OperatorType<Operator::Max<T>,T> { static MPI_Op Enum() { return MPI_MAX; } };

template <typename T>
struct OperatorType<Operator::Min<T>,T> { static MPI_Op Enum() { return MPI_MIN; } };

template <typename T>
struct OperatorType<Operator::Sum<T>,T> { static MPI_Op Enum() { return MPI_SUM; } };

template <typename T>
struct OperatorType<Operator::Prod<T>,T> { static MPI_Op Enum() { return MPI_PROD; } };

template <typename T>
struct OperatorType<Operator::LogicalAND<T>,T> { static MPI_Op Enum() { return MPI_LAND; } };

template <typename T>
struct OperatorType<Operator::BitwiseAND<T>,T> { static MPI_Op Enum() { return MPI_BAND; } };

template <typename T>
struct OperatorType<Operator::LogicalOR<T>,T> { static MPI_Op Enum() { return MPI_LOR; } };

template <typename T>
struct OperatorType<Operator::BitwiseOR<T>,T> { static MPI_Op Enum() { return MPI_BOR; } };

template <typename T>
struct OperatorType<Operator::LogicalXOR<T>,T> { static MPI_Op Enum() { return MPI_LXOR; } };

template <typename T>
struct OperatorType<Operator::BitwiseXOR<T>,T> { static MPI_Op Enum() { return MPI_BXOR; } };

template <typename Op>
class UserDefinedOperator
{
private:
    MPI_Op m_op;
    static Op* m_opointer;

public:
    UserDefinedOperator( Op& op )
    {
    }
};

} // end of namespace mpi

} // end of namespace kvs
