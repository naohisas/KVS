/*****************************************************************************/
/**
 *  @file   BDMLData.h
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
#include <string>
#include <iostream>
#include <kvs/FileFormatBase>
#include <kvs/Indent>
#include "BDMLTag.h"
#include "InfoTag.h"
#include "DataTag.h"


namespace kvs
{

class BDMLData : public kvs::FileFormatBase
{
public:
    typedef kvs::FileFormatBase BaseClass;

public:
    static bool CheckExtension( const std::string& filename );

private:
    bdml::BDMLTag m_bdml;
    bdml::InfoTag m_info;
    bdml::DataTag m_data;

public:

    BDMLData() {}
    BDMLData( const std::string& filename );

    const bdml::BDMLTag& bdml() const { return m_bdml; }
    const bdml::InfoTag& info() const { return m_info; }
    const bdml::DataTag& data() const { return m_data; }

    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );

private:
    bool write( const std::string& filename );
};

} // end of namespace kvs
