#pragma once
#include <kvs/ObjectBase>
#include <kvs/Module>
#include <kvs/SharedPointer>
#include <kvs/opencv/VideoObject>
#include <string>


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Movie object.
 */
/*===========================================================================*/
class MovieObject : public kvs::opencv::VideoObject
{
    kvsModule( kvs::opencv::MovieObject, Object );
    kvsModuleBaseClass( kvs::opencv::VideoObject );

public:
    MovieObject() {}
    MovieObject( const std::string& filename );

    bool read( const std::string& filename );
};

} // end of namespace opencv

} // end of namespace kvs
