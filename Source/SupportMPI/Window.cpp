#include "Window.h"
#include <kvs/Type>

namespace kvs
{

namespace mpi
{

// Template specialization
template class Window<kvs::Int8>;
template class Window<kvs::UInt8>;
template class Window<kvs::Int16>;
template class Window<kvs::UInt16>;
template class Window<kvs::Int32>;
template class Window<kvs::UInt32>;
template class Window<kvs::Int64>;
template class Window<kvs::UInt64>;
template class Window<kvs::Real32>;
template class Window<kvs::Real64>;

} // end of namespace mpi

} // end of namespace kvs
