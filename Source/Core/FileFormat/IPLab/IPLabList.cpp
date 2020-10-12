/*****************************************************************************/
/**
 *  @file   IPLabList.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "IPLabList.h"


namespace kvs
{

bool IPLabList::read( const std::string& dirname )
{
    kvs::Directory dir( dirname );
    if ( !dir.exists() ) { return false; }

    for ( const auto& file : dir.fileList() )
    {
        if ( file.extension() == "ipl" || file.extension() == "ipm" )
        {
            m_filenames.push_back( file.filePath() );
        }
    }

    return true;
}

kvs::IPLab IPLabList::load( const size_t index ) const
{
    kvs::IPLab ipl;
    ipl.read( m_filenames[index] );
    return ipl;
}

} // end of namespace kvs
