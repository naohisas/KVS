#pragma once
#include "IPLab.h"
#include <kvs/Directory>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  IPLab image list class.
 */
/*===========================================================================*/
class IPLabList
{
private:
    std::vector<std::string> m_filenames; ///< filename list

public:
    IPLabList() {}
    IPLabList( const std::string& dirname ) { this->read( dirname ); }

    const std::vector<std::string>& filenames() const { return m_filenames; }
    const std::string& filename( const size_t index ) const { return m_filenames[index]; }

    bool read( const std::string& dirname );
    IPLab load( const size_t index ) const;
};

} // end of namespace kvs
