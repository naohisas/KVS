/*****************************************************************************/
/**
 *  @file   DataDescriptorFile.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
    kvs::grads::DSet m_dset{}; ///< data filename (DSET)
    kvs::grads::Undef m_undef{}; ///< undefined data (UNDEF)
    kvs::grads::Title m_title{}; ///< brief description of the data (TITLE)
    kvs::grads::Options m_options{}; ///< option list (OPTIONS)
    kvs::grads::XDef m_xdef{}; ///< grid point values for the X dimension (XDEF)
    kvs::grads::YDef m_ydef{}; ///< grid point values for the Y dimension (YDEF)
    kvs::grads::ZDef m_zdef{}; ///< grid point values for the Z dimension (ZDEF)
    kvs::grads::TDef m_tdef{}; ///< grid point values for the T dimension (TDEF)
    kvs::grads::Vars m_vars{}; ///< all variables in the data set (VARS)

public:
    DataDescriptorFile() = default;

    const kvs::grads::DSet& dset() const { return m_dset; }
    const kvs::grads::Undef& undef() const { return m_undef; }
    const kvs::grads::Title& title() const { return m_title; }
    const kvs::grads::Options& options() const { return m_options; }
    const kvs::grads::XDef& xdef() const { return m_xdef; }
    const kvs::grads::YDef& ydef() const { return m_ydef; }
    const kvs::grads::ZDef& zdef() const { return m_zdef; }
    const kvs::grads::TDef& tdef() const { return m_tdef; }
    const kvs::grads::Vars& vars() const { return m_vars; }

    void setDSet( const kvs::grads::DSet& dset ) { m_dset = dset; }
    void setUndef( const kvs::grads::Undef& undef ) { m_undef = undef; }
    void setTitle( const kvs::grads::Title& title ) { m_title = title; }
    void setXDef( const kvs::grads::XDef& xdef ) { m_xdef = xdef; }
    void setYDef( const kvs::grads::YDef& ydef ) { m_ydef = ydef; }
    void setZDef( const kvs::grads::ZDef& zdef ) { m_zdef = zdef; }
    void setTDef( const kvs::grads::TDef& tdef ) { m_tdef = tdef; }
    void setVars( const kvs::grads::Vars& vars ) { m_vars = vars; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( std::ifstream& ifs );
};

} // end of namespace grads

} // end of namespace kvs
