/****************************************************************************/
/**
 *  @file   ImporterBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImporterBase.h 1795 2014-08-01 08:38:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/FileFormatBase>
#include <kvs/ObjectBase>
#include <kvs/Module>


namespace kvs
{

/*==========================================================================*/
/**
 *  Importer base class.
 */
/*==========================================================================*/
class ImporterBase
{
    kvsModuleBase( kvs::ImporterBase );

private:
    bool m_is_success; ///< check flag for importing

public:
    ImporterBase() {}
    virtual ~ImporterBase() {}

    bool isSuccess() const { return m_is_success; }
    bool isFailure() const { return !m_is_success; }
    virtual kvs::ObjectBase* exec( const kvs::FileFormatBase* file_format ) = 0;

protected:
    void setSuccess( const bool success ) { m_is_success = success; }
};

} // end of namespace kvs
