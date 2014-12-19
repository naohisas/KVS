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
 *  $Id: Device.cpp 1652 2013-11-26 04:44:15Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Device.h"
#include "ErrorString.h"
#include "Context.h"
#include "DriverAPI.h"
#include <kvs/Message>


namespace kvs
{

namespace cuda
{

namespace DriverAPI
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
    CUresult error = cuDeviceGetCount( &count );
    if ( error != CUDA_SUCCESS ) return false;

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
    KVS_CU_CALL( cuDeviceGetCount( &count ) );

    return count;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Device class.
 */
/*===========================================================================*/
Device::Device():
    m_handler( 0 ),
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
    KVS_CU_CALL( cuDeviceGet( &m_handler, ordinal ) );
    if ( kvs::cuda::DriverAPI::HasError() ) return false;

    KVS_CU_CALL( cuDeviceGetProperties( &m_property, m_handler ) );
    if ( kvs::cuda::DriverAPI::HasError() ) return false;

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Updates device information.
 */
/*===========================================================================*/
void Device::update()
{
    kvs::cuda::DriverAPI::Context context( *this );

#if defined( cuMemGetInfo )
    /* In this case, the function 'cuMemGetInfo' is defined to 'cuMemGetInfo_v2'
     * as "#define cuMemGetInfo cuMemGetInfo_v2". And then, the function
     * 'cuMemGetInfo_v2' is defined as follows:
     * CUresult cuMemGetInfo_v2( size_t * free, size_t * total )
     */
    KVS_CU_CALL( cuMemGetInfo( &m_free_memory, &m_total_memory ) );
#else
    /* The function 'cuMemGetInfo' is defined as follows:
     * CUresult cuMemGetInfo( unsigned int * free, unsigned int * total )
     * Therefore, the temporary parameters defined as unsigned int are used
     * to obtain the memory information.
     */
    unsigned int free_memory = 0;
    unsigned int total_memory = 0;
    KVS_CU_CALL( cuMemGetInfo( &free_memory, &total_memory ) );
    m_free_memory = static_cast<size_t>( free_memory );
    m_total_memory = static_cast<size_t>( total_memory );
#endif
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
    int major_revision = 0;
    int minor_revision = 0;
    KVS_CU_CALL( cuDeviceComputeCapability( &major_revision, &minor_revision, m_handler ) );

    return major_revision;
}

/*===========================================================================*/
/**
 *  @brief  Returns the device minor revision number.
 *  @return minor revision number
 */
/*===========================================================================*/
int Device::minorRevision() const
{
    int major_revision = 0;
    int minor_revision = 0;
    KVS_CU_CALL( cuDeviceComputeCapability( &major_revision, &minor_revision, m_handler ) );

    return minor_revision;
}

/*===========================================================================*/
/**
 *  @brief  Returns the device name.
 *  @return device name
 */
/*===========================================================================*/
std::string Device::name() const
{
    const int length = 256;
    char name[ length ];
    KVS_CU_CALL( cuDeviceGetName( name, length, m_handler ) );

    return name;
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
    return m_property.totalConstantMemory;
}

/*===========================================================================*/
/**
 *  @brief  Returns the warp size in threads.
 *  @return warp size in threads
 */
/*===========================================================================*/
int Device::warpSize() const
{
    return m_property.SIMDWidth;
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
    return m_property.textureAlign;
}

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
