/*****************************************************************************/
/**
 *  @file   Context.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Context.cpp 1652 2013-11-26 04:44:15Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Context.h"
#include "ErrorString.h"
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
 *  @brief  Constructs a new Context class.
 */
/*===========================================================================*/
Context::Context():
    m_handler( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Context class.
 *  @param  device [in] CUDA device
 */
/*===========================================================================*/
Context::Context( kvs::cuda::DriverAPI::Device& device )
{
    this->create( device );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Context class.
 */
/*===========================================================================*/
Context::~Context()
{
    KVS_CU_CALL( cuCtxDetach( m_handler ) );
}

/*===========================================================================*/
/**
 *  @brief  Creates a CUDA context.
 *  @param  device [in] CUDA device
 *  @return true, if the context is created successfully
 */
/*===========================================================================*/
bool Context::create( kvs::cuda::DriverAPI::Device& device )
{
    KVS_CU_CALL( cuCtxCreate( &m_handler, 0, device.handler() ) );
    if ( kvs::cuda::DriverAPI::HasError() ) return false;

    return true;
}

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
