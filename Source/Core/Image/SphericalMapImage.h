#pragma once
#include <kvs/ColorImage>


namespace kvs
{

class CubeMapImage;

/*===========================================================================*/
/**
 *  @brief  Spherical map image class.
 */
/*===========================================================================*/
class SphericalMapImage : public kvs::ColorImage
{
public:
    SphericalMapImage( const kvs::ColorImage& image );
    SphericalMapImage( const kvs::CubeMapImage& cube_image );

    void stitch( const kvs::CubeMapImage& cube_image );
};

} // end of namespace kvs
