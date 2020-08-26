/*****************************************************************************/
/**
 *  @file   LUDecomposer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "LUDecomposition.h"

namespace kvs
{

// KVS_DEPRECATED
template <typename T> using LUDecomposer = LUDecomposition<T>;

} // end of namespace kvs
