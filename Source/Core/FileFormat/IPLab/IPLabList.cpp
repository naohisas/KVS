#include "IPLabList.h"


namespace kvs
{

bool IPLabList::read( const std::string& dirname )
{
    kvs::Directory dir( dirname );
    if ( !dir.exists() ) { return false; }

    const size_t nfiles = dir.fileList().size();
    for ( size_t i = 0; i < nfiles; i++ )
    {
        const kvs::File& file = dir.fileList().at(i);
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
