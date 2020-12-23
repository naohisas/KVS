/*****************************************************************************/
/**
 *  @file   EigenDecomposer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "EigenDecomposition.h"

namespace kvs
{

// KVS_DEPRECATED
template <typename T> using EigenDecomposer = EigenDecomposition<T>;

} // end of namespace kvs
