/*****************************************************************************/
/**
 *  @file   Device.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include "../CUDA.h"
#include <string>
#include <kvs/Vector3>


namespace kvs
{

namespace cuda
{

namespace RuntimeAPI
{

/*===========================================================================*/
/**
 *  @brief  Device class (runtime API).
 */
/*===========================================================================*/
class Device
{
public:

    typedef int Handler;
    typedef cudaDeviceProp Property;

private:

    Handler m_handler; ///< device handler
    Property m_property; ///< cuda device properties
    size_t m_free_memory; ///< free amount of memory on the device
    size_t m_total_memory; ///< total amount of memory on the device

public:

    static bool IsEnabled();
    static int Count();

public:

    Device();
    Device( const int ordinal );
    ~Device();

    bool create( const int ordinal );
    void update();

    Handler handler();
    int majorRevision() const;
    int minorRevision() const;
    std::string name() const;
    size_t totalMemory() const;
    size_t freeMemory() const;
    int maxThreadsPerBlock() const;
    kvs::Vector3i maxThreadsDimension() const;
    kvs::Vector3i maxGridSize() const;
    int sharedMemoryPerBlock() const;
    int totalConstantMemory() const;
    int warpSize() const;
    int memoryPitch() const;
    int registersPerBlock() const;
    int clockRate() const;
    int textureAlignment() const;
};

} // end of namespace RuntimeAPI

} // end of namespace cuda

} // end of namespace kvs
