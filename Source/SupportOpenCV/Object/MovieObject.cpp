#include "MovieObject.h"


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MovieObject.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
MovieObject::MovieObject( const std::string& filename )
{
    if ( !this->read( filename ) )
    {
        kvsMessageError( "Cannot read %s.", filename.c_str() );
        return;
    }
}

/*===========================================================================*/
/**
 *  @brief  Read movie file.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool MovieObject::read( const std::string& filename )
{
    if ( !BaseClass::createCaptureDevice( filename ) )
    {
        kvsMessageError( "Cannot create capture device from %s.", filename.c_str() );
        return false;
    }

    if ( !BaseClass::initialize( BaseClass::device() ) )
    {
        kvsMessageError( "Cannot initialize movie object from %s.", filename.c_str() );
        return false;
    }

    return true;
}

} // end of namespace opencv

} // end of namespace kvs
