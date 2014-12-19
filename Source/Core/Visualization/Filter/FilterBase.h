/****************************************************************************/
/**
 *  @file   FilterBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FilterBase.h 1795 2014-08-01 08:38:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__FILTER_BASE_H_INCLUDE
#define KVS__FILTER_BASE_H_INCLUDE

#include <kvs/ObjectBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Filter base class.
 */
/*==========================================================================*/
class FilterBase
{
    kvsModuleBase( kvs::FilterBase );

private:

    bool m_is_success; ///< check flag for the filter process

public:

    FilterBase() {}
    virtual ~FilterBase() {}

    bool isSuccess() const { return m_is_success; }
    bool isFailure() const { return !m_is_success; }
    virtual kvs::ObjectBase* exec( const kvs::ObjectBase* object ) = 0;

protected:

    void setSuccess( const bool success ) { m_is_success = success; }
};

} // end of namespace kvs

#endif // KVS__FILTER_BASE_H_INCLUDE
