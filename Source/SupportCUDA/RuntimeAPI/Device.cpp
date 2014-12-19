/*****************************************************************************/
/**
 *  @file   Device.cpp
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
#include "Device.h"
#include "ErrorString.h"
#include "RuntimeAPI.h"
#include <kvs/Message>


namespace kvs
{

namespace cuda
{

namespace RuntimeAPI
{

/*===========================================================================*/
/**
 *  @brief  Checks the CUDA compatibility.
 *  @return true if the device supports CUDA
 */
/*===========================================================================*/
bool Device::IsEnabled()
{
    int count = 0;
    cudaError_t error = cudaGetDeviceCount( &count );
    if ( error != cudaSuccess ) return false;

    return count > 0;
}

/*===========================================================================*/
/**
 *  @brief  Returns a number of compute-capable devices.
 *  @return number of compute-capable devices
 */
/*===========================================================================*/
int Device::Count()
{
    int count = 0;
    KVS_CUDA_CALL( cudaGetDeviceCount( &count ) );

    return count;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Device class.
 */
/*===========================================================================*/
Device::Device():
    m_free_memory( 0 ),
    m_total_memory( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Device class.
 *  @param  ordinal [in] ordinal (ID of the compute-capable device)
 */
/*===========================================================================*/
Device::Device( const int ordinal )
{
    if ( this->create( ordinal ) ) this->update();
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Device class.
 */
/*===========================================================================*/
Device::~Device()
{
}

/*===========================================================================*/
/**
 *  @brief  Creates a compute-capable device.
 *  @param  ordinal [in] ordinal (ID of the compute-capable device)
 *  @return true, if the device is created successfully
 */
/*===========================================================================*/
bool Device::create( const int ordinal )
{
    m_handler = ordinal;

    KVS_CUDA_CALL( cudaSetDevice( m_handler ) );
    if ( kvs::cuda::RuntimeAPI::HasError() ) return false;

    KVS_CUDA_CALL( cudaGetDeviceProperties( &m_property, m_handler ) );
    if ( kvs::cuda::RuntimeAPI::HasError() ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Updates device information.
 */
/*===========================================================================*/
void Device::update()
{
    KVS_CUDA_CALL( cudaMemGetInfo( &m_free_memory, &m_total_memory ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the device handler.
 *  @return device handler
 */
/*===========================================================================*/
Device::Handler Device::handler()
{
    return m_handler;
}

/*===========================================================================*/
/**
 *  @brief  Returns the device major revision number.
 *  @return major revision number
 */
/*===========================================================================*/
int Device::majorRevision() const
{
    return m_property.major;
}

/*===========================================================================*/
/**
 *  @brief  Returns the device minor revision number.
 *  @return minor revision number
 */
/*===========================================================================*/
int Device::minorRevision() const
{
    return m_property.minor;
}

/*===========================================================================*/
/**
 *  @brief  Returns the device name.
 *  @return device name
 */
/*===========================================================================*/
std::string Device::name() const
{
    return m_property.name;
}

/*===========================================================================*/
/**
 *  @brief  Returns the total memory size in byte.
 *  @return total memory size in byte
 */
/*===========================================================================*/
size_t Device::totalMemory() const
{
    return m_total_memory;
}

/*===========================================================================*/
/**
 *  @brief  Returns the free memory size in byte.
 *  @return free memory size in byte
 */
/*===========================================================================*/
size_t Device::freeMemory() const
{
    return m_free_memory;
}

/*===========================================================================*/
/**
 *  @brief  Returns the max. number of threads per block.
 *  @return max. number of threads per block.
 */
/*===========================================================================*/
int Device::maxThreadsPerBlock() const
{
    return m_property.maxThreadsPerBlock;
}

/*===========================================================================*/
/**
 *  @brief  Returns the max. size of each dimension of a block.
 *  @return max. size of each dimension of a block.
 */
/*===========================================================================*/
kvs::Vector3i Device::maxThreadsDimension() const
{
    return kvs::Vector3i( m_property.maxThreadsDim );
}

/*===========================================================================*/
/**
 *  @brief  Returns the max. size of each dimension of a grid.
 *  @return max. size of each dimension of a block.
 */
/*===========================================================================*/
kvs::Vector3i Device::maxGridSize() const
{
    return kvs::Vector3i( m_property.maxGridSize );
}

/*===========================================================================*/
/**
 *  @brief  Returns the shared memory available per block in bytes.
 *  @return shared memory available per block
 */
/*===========================================================================*/
int Device::sharedMemoryPerBlock() const
{
    return m_property.sharedMemPerBlock;
}

/*===========================================================================*/
/**
 *  @brief  Returns the constant memory available on device in bytes.
 *  @return constant memory available on device
 */
/*===========================================================================*/
int Device::totalConstantMemory() const
{
    return m_property.totalConstMem;
}

/*===========================================================================*/
/**
 *  @brief  Returns the warp size in threads.
 *  @return warp size in threads
 */
/*===========================================================================*/
int Device::warpSize() const
{
    return m_property.warpSize;
}

/*===========================================================================*/
/**
 *  @brief  Returns the max. pitch in bytes allowed by memory copies.
 *  @return max. pitch
 */
/*===========================================================================*/
int Device::memoryPitch() const
{
    return m_property.memPitch;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of registers available per block.
 *  @return number of registers available per block
 */
/*===========================================================================*/
int Device::registersPerBlock() const
{
    return m_property.regsPerBlock;
}

/*===========================================================================*/
/**
 *  @brief  Returns the clock frequency in KHz.
 *  @return clock frequency
 */
/*===========================================================================*/
int Device::clockRate() const
{
    return m_property.clockRate;
}

/*===========================================================================*/
/**
 *  @brief  Returns the alignment requirement for textures.
 *  @return alignment requirement for textures
 */
/*===========================================================================*/
int Device::textureAlignment() const
{
    return m_property.textureAlignment;
}

} // end of namespace RuntimeAPI

} // end of namespace cuda

} // end of namespace kvs
