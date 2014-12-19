/*****************************************************************************/
/**
 *  @file   DataDescriptorFile.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataDescriptorFile.h 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GRADS__DATA_DESCRIPTOR_H_INCLUDE
#define KVS__GRADS__DATA_DESCRIPTOR_H_INCLUDE

#include <iostream>
#include <fstream>
#include <kvs/Indent>
#include "DSet.h"
#include "Undef.h"
#include "Title.h"
#include "Options.h"
#include "XDef.h"
#include "YDef.h"
#include "ZDef.h"
#include "TDef.h"
#include "Vars.h"


namespace kvs
{

namespace grads
{

/*===========================================================================*/
/**
 *  @brief  DataDescriptorFile class.
 */
/*===========================================================================*/
class DataDescriptorFile
{
private:

    kvs::grads::DSet m_dset; ///< data filename (DSET)
    kvs::grads::Undef m_undef; ///< undefined data (UNDEF)
    kvs::grads::Title m_title; ///< brief description of the data (TITLE)
    kvs::grads::Options m_options; ///< option list (OPTIONS)
    kvs::grads::XDef m_xdef; ///< grid point values for the X dimension (XDEF)
    kvs::grads::YDef m_ydef; ///< grid point values for the Y dimension (YDEF)
    kvs::grads::ZDef m_zdef; ///< grid point values for the Z dimension (ZDEF)
    kvs::grads::TDef m_tdef; ///< grid point values for the T dimension (TDEF)
    kvs::grads::Vars m_vars; ///< all variables in the data set (VARS)

public:

    DataDescriptorFile();

public:

    const kvs::grads::DSet& dset() const;
    const kvs::grads::Undef& undef() const;
    const kvs::grads::Title& title() const;
    const kvs::grads::Options& options() const;
    const kvs::grads::XDef& xdef() const;
    const kvs::grads::YDef& ydef() const;
    const kvs::grads::ZDef& zdef() const;
    const kvs::grads::TDef& tdef() const;
    const kvs::grads::Vars& vars() const;

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs

#endif // KVS__GRADS__DATA_DESCRIPTOR_H_INCLUDE
