#pragma once
#include <functional>
#include <mpi.h>


namespace kvs
{

namespace mpi
{

namespace Operator
{

template <typename T>
struct Max : public std::binary_function<T,T,T>
{
    const T& operator () ( const T& x, const T& y ) const { return x < y ? y : x; }
};

template <typename T>
struct Min : public std::binary_function<T,T,T>
{
    const T& operator () ( const T& x, const T& y ) const { return x < y ? x : y; }
};

template <typename T>
struct Sum : public std::binary_function<T,T,T>
{
    T operator () ( const T& x, const T& y ) const { return x + y; }
};

template <typename T>
struct Prod : public std::binary_function<T,T,T>
{
    T operator () ( const T& x, const T& y ) const { return x * y; }
};

template <typename T>
struct LogicalAND : public std::binary_function<T,T,T>
{
    bool operator () ( const T& x, const T& y ) const { return x && y; }
};

template <typename T>
struct BitwiseAND : public std::binary_function<T,T,T>
{
    T operator () ( const T& x, const T& y ) const { return x & y; }
};

template <typename T>
struct LogicalOR : public std::binary_function<T,T,T>
{
    bool operator () ( const T& x, const T& y ) const { return x || y; }
};

template <typename T>
struct BitwiseOR : public std::binary_function<T,T,T>
{
    T operator () ( const T& x, const T& y ) const { return x | y; }
};

template <typename T>
struct LogicalXOR : public std::binary_function<T,T,T>
{
    bool operator () ( const T& x, const T& y ) const { return ( x || y ) && !( x && y ); }
};

template <typename T>
struct BitwiseXOR : public std::binary_function<T,T,T>
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
