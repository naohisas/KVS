/*****************************************************************************/
/**
 *  @file   Context.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "../CUDA.h"
#include "Device.h"


namespace kvs
{

namespace cuda
{

namespace DriverAPI
{

/*===========================================================================*/
/**
 *  @brief  Context class.
 */
/*===========================================================================*/
class Context
{
private:

    CUcontext m_handler; ///< context handler

public:

    Context();
    Context( kvs::cuda::DriverAPI::Device& device );
    ~Context();

    bool create( kvs::cuda::DriverAPI::Device& device );
};

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
