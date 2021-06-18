/****************************************************************************/
/**
 *  @file   ImporterBase.h
 *  @author Naohisa Sakamoto
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
