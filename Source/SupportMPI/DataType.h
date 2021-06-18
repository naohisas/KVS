/*****************************************************************************/
/**
 *  @file   DataType.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

namespace kvs
{

namespace mpi
{

template <typename T>
struct DataType
{
    static MPI_Datatype Enum() { return MPI_DATATYPE_NULL; }
};

template <> inline MPI_Datatype DataType<kvs::Int8>::Enum() { return MPI_CHAR; }
template <> inline MPI_Datatype DataType<kvs::UInt8>::Enum() { return MPI_UNSIGNED_CHAR; }
template <> inline MPI_Datatype DataType<kvs::Int16>::Enum() { return MPI_SHORT; }
template <> inline MPI_Datatype DataType<kvs::UInt16>::Enum() { return MPI_UNSIGNED_SHORT; }
template <> inline MPI_Datatype DataType<kvs::Int32>::Enum() { return MPI_INT; }
template <> inline MPI_Datatype DataType<kvs::UInt32>::Enum() { return MPI_UNSIGNED; }
template <> inline MPI_Datatype DataType<kvs::Real32>::Enum() { return MPI_FLOAT; }
template <> inline MPI_Datatype DataType<kvs::Real64>::Enum() { return MPI_DOUBLE; }

} // end of namespace mpi

} // end of namespace kvs
