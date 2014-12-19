/*****************************************************************************/
/**
 *  @file   Context.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Context.h 1652 2013-11-26 04:44:15Z naohisa.sakamoto@gmail.com $
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
